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
    static Sphere *factory(  const glm::dvec3 &center_, 
                            double radius_, 
                            IMaterial *matPtr_ 
    ){
        Sphere::sphereUPtrs.emplace_back( std::move(
            new Sphere( center_,radius_,matPtr_ )
        ));
        return Sphere::sphereUPtrs.back().get();
    }  

    static std::vector<std::unique_ptr<Sphere>> &get_sphereUPtrs(){
        return Sphere::sphereUPtrs;
    }


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

            glm::dvec3 normDir = glm::normalize( record_.point - this->center );
            record_.set_face_normal( r_, this->radius>0.0 ? normDir : -normDir );
                    // trick: if the radius is negative, we can build a inn-sphere (i.e. glass-ball)
            record_.matPtr = this->matPtr;

            return true;

        }else{
            return false;
        }
    }

    bool bounding_box( double t0_, double t1_, AABB &output_aabb_ )const override{
        glm::dvec3 rv { this->radius, this->radius, this->radius };
        output_aabb_ = AABB{
            this->center - rv,
            this->center + rv,
        };
        return true;
    }

     
private:

    Sphere( const glm::dvec3 &center_, 
            double radius_, 
            IMaterial *matPtr_ 
        ):
        center(center_),
        radius(radius_),
        matPtr(matPtr_)
        {}

    glm::dvec3 center {};
    double     radius {}; // can be negative
    IMaterial  *matPtr {};

    static std::vector<std::unique_ptr<Sphere>> sphereUPtrs;

};

inline std::vector<std::unique_ptr<Sphere>> Sphere::sphereUPtrs {};


#endif 