/*
 * ======================= Lambertian.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_LAMBERTIAN_H
#define TPR_LAMBERTIAN_H

#include "pch.h"

#include "IMaterial.h"
#include "Ray.h"
#include "HitRecord.h"
#include "distribution.h"



class Lambertian : public IMaterial {
public:
    explicit Lambertian( const glm::dvec3 albedo_ ):
        albedo(albedo_)
        {}
    
    bool scatter(   const Ray       &rIn_, // no use
                    const HitRecord &hitRec_,
                    glm::dvec3      &attenuation_,// out
                    Ray             &rScattered_ // out
                )const override{

        glm::dvec3 scatterDir =  hitRec_.normal + create_random_pos_on_unitSphere();
        rScattered_ = Ray( hitRec_.point, scatterDir, rIn_.get_time() );
        attenuation_ = this->albedo;
        return true;           
    }

private:
    glm::dvec3  albedo;

};



#endif 
