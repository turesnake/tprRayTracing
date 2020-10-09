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

#include "Texture.h"



class Lambertian : public IMaterial {
public:
    explicit Lambertian( const glm::dvec3 c_ ):
        albedo( std::make_shared<SolidColor>(c_) )
        {}

    explicit Lambertian( std::shared_ptr<Texture> a_ ):
        albedo(a_)
        {}
    
    bool scatter(   const Ray       &rIn_, // no use
                    const HitRecord &hitRec_,
                    glm::dvec3      &attenuation_,// out
                    Ray             &rScattered_ // out
                )const override{

        glm::dvec3 scatterDir =  hitRec_.normal + create_random_pos_on_unitSphere();
        rScattered_ = Ray( hitRec_.point, scatterDir, rIn_.get_time() );
        attenuation_ = this->albedo->value( hitRec_.u, hitRec_.v, hitRec_.point );
        return true;           
    }

private:
    std::shared_ptr<Texture> albedo;

};



#endif 
