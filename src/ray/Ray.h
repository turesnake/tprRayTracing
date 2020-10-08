/*
 * ========================= Ray.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_RAY_H
#define TPR_RAY_H


#include "pch.h"




class Ray{
public:
    Ray() = default;
    Ray(const glm::dvec3 &origin_, 
        const glm::dvec3 &dir_,
        double time_ = 0.0
        ):
        origin(origin_),
        dir(dir_),
        time(time_)
        {}

    const glm::dvec3 get_origin()const{ return this->origin; }
    const glm::dvec3 get_dir()const{ return this->dir; }
    double      get_time()const{ return this->time; }

    glm::dvec3 at( double t )const{
        return this->origin + this->dir * t;
    }

private:
    glm::dvec3 origin {};
    glm::dvec3 dir    {};
    double     time   {};

};







#endif
