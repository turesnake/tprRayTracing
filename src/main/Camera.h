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
    Camera(){
        // set by hand...
        this->originPoint = glm::dvec3{ 0.0, 0.0, 0.0 };

        this->plane_height = 2.0;
        this->plane_width = this->plane_height * ASPECT_RATIO<>;
        this->focal_length = 1.0;

        this->left_2_right = glm::dvec3{ this->plane_width, 0.0, 0.0 };
        this->bottom_2_top = glm::dvec3{ 0.0, this->plane_height, 0.0 };

        this->leftBottomPoint = 
            this->originPoint - this->left_2_right*0.5 - this->bottom_2_top*0.5
            - glm::dvec3{ 0.0, 0.0, this->focal_length };

    }


    // a ray from viewport to render plane
    Ray create_a_ray( double u_, double v_ ){
        glm::dvec3 rayDir =
            this->leftBottomPoint + 
            this->left_2_right * u_ +
            this->bottom_2_top * v_ -
            this->originPoint;
        return Ray{ this->originPoint, rayDir };
    }


//private:
    glm::dvec3 originPoint     {};// viewport
    glm::dvec3 leftBottomPoint {};

    double plane_width {};
    double plane_height {};
    double focal_length {};// distance from viewport to render plane

    glm::dvec3 left_2_right {};// vector
    glm::dvec3 bottom_2_top {};// vector




};







#endif
