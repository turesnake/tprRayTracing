/*
 * ========================= Metal.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_METAL_H
#define TPR_METAL_H

#include "pch.h"

#include "IMaterial.h"
#include "Ray.h"
#include "HitRecord.h"
#include "distribution.h"


class Metal : public IMaterial {
public:
    explicit Metal( const glm::dvec3 albedo_, double fuzz_ ):
        albedo(albedo_),
        fuzz( fuzz_<1.0 ? fuzz_ : 1.0 )
        {}
    
    bool scatter(   const Ray       &rIn_,
                    const HitRecord &hitRec_,
                    glm::dvec3      &attenuation_,// out
                    Ray             &rScattered_ // out
                )const override{

        glm::dvec3 reflectDir = reflect( glm::normalize(rIn_.get_dir()), hitRec_.normal );
        rScattered_ = Ray(  hitRec_.point, 
                            reflectDir + this->fuzz*create_random_pos_in_unitSphere() );
        attenuation_ = this->albedo;
        return ( glm::dot(reflectDir, hitRec_.normal) > 0.0 );// same hemisphere
    }

private:
    glm::dvec3  albedo;
    double      fuzz; // [0.0, 1.0]
};



#endif 
