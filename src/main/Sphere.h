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

#include <iostream>


class Sphere : public IHittable{
public:

    Sphere( const glm::dvec3 &center_, double radius_ ):
        center(center_),
        radius(radius_)
        {}

    const glm::dvec3 get_center()const{ return this->center; }
    double get_radius()const{ return this->radius; }


    // return hit-t
    // 一元二次方程的解，实行了一点优化，改用 half_b 来取代 b，减少了一点点运算量
    //std::optional<double> is_hit_by_ray( const Ray &r_ ){
    std::optional<HitRecord> hit( const Ray &r_, double tmin_, double tmax_ )const override{

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
                    return std::nullopt;
                }
            }

            HitRecord hitRecord {};
            hitRecord.t = t;
            hitRecord.point = r_.at(t);
            hitRecord.set_face_normal( r_, (hitRecord.point-this->center)/this->radius );
                            // param_2 be norm

            return { hitRecord };

        }else{
            return std::nullopt;
        }
    }


private:
    glm::dvec3 center {};
    double     radius {};


};




#endif 
