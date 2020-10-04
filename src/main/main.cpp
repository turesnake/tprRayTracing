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

#include "WorldObjs.h"


//tmp
#include "tprGeneral.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Camera.h"
#include "Sphere.h"

#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"


#include <iostream>



std::string get_current_time_str();
glm::dvec3 calc_ray_color( const Ray &r_, int boundN_ );




// all spheres in screne
std::vector<std::unique_ptr<Sphere>> spheres {};
std::vector<std::unique_ptr<IMaterial>> matPtrs {};

WorldObjs worldObjs {};


void create_objs_1();
void create_objs_2();


int main( int argc, char* argv[] ){
    debug::log( "\n---------------------- BEGIN ----------------------\n\n" );
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare( argv[0] );
    
    debug::log("SCREEN = {}, {}", IMAGE_W<>, IMAGE_H<> );

    //==========================================//
    //               test
    //------------------------------------------//
    //...
    /*
    for( int i=0; i<20; i++ ){
        debug::log( "{}", tprMath::get_random_double( -1.0, 1.0 ) );
    }
    */
    //return 0;

    //==========================================//
    //             mats, objs
    //------------------------------------------//

    //----- camera -----//
    Camera camera { glm::dvec3{ -2.0, 1.0, 1.0 },// lookfrom
                    glm::dvec3{ 0.0, 0.0, -1.0 },// lookat
                    glm::dvec3{ 0.0, 1.0, 0.0 },// world-up
                    20, 
                    ASPECT_RATIO<> };


    //----- mats -----//
    //----- objs -----//
    //create_objs_1();
    create_objs_2();
    

    /*
    // two balls
    auto mat_left = std::make_unique<Lambertian>( glm::dvec3{ 0.0, 0.0, 1.0 } );
    auto mat_right = std::make_unique<Lambertian>( glm::dvec3{ 1.0, 0.0, 0.0 } );

    double R = cos( tprMath::pi / 4.0 );
    //double R = 1.0;

    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ -R, 0.0, -1.0 }, R, mat_left.get() ) );
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{  R, 0.0, -1.0 }, R, mat_right.get() ) );
    */
    

    // worldObjs 
    for( auto &sphereUPtr : spheres ){
        worldObjs.add( sphereUPtr.get() );
    }
    


    //==========================================//
    //              rendering
    //------------------------------------------//
    std::vector<RGBA> pngData ( IMAGE_W<> * IMAGE_H<>, RGBA{ 255, 0, 0, 255 } );

    debug::log("Start Rendering:");
    for( int h=0; h<IMAGE_H<>; h++ ){// bottom -> top
        std::cout<<"."<<std::flush;// progress indicator
        for( int w=0; w<IMAGE_W<>; w++ ){// left -> right
            size_t idx = static_cast<size_t>( h*IMAGE_W<> + w );

            glm::dvec3 colorSum {};

            for( int s=0; s<SAMPLES_PER_PIX<>; s++ ){
                double u = (w + tprMath::get_random_double()) / (IMAGE_W<double>-1.0);//[0.0,1.0]
                double v = (h + tprMath::get_random_double()) / (IMAGE_H<double>-1.0);//[0.0,1.0]
                Ray r  = camera.create_a_ray( u,v );
                colorSum += calc_ray_color( r, BOUNDS_NUM<> );
            }

            //--- calc the average-val of all samples in one pix ---
            double scale = 1.0 / SAMPLES_PER_PIX<double>;
            // gamma by sqrt
            double r = sqrt( colorSum.x * scale );
            double g = sqrt( colorSum.y * scale );
            double b = sqrt( colorSum.z * scale );

            pngData.at(idx) = RGBA::dvec3_2_RGBA( glm::dvec3{r,g,b}, 255 );

            //debug::log( "{},{},{}", pngData.at(idx).r, pngData.at(idx).g, pngData.at(idx).b );

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


    if( current_OS == OS_WIN32 ){
        debug::log( "Please type any KEY ans RETURN to end the app:" );
        int inputStr {};
        std::cin >> inputStr;
    }

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


// create a render-color in the format of dvec3 [0.0, 1.0]
glm::dvec3 calc_ray_color( const Ray &r_, int boundN_ ){

    if(boundN_==0){ return glm::dvec3{ 1.0, 0.0, 0.0 }; }// red
    //if(boundN_==0){ return glm::dvec3{ 0.0, 0.0, 0.0 }; }// no light return

    //--- render the spheres ---//
    auto hitRecord = worldObjs.hit( r_, 0.001, tprMath::infinity );
    if( hitRecord.has_value() ){
        HitRecord &hret = hitRecord.value();

        Ray rScattered {};
        glm::dvec3 attenuation {};

        if( hret.matPtr->scatter(r_, hret, attenuation, rScattered) ){
            return attenuation * calc_ray_color( rScattered, boundN_-1 );// recursive
        }else{
            return glm::dvec3{ 1.0, 0.0, 0.0 };
        }
    }

    //--- render the sky ---//
    glm::dvec3 rayNormDir = glm::normalize( r_.get_dir() );

    double t = 0.5 * ( rayNormDir.y + 1.0 );

    glm::dvec3 color_white { 1.0, 1.0, 1.0 };// white
    glm::dvec3 color_lightBlue { 0.5, 0.7, 1.0 };// blue
    //glm::dvec3 color_lightBlue { 0.65, 1.0, 0.2 };// blue

    
    glm::dvec3 skyColor = tprMath::lerp( color_white, color_lightBlue, t );
    return skyColor;
}




IMaterial *create_mat( std::unique_ptr<IMaterial> uptr ){
    matPtrs.push_back( std::move( uptr ) );
    return matPtrs.back().get();
}


void create_objs_1(){

    //----- mats -----//
    IMaterial *mat_lambt_ground = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.8, 0.8, 0.0 } ) );

    IMaterial *mat_lambt_egg     = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.7, 0.3, 0.3 } ) );
    IMaterial *mat_lambt_midblue = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.1, 0.2, 0.5 } ) );

    IMaterial *mat_metal_silver = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.8, 0.8 }, 0.3 ) );
    IMaterial *mat_metal_gold_fuzz = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 1.0 ) );
    IMaterial *mat_metal_gold = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 0.0 ) );

    IMaterial *mat_diel_glass = create_mat( std::make_unique<Dielectric>( 1.5 ) );


    //----- objs -----//
    // ground sphere
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ 0.0, -100.5, -1.0 }, 100.0, mat_lambt_ground ) );

    // center
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.5, mat_lambt_midblue ) );

    // left
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, 0.5, mat_diel_glass ) );
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, -0.45, mat_diel_glass ) );
    
    // right
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ 1.0, 0.0, -1.0 }, 0.5, mat_metal_gold ) );

}


void create_objs_2(){

    //----- mats -----//
    IMaterial *mat_lambt_ground = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.8, 0.8, 0.0 } ) );

    IMaterial *mat_lambt_egg     = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.7, 0.3, 0.3 } ) );
    IMaterial *mat_lambt_midblue = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.1, 0.2, 0.5 } ) );

    IMaterial *mat_metal_silver = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.8, 0.8 }, 0.3 ) );
    IMaterial *mat_metal_gold_fuzz = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 1.0 ) );
    IMaterial *mat_metal_gold = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 0.0 ) );

    IMaterial *mat_diel_glass = create_mat( std::make_unique<Dielectric>( 1.5 ) );
    IMaterial *mat_diel_diamond = create_mat( std::make_unique<Dielectric>( 2.4 ) );


    //----- objs -----//
    // ground sphere
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ 0.0, -100.5, -1.0 }, 100.0, mat_lambt_ground ) );

    // center
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.5, mat_diel_diamond ) );
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ 0.0, 0.0, -1.0 }, -0.45, mat_diel_diamond ) );
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.3, mat_metal_gold ) );


    // left
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, 0.5, mat_diel_glass ) );
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, -0.45, mat_diel_glass ) );
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, 0.35, mat_lambt_midblue ) );

    // right
    spheres.push_back( std::make_unique<Sphere>( glm::dvec3{ 1.0, 0.0, -1.0 }, 0.5, mat_metal_silver ) );



}











