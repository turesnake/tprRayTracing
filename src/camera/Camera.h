/*
 * ======================== Camera.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_Camera_H
#define TPR_Camera_H


#include "pch.h"

#include "Ray.h"


class Camera{
public:
    Camera( glm::dvec3 lookfrom_,// pos
            glm::dvec3 lookat_, // pos
            glm::dvec3 vup_,    // vertical-up (world y-up)
            double vfovDegree_, // vertical field-of-view in degrees
            double aspect_ratio_
    ){
        // fov plane
        double radian = glm::radians( vfovDegree_ );
        double plane_height = 2.0 * tan(radian*0.5);
        double plane_width = plane_height * aspect_ratio_;
        //double focal_length = 1.0;

        // rotation
        // local-coord:{u,v,w} just like {x,y,z}
        // both u,v,w are normal-vector
        glm::dvec3 w = - glm::normalize( lookat_ - lookfrom_ );
        glm::dvec3 u = glm::normalize( glm::cross( vup_, w ) );
        glm::dvec3 v = glm::cross( w, u );

        this->originPoint = lookfrom_;

        this->left_2_right = plane_width * u;
        this->bottom_2_top = plane_height * v;

        this->leftBottomPoint = 
            this->originPoint - this->left_2_right*0.5 - this->bottom_2_top*0.5 - w;
    }


    // a ray from viewport to render plane
    // s_:u
    // t_:v
    Ray create_a_ray( double s_, double t_ ){
        glm::dvec3 rayDir =
            this->leftBottomPoint + 
            this->left_2_right * s_ +
            this->bottom_2_top * t_ -
            this->originPoint;
        return Ray{ this->originPoint, rayDir };
    }


private:
    glm::dvec3 originPoint     {};// viewport
    glm::dvec3 leftBottomPoint {};

    glm::dvec3 left_2_right {};// vector
    glm::dvec3 bottom_2_top {};// vector

};







#endif
