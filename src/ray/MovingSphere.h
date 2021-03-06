/*
 * ======================= MovingSphere.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.07
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   implicit sphere
 * ----------------------------
 */
#ifndef TPR_MOVING_SPHERE_H
#define TPR_MOVING_SPHERE_H

#include "pch.h"

#include "Ray.h"
#include "IHittable.h"
#include "IMaterial.h"
#include "HitRecord.h"


#include "Sphere.h"




class MovingSphere : public IHittable{
public:

    MovingSphere(   const glm::dvec3 &center0_, 
                    const glm::dvec3 &center1_,
                    double time0_,
                    double time1_,
                    double radius_, 
                    std::shared_ptr<IMaterial> matSPtr_ 
        ):
        center0(center0_),
        center1(center1_),
        time0(time0_),
        time1(time1_),
        radius(radius_),
        matSPtr(matSPtr_)
        {}

    const glm::dvec3 get_center( double t_ )const{ 
        glm::dvec3 off = center1 - center0;
        return center0 + ((t_-time0)/(time1-time0)) * off;
    }

    double get_radius()const{ return this->radius; }


    // return hit-t
    // 一元二次方程的解，实行了一点优化，改用 half_b 来取代 b，减少了一点点运算量
    //std::optional<double> is_hit_by_ray( const Ray &r_ ){
    bool hit( const Ray &r_, double tmin_, double tmax_, HitRecord &record_ )const override{

        glm::dvec3 currentCenter = this->get_center(r_.get_time());

        glm::dvec3 rDir = r_.get_dir();
        glm::dvec3 oc = r_.get_origin() - currentCenter;// origin - center
       
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

            glm::dvec3 normDir = glm::normalize( record_.point - currentCenter );
            record_.set_face_normal( r_, this->radius>0.0 ? normDir : -normDir );
                    // trick: if the radius is negative, we can build a inn-sphere (i.e. glass-ball)
            record_.matSPtr = this->matSPtr;

            return true;

        }else{
            return false;
        }
    }


    bool bounding_box( double t0_, double t1_, AABB &output_aabb_ )const override{
        glm::dvec3 rv { this->radius, this->radius, this->radius };
        AABB a = AABB{
            this->center0 - rv,
            this->center0 + rv,
        };
        AABB b = AABB{
            this->center1 - rv,
            this->center1 + rv,
        };
        output_aabb_ = AABB::surrounding_box( a, b );
        return true;
    }


private:
    

    glm::dvec3 center0 {};
    glm::dvec3 center1 {};

    double time0 {};
    double time1 {};

    double     radius {}; // can be negative
    std::shared_ptr<IMaterial>  matSPtr {};
};


#endif 
