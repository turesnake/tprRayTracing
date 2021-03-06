/*
 * ======================= HitRecord.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_HIT_RECORD_H
#define TPR_HIT_RECORD_H

#include "pch.h"
#include "Ray.h"


class IMaterial;


class HitRecord{
public:
    glm::dvec3 point  {};
    glm::dvec3 normal {};
    double     t {};
    double     u {};//texture
    double     v {};//texture
    bool       is_front_face {};
    std::shared_ptr<IMaterial>  matSPtr {};


    void set_face_normal( const Ray &r_, const glm::dvec3 &outward_normal_ ){
        this->is_front_face = glm::dot( r_.get_dir(), outward_normal_ ) < 0.0;
        this->normal = this->is_front_face ? outward_normal_ : -outward_normal_;
    }

};


#endif 
