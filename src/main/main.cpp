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

#include "HittableList.h"


//tmp
#include "tprGeneral.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Camera.h"
#include "Sphere.h"
#include "MovingSphere.h"

#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

#include "BVH.h"

#include <iostream>



std::string get_current_time_str();
glm::dvec3 calc_ray_color( const Ray &r_, int boundN_ );




// all spheres in screne
std::vector<std::unique_ptr<IMaterial>> matPtrs {};

HittableList worldObjs {};
BVH_Node     *bvhPtr {nullptr}; 

HittableList worldObjs2 {}; // test


void create_scene_1();
void create_scene_2();
void create_scene_3();


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
    //return 0;
    

    // switch the random seed
    
    for( int i=0; i<15; i++ ){
        double d = tprMath::get_random_double();
    }
    

    //==========================================//
    //             mats, objs
    //------------------------------------------//

    //----- camera -----//
    glm::dvec3 lookfrom { 13.0, 2.0, 3.0 };
    glm::dvec3 lookat   { 0.0, 0.0, 0.0 };
    glm::dvec3 worldup  { 0.0, 1.0, 0.0 };
    //double dist_to_focus = glm::length( lookat - lookfrom );
    double dist_to_focus = 10.0;
    double aperture = 0.1;

    Camera camera { lookfrom,
                    lookat,
                    worldup,
                    20, 
                    ASPECT_RATIO<>,
                    aperture,
                    dist_to_focus,
                    0.0,
                    1.0
                };


    //----- mats objs -----//
    //create_scene_1();
    //create_scene_2();
    create_scene_3();


    // worldObjs 
    /*
    for( auto &sphereUPtr : Sphere::get_sphereUPtrs() ){
        worldObjs.add( sphereUPtr.get() );
    }
    for( auto &sphereUPtr : MovingSphere::get_movingSphereUPtrs() ){
        worldObjs.add( sphereUPtr.get() );
    }
    */
    


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

    //if(boundN_==0){ return glm::dvec3{ 1.0, 0.0, 0.0 }; }// red
    if(boundN_==0){ return glm::dvec3{ 0.0, 0.0, 0.0 }; }// no light return

    //--- render the spheres ---//
    HitRecord hret {};

    //if( worldObjs.hit( r_, 0.001, tprMath::infinity, hret ) ){
    if( bvhPtr->hit( r_, 0.001, tprMath::infinity, hret ) ){
    //if( worldObjs2.hit( r_, 0.001, tprMath::infinity, hret ) ){

        //debug::log( "pix hit" );

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


void create_scene_1(){

    //----- mats -----//
    IMaterial *mat_lambt_ground = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.8, 0.8, 0.0 } ) );
    //IMaterial *mat_lambt_ground = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.4, 0.5, 0.6 } ) );

    IMaterial *mat_lambt_egg     = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.7, 0.3, 0.3 } ) );
    IMaterial *mat_lambt_midblue = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.1, 0.2, 0.5 } ) );

    IMaterial *mat_metal_silver = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.8, 0.8 }, 0.0 ) );
    IMaterial *mat_metal_gold_fuzz = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 1.0 ) );
    IMaterial *mat_metal_gold = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 0.0 ) );

    IMaterial *mat_diel_glass = create_mat( std::make_unique<Dielectric>( 1.5 ) );
    IMaterial *mat_diel_diamond = create_mat( std::make_unique<Dielectric>( 2.4 ) );


    //----- objs -----//
    // ground sphere
    Sphere::factory( glm::dvec3{ 0.0, -100.5, -1.0 }, 100.0, mat_lambt_ground );

    // center
    Sphere::factory( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.5, mat_metal_silver );

    // left
    Sphere::factory( glm::dvec3{-1.0, 0.0, -1.0 }, 0.5, mat_diel_diamond );
    Sphere::factory( glm::dvec3{-1.0, 0.0, -1.0 }, -0.45, mat_diel_diamond );
    Sphere::factory( glm::dvec3{-1.0, 0.0, -1.0 }, 0.3, mat_metal_gold );
    
    // right
    Sphere::factory( glm::dvec3{ 1.0, 0.0, -1.0 }, 0.5, mat_metal_gold );

}


void create_scene_2(){

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
    Sphere::factory( glm::dvec3{ 0.0, -100.5, -1.0 }, 100.0, mat_lambt_ground );

    // center
    Sphere::factory( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.5, mat_diel_diamond );
    Sphere::factory( glm::dvec3{ 0.0, 0.0, -1.0 }, -0.45, mat_diel_diamond );
    Sphere::factory( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.3, mat_metal_gold );


    // left
    Sphere::factory( glm::dvec3{-1.0, 0.0, -1.0 }, 0.5, mat_diel_glass );
    Sphere::factory( glm::dvec3{-1.0, 0.0, -1.0 }, -0.45, mat_diel_glass );
    Sphere::factory( glm::dvec3{-1.0, 0.0, -1.0 }, 0.35, mat_lambt_midblue );

    // right
    Sphere::factory( glm::dvec3{ 1.0, 0.0, -1.0 }, 0.5, mat_metal_silver );



}


// last scene in book 《Ray Tracing in One Weekend》
void create_scene_3(){

    // lambdas
    auto get_random_color = []( double min_=0.0, double max_=1.0 )->glm::dvec3{
        return glm::dvec3{
            tprMath::get_random_double( min_, max_ ),
            tprMath::get_random_double( min_, max_ ),
            tprMath::get_random_double( min_, max_ )
        };
    };

    //----- ground -----//
    IMaterial *mat_lambt_ground = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.5, 0.5, 0.5 } ) );
    //IMaterial *mat_lambt_ground = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.75, 0.7, 0.3 } ) );
    /*
    worldObjs.add(
        //Sphere::factory( glm::dvec3{ 0.0, -1000.0, 0.0 }, 1000.0, mat_lambt_ground )
        Sphere::factory( glm::dvec3{ 0.0, -1000.0, 0.0 }, 1000.0, mat_lambt_ground )
    );
    */

    //--- many sml balls ---//
    for( int a=-11; a<11; a++ ){
        for( int b=-11; b<11; b++ ){
            double choose_mat = tprMath::get_random_double();

            glm::dvec3 center { a + 0.9*tprMath::get_random_double(),
                                0.2,
                                b + 0.9*tprMath::get_random_double() };

            if( glm::length(center - glm::dvec3{4.0, 0.2, 0.0}) > 0.9 ){
                std::unique_ptr<IMaterial> matUPtr {};

                if( choose_mat < 0.7 ){
                    // diffuse
                    glm::dvec3 albedo = get_random_color();
                    IMaterial *mat = create_mat( std::make_unique<Lambertian>( albedo ) );
                    
                    glm::dvec3 center2 = center + glm::dvec3{
                        0.0, 
                        tprMath::get_random_double( 0.0, 0.5 ),
                        0.0
                    };
                    worldObjs.add(
                        MovingSphere::factory( center, center2, 0.0, 1.0, 0.2, mat )
                    );


                }else if( choose_mat < 0.9 ){
                    // metal
                    glm::dvec3 albedo = get_random_color( 0.5, 1.0 );
                    double fuzz = tprMath::get_random_double( 0.0, 0.5 );
                    IMaterial *mat = create_mat( std::make_unique<Metal>( albedo, fuzz ) );
                    worldObjs.add(
                        Sphere::factory( center, 0.2, mat )
                    );

                }else{
                    // glass
                    double refractIndex = tprMath::get_random_double( 1.5, 2.4 );
                    IMaterial *mat = create_mat( std::make_unique<Dielectric>( refractIndex ) );
                    worldObjs.add(
                        Sphere::factory( center, 0.2, mat )
                    );

                }
            }
        }
    }

    //--- oth mats ---//
    IMaterial *mat_lambt_1 = create_mat( std::make_unique<Lambertian>( glm::dvec3{ 0.4, 0.2, 0.1 } ) );

    IMaterial *mat_metal_1 = create_mat( std::make_unique<Metal>( glm::dvec3{ 0.7, 0.6, 0.5 }, 0.0 ) );

    IMaterial *mat_diel_glass = create_mat( std::make_unique<Dielectric>( 1.5 ) );
    IMaterial *mat_diel_diamond = create_mat( std::make_unique<Dielectric>( 2.4 ) );


    //----- objs -----//
    // center
    worldObjs.add(
        Sphere::factory( glm::dvec3{ 0.0, 1.0, 0.0 }, 1.0, mat_diel_glass )
    );
    // left
    worldObjs.add(
        Sphere::factory( glm::dvec3{-4.0, 1.0, 0.0 }, 1.0, mat_lambt_1 )
    );
    // right
    worldObjs.add(
        Sphere::factory( glm::dvec3{ 4.0, 1.0, 0.0 }, 1.0, mat_metal_1 )
    );


    // 非常神奇的部分...
    bvhPtr = BVH_Node::factory( worldObjs, 0.0, 1.0 );

    bvhPtr->print();

    worldObjs2.add( bvhPtr );

}









