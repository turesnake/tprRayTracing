/*
 * ========================= main.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- CPP --------------------//
#include <ctime>

//-------------------- Main --------------------//
#include "SysConfig.h" // 此文件由 cmake 自己生成，并不在 src/ 目录中
#include "prepare.h"


#include "write_2_png.h"

#include "global.h"


//tmp
#include "tprGeneral.h"
#include "Ray.h"
#include "Camera.h"
#include "Sphere.h"

#include <iostream>


std::string get_current_time_str();
RGBA calc_ray_color( const Ray &r_ );


// all spheres in screne
std::vector<Sphere> spheres {};

std::vector<IHittable*> hittablePtrs {};



int main( int argc, char* argv[] ){
    debug::log( "\n---------------------- BEGIN ----------------------\n\n" );
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare( argv[0] );

    
    //==========================================//
    //               test
    //------------------------------------------//
    //...

    debug::log("SCREEN = {}, {}", 
        IMAGE_W<>,
        IMAGE_H<>
    );

    //==========================================//
    //               tmp works
    //------------------------------------------//

    Camera camera {};

    spheres.push_back( Sphere{ glm::dvec3{ 0.0, 0.0, -1.0 }, 0.5 } );

    for( auto &sphere : spheres ){
        hittablePtrs.push_back( &sphere );
    }

    std::vector<RGBA> pngData ( IMAGE_W<> * IMAGE_H<>, RGBA{ 255, 0, 0, 255 } );


    debug::log("Start Rendering:");
    for( int h=0; h<IMAGE_H<>; h++ ){// bottom -> top
        std::cout<<".";// progress indicator
        for( int w=0; w<IMAGE_W<>; w++ ){// left -> right
            int idx = h*IMAGE_W<> + w;

            double u = static_cast<double>(w)/static_cast<double>(IMAGE_W<> - 1);//[0.0,1.0]
            double v = static_cast<double>(h)/static_cast<double>(IMAGE_H<> - 1);//[0.0,1.0]

            Ray r  = camera.create_a_ray( u,v );

            pngData.at(idx) = calc_ray_color( r );


        }
    }
    std::cout<<std::endl;

    

    //==========================================//
    //           write 2 png file
    //------------------------------------------//
    std::string pngFileName = "Kr_" + get_current_time_str() + ".png";
    std::string pngPath = tprGeneral::path_combine( path_output, pngFileName );
    write_2_png( pngPath, IMAGE_W<>, IMAGE_H<>, pngData, true );// [left-bottom]


    debug::log( "\n\n---------------------- END ----------------------\n" );
    return(0);
}



std::string get_current_time_str(){

    time_t timeVal {};
    time( &timeVal );
    tm *currentTime = localtime(&timeVal);
    return fmt::format(
        "{0}_{1}_{2}_{3}_{4}_{5}",
        currentTime->tm_year+1900,
        currentTime->tm_mon+1,
        currentTime->tm_mday,
        currentTime->tm_hour,
        currentTime->tm_min,
        currentTime->tm_sec
    );
}


RGBA calc_ray_color( const Ray &r_ ){

    //--- render the spheres ---//
    
    for( auto objPtr : hittablePtrs ){

        auto hitRet = objPtr->hit( r_, -100.01, 100.0 );
        if( hitRet.has_value() ){

            HitRecord &hret = hitRet.value();

            glm::dvec3 c { hret.normal.x+1.0, hret.normal.y+1.0, hret.normal.z+1.0 };
            c *= 0.5;

            return RGBA::dvec3_2_RGBA( c, 255 );

        }
    }

    //--- render the sky ---//
    glm::dvec3 rayNormDir = glm::normalize( r_.get_dir() );

    double t = 0.5 * ( rayNormDir.y + 1.0 );

    glm::dvec3 color_white { 1.0, 1.0, 1.0 };// white
    glm::dvec3 color_lightBlue { 0.5, 0.7, 1.0 };// blue

    glm::dvec3 skyColor = tprMath::lerp( color_white, color_lightBlue, t );


    return RGBA::dvec3_2_RGBA( skyColor, 255 );

    


}













