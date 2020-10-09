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


// 这个函数的 解释，书本好像存在问题 ...
inline void get_sphere_uv( const glm::dvec3 &pos_, double &out_u_, double &out_v_ ){
    double phi   = atan2( pos_.z, pos_.x ); // radian [ -pi,   pi ]
    double theta = asin( pos_.y );          // radian [ -pi/2, pi/2 ]
        // 为什么不是 acos(y) ???
        // 书上 关于 球坐标 的描述，存在问题
    out_u_ = 1.0 - (phi + tprMath::pi)/(2.0 * tprMath::pi); 
    out_v_ = ( theta + tprMath::pi*0.5 ) / tprMath::pi;
}



class Sphere : public IHittable{
public:
    Sphere()=default;

    Sphere( const glm::dvec3 &center_, 
            double radius_, 
            std::shared_ptr<IMaterial> matSPtr_ 
        ):
        center(center_),
        radius(radius_),
        matSPtr(matSPtr_)
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

            glm::dvec3 normDir = glm::normalize( record_.point - this->center );
            if( this->radius < 0.0 ){
                normDir = -normDir;
            }
                // trick: if the radius is negative, we can build a inn-sphere (i.e. glass-ball)

            record_.set_face_normal( r_, normDir );
            get_sphere_uv( normDir, record_.u, record_.v );
            record_.matSPtr = this->matSPtr;

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
    glm::dvec3 center {};
    double     radius {}; // can be negative
    std::shared_ptr<IMaterial> matSPtr {};
};




#endif 
