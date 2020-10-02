/*
 * ======================= IHittable.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_I_HIT_ABLE_H
#define TPR_I_HIT_ABLE_H

#include "pch.h"
#include "Ray.h"


class HitRecord{
public:
    glm::dvec3 point  {};
    glm::dvec3 normal {};
    double     t {};
    bool       is_front_face;

    void set_face_normal( const Ray &r_, const glm::dvec3 &outward_normal_ ){
        this->is_front_face = glm::dot( r_.get_dir(), outward_normal_ ) < 0.0;
        this->normal = this->is_front_face ? outward_normal_ : -outward_normal_;
    }

};


class IHittable{
public:
    IHittable()=default;
    virtual ~IHittable()=default;// MUST
    virtual std::optional<HitRecord> hit( const Ray &r_, double tmin_, double tmax_ )const=0;
};



#endif 
