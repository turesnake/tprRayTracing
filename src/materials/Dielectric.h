/*
 * ========================= Dielectric.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_DIELECTRIC_H
#define TPR_DIELECTRIC_H

#include "pch.h"

#include "IMaterial.h"
#include "Ray.h"
#include "HitRecord.h"
#include "distribution.h"


// glass
class Dielectric : public IMaterial {
public:
    explicit Dielectric( double refractIndex_ ):
        refractIndex(refractIndex_)
        {}
    
    bool scatter(   const Ray       &rIn_,
                    const HitRecord &hitRec_,
                    glm::dvec3      &attenuation_,// out
                    Ray             &rScattered_ // out
                )const override{
        
        attenuation_ = glm::dvec3{ 1.0, 1.0, 1.0 };
                // suppose always absords nothing

        double etaA_over_etaB = hitRec_.is_front_face ? (1.0/refractIndex) : refractIndex;
                // suppose etaA = 1.0;(air)

        glm::dvec3 normRayDir = glm::normalize(rIn_.get_dir());

        //--- total internal reflection ---//
        double cosA = std::min( glm::dot(-normRayDir, hitRec_.normal), 1.0 );
        double sinA = sqrt( 1.0 - cosA*cosA );
        if( etaA_over_etaB * sinA > 1.0 ){
            glm::dvec3 reflectDir = reflect( normRayDir, hitRec_.normal );
            rScattered_ = Ray(  hitRec_.point, reflectDir );
            return true;
        }

        //--- Schlick Approximation ---//
        double reflect_probability = schlick( cosA, etaA_over_etaB );
        if( tprMath::get_random_double() < reflect_probability ){
            // choose reflect
            glm::dvec3 reflectDir = reflect( normRayDir, hitRec_.normal );
            rScattered_ = Ray(  hitRec_.point, reflectDir );
            return true;
        }

        //--- refract ---//
        glm::dvec3 refractedDir = refract(
            glm::normalize(rIn_.get_dir()),
            hitRec_.normal,
            etaA_over_etaB
        );


        rScattered_ = Ray( hitRec_.point, refractedDir );
        return true;        
    }

private:
    double refractIndex;
};


#endif 
