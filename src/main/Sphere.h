/*
 * ========================= Sphere.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.02
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   implicit sphere
 * ----------------------------
 */
#ifndef TPR_SPHERE_H
#define TPR_SPHERE_H

#include "pch.h"

#include "Ray.h"
#include "IHittable.h"
#include "IMaterial.h"
#include "HitRecord.h"


#include <iostream>


class Sphere : public IHittable{
public:

    Sphere( const glm::dvec3 &center_, double radius_, IMaterial *matPtr_ ):
        center(center_),
        radius(radius_),
        matPtr(matPtr_)
        {}

    const glm::dvec3 get_center()const{ return this->center; }
    double get_radius()const{ return this->radius; }


    // return hit-t
    // 一元二次方程的解，实行了一点优化，改用 half_b 来取代 b，减少了一点点运算量
    //std::optional<double> is_hit_by_ray( const Ray &r_ ){
    bool hit( const Ray &r_, double tmin_, double tmax_, HitRecord &record_ )const override{

        glm::dvec3 rDir = r_.get_dir();
        glm::dvec3 oc = r_.get_origin() - this->center;// origin - center
       
        double a = glm::dot( rDir, rDir );
        double half_b = glm::dot( rDir, oc );
        double c = glm::dot(oc,oc) - this->radius*this->radius;

        double discriminant = half_b*half_b - a*c;
        if( discriminant > 0.0 ){

            double root = sqrt(discriminant);

            // first check near point, than far one
            // if both fail, just return
            double t = ( -half_b - root ) / a; // near point
            if( !(t>tmin_ && t<tmax_) ){
                t = ( -half_b + root ) / a; // far point
                if( !(t>tmin_ && t<tmax_) ){
                    return false;
                }
            }

            record_.t = t;
            record_.point = r_.at(t);
            record_.set_face_normal( r_, (record_.point-this->center)/this->radius );
                            // param_2 be norm
            record_.matPtr = this->matPtr;

            return true;

        }else{
            return false;
        }
    }


private:
    glm::dvec3 center {};
    double     radius {};
    IMaterial  *matPtr {};
};








#endif 
