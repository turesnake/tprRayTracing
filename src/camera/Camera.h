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
#include "distribution.h"


class Camera{
public:
    Camera( glm::dvec3 lookfrom_,// pos
            glm::dvec3 lookat_, // pos
            glm::dvec3 vup_,    // vertical-up (world y-up)
            double vfovDegree_, // vertical field-of-view in degrees
            double aspect_ratio_,
            double aperture_, // 光圈
            double focus_dist_, // 焦距
            double time0_=0.0,
            double time1_=0.0
    ):
    time0(time0_),
    time1(time1_)
    {
        // fov plane
        double radian = glm::radians( vfovDegree_ );
        // viewport height,width: unit length
        double viewport_height = 2.0 * tan(radian*0.5);
        double viewport_width = viewport_height * aspect_ratio_;

        // rotation
        w = - glm::normalize( lookat_ - lookfrom_ );
        u = glm::normalize( glm::cross( vup_, w ) );
        v = glm::cross( w, u );

        this->originPoint = lookfrom_;

        this->left_2_right = focus_dist_ * viewport_width * u;
        this->bottom_2_top = focus_dist_ * viewport_height * v;

        this->leftBottomPoint = 
            this->originPoint - this->left_2_right*0.5 - this->bottom_2_top*0.5 - focus_dist_*w;

        this->lens_radius = aperture_ * 0.5;
    }


    // a ray from viewport to render plane
    // s_:u
    // t_:v
    Ray create_a_ray( double s_, double t_ ){

        glm::dvec3 rd = this->lens_radius * create_random_pos_in_unitDisk();
        glm::dvec3 off = this->u*rd.x + this->v*rd.y;

        glm::dvec3 rayDir =
            this->leftBottomPoint + 
            this->left_2_right * s_ +
            this->bottom_2_top * t_ -
            this->originPoint;
        return Ray{ this->originPoint + off,
                    rayDir - off,
                    tprMath::get_random_double( this->time0, this->time1 )
                    };
    }


private:
    glm::dvec3 originPoint     {};// viewport
    glm::dvec3 leftBottomPoint {};

    glm::dvec3 left_2_right {};// vector
    glm::dvec3 bottom_2_top {};// vector

    // local-coord:{u,v,w} just like {x,y,z}
    // both u,v,w are normal-vector
    glm::dvec3 u {};
    glm::dvec3 v {};
    glm::dvec3 w {};

    double lens_radius {};

    // in current design,the camera just used once!
    // so, the render time period is static (only one period)
    double time0 {};
    double time1 {};

};







#endif
