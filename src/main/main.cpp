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

#include "Texture.h"

#include "BVH.h"

#include <iostream>



std::string get_current_time_str();
glm::dvec3 calc_ray_color( const Ray &r_, int boundN_ );




//--- all spheres in screne ---

std::shared_ptr<BVH_Node> bvhSPtr {nullptr}; 



void create_scene_1();
void create_scene_2();
void create_scene_3();
void create_scene_4();
void create_scene_5();


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
    for( int i=0; i<7; i++ ){
        //double d = tprMath::get_random_double();
    }
    
    

    //==========================================//
    //             mats, objs
    //------------------------------------------//

    //----- camera -----//
    glm::dvec3 lookfrom {};
    glm::dvec3 lookat   {};
    glm::dvec3 worldup  { 0.0, 1.0, 0.0 };
    //double dist_to_focus = glm::length( lookat - lookfrom );
    double vfov {};
    double dist_to_focus {};
    double aperture {};


    switch( 5 ){

        case 3:
            // last scne in book 1
            create_scene_3();
            lookfrom = glm::dvec3{ 13.0, 2.0, 3.0 };
            lookat = glm::dvec3{ 0.0, 0.0, 0.0 };
            vfov = 20.0;
            dist_to_focus = 10.0;
            aperture = 0.1;
            break;
        
        case 4:
            // two big spheres with checker-texture
            create_scene_4();
            lookfrom = glm::dvec3{ 13.0, 2.0, 3.0 };
            lookat = glm::dvec3{ 0.0, 0.0, 0.0 };
            vfov = 20.0;
            dist_to_focus = 10.0;
            aperture = 0.1;
            break;

        case 5:
            // two perlin spheres (big + sml)
            create_scene_5();
            lookfrom = glm::dvec3{ 13.0, 2.0, 3.0 };
            lookat = glm::dvec3{ 0.0, 0.0, 0.0 };
            vfov = 20.0;
            dist_to_focus = glm::length( lookat - lookfrom );
            aperture = 0.02;
            break;
        
        default:
            break;
    }



    Camera camera { lookfrom,
                    lookat,
                    worldup,
                    vfov, 
                    ASPECT_RATIO<>,
                    aperture,
                    dist_to_focus,
                    0.0,
                    1.0
                };


    //==========================================//
    //              rendering
    //------------------------------------------//
    std::vector<RGBA> pngData ( IMAGE_W<> * IMAGE_H<>, RGBA{ 255, 0, 0, 255 } );

    debug::log("Start Rendering:");
    clock_t time1 = clock();

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

    clock_t time2 = clock();
    double timeoff = static_cast<double>(time2-time1) / 
                     static_cast<double>(CLOCKS_PER_SEC);
    debug::log( "\ntime chekc:\n   t1: {}, t2: {}; timeoff:{}", time1, time2, timeoff  );


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

    if( bvhSPtr->hit( r_, 0.001, tprMath::infinity, hret ) ){

        Ray rScattered {};
        glm::dvec3 attenuation {};

        if( hret.matSPtr->scatter(r_, hret, attenuation, rScattered) ){
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



void create_scene_1(){

    HittableList scene {};

    //----- mats -----//
    auto mat_lambt_ground = std::make_shared<Lambertian>( glm::dvec3{ 0.8, 0.8, 0.0 } );
    auto mat_lambt_egg    = std::make_shared<Lambertian>( glm::dvec3{ 0.7, 0.3, 0.3 } );
    auto mat_lambt_midblue = std::make_shared<Lambertian>( glm::dvec3{ 0.1, 0.2, 0.5 } );

    auto mat_metal_silver = std::make_shared<Metal>( glm::dvec3{ 0.8, 0.8, 0.8 }, 0.0 );
    auto mat_metal_gold_fuzz = std::make_shared<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 1.0 );
    auto mat_metal_gold = std::make_shared<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 0.0 );

    auto mat_diel_glass = std::make_shared<Dielectric>( 1.5 );
    auto mat_diel_diamond = std::make_shared<Dielectric>( 2.4 );


    //----- objs -----//
    // ground sphere
    scene.add( std::make_shared<Sphere>( glm::dvec3{ 0.0, -100.5, -1.0 }, 100.0, mat_lambt_ground ) );

    // center
    scene.add( std::make_shared<Sphere>( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.5, mat_metal_silver ));

    // left
    scene.add( std::make_shared<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, 0.5, mat_diel_diamond ));
    scene.add( std::make_shared<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, -0.45, mat_diel_diamond ));
    scene.add( std::make_shared<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, 0.3, mat_metal_gold ));
    
    // right
    scene.add( std::make_shared<Sphere>( glm::dvec3{ 1.0, 0.0, -1.0 }, 0.5, mat_metal_gold ));

    //-----//
    bvhSPtr = std::make_shared<BVH_Node>( scene, 0.0, 1.0 );
}



void create_scene_2(){

    HittableList scene {};

    //----- mats -----//
    auto mat_lambt_ground = std::make_shared<Lambertian>( glm::dvec3{ 0.8, 0.8, 0.0 } );
    auto mat_lambt_egg    = std::make_shared<Lambertian>( glm::dvec3{ 0.7, 0.3, 0.3 } );
    auto mat_lambt_midblue = std::make_shared<Lambertian>( glm::dvec3{ 0.1, 0.2, 0.5 } );

    auto mat_metal_silver = std::make_shared<Metal>( glm::dvec3{ 0.8, 0.8, 0.8 }, 0.0 );
    auto mat_metal_gold_fuzz = std::make_shared<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 1.0 );
    auto mat_metal_gold = std::make_shared<Metal>( glm::dvec3{ 0.8, 0.6, 0.2 }, 0.0 );

    auto mat_diel_glass = std::make_shared<Dielectric>( 1.5 );
    auto mat_diel_diamond = std::make_shared<Dielectric>( 2.4 );


    //----- objs -----//
    // ground sphere
    scene.add( std::make_shared<Sphere>( glm::dvec3{ 0.0, -100.5, -1.0 }, 100.0, mat_lambt_ground ));

    // center
    scene.add( std::make_shared<Sphere>( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.5, mat_diel_diamond ));
    scene.add( std::make_shared<Sphere>( glm::dvec3{ 0.0, 0.0, -1.0 }, -0.45, mat_diel_diamond ));
    scene.add( std::make_shared<Sphere>( glm::dvec3{ 0.0, 0.0, -1.0 }, 0.3, mat_metal_gold ));

    // left
    scene.add( std::make_shared<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, 0.5, mat_diel_glass ));
    scene.add( std::make_shared<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, -0.45, mat_diel_glass ));
    scene.add( std::make_shared<Sphere>( glm::dvec3{-1.0, 0.0, -1.0 }, 0.35, mat_lambt_midblue ));

    // right
    scene.add( std::make_shared<Sphere>( glm::dvec3{ 1.0, 0.0, -1.0 }, 0.5, mat_metal_silver ));

    //-----//
    bvhSPtr = std::make_shared<BVH_Node>( scene, 0.0, 1.0 );
}


// last scene in book 《Ray Tracing in One Weekend》
void create_scene_3(){

    HittableList scene {};

    // lambdas
    auto get_random_color = []( double min_=0.0, double max_=1.0 )->glm::dvec3{
        return glm::dvec3{
            tprMath::get_random_double( min_, max_ ),
            tprMath::get_random_double( min_, max_ ),
            tprMath::get_random_double( min_, max_ )
        };
    };

    //----- ground -----//
    auto mat_lambt_ground = std::make_shared<Lambertian>( glm::dvec3{ 0.5, 0.5, 0.5 } );

    auto mat_lambt_checker_ground = std::make_shared<Lambertian>( 
        std::make_shared<CheckerTexture>(  
            glm::dvec3{ 0.2, 0.3, 0.1 },
            glm::dvec3{ 0.9, 0.9, 0.9 }
        )
    );

    scene.add( std::make_shared<Sphere>(  
        glm::dvec3{ 0.0, -1000.0, 0.0 }, 1000.0, mat_lambt_checker_ground
    ));

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
                    auto mat = std::make_shared<Lambertian>( albedo );
                    
                    glm::dvec3 center2 = center + glm::dvec3{
                        0.0, 
                        tprMath::get_random_double( 0.0, 0.5 ),
                        0.0
                    };
                    scene.add( std::make_shared<MovingSphere>( 
                        center, center2, 0.0, 1.0, 0.2, mat
                    ));

                }else if( choose_mat < 0.9 ){
                    // metal
                    glm::dvec3 albedo = get_random_color( 0.5, 1.0 );
                    double fuzz = tprMath::get_random_double( 0.0, 0.5 );

                    auto mat = std::make_shared<Metal>( albedo, fuzz );
                    scene.add( std::make_shared<Sphere>( 
                        center, 0.2, mat 
                    ));

                }else{
                    // glass
                    double refractIndex = tprMath::get_random_double( 1.5, 2.4 );

                    auto mat = std::make_shared<Dielectric>( refractIndex );

                    scene.add( std::make_shared<Sphere>( 
                        center, 0.2, mat
                    ));
                }
            }
        }
    }

    //--- oth mats ---//
    auto mat_lambt_1 = std::make_shared<Lambertian>( glm::dvec3{ 0.4, 0.2, 0.1 } );

    auto mat_metal_1 = std::make_shared<Metal>( glm::dvec3{ 0.7, 0.6, 0.5 }, 0.0 );

    auto mat_diel_glass = std::make_shared<Dielectric>( 1.5 );
    auto mat_diel_diamond = std::make_shared<Dielectric>( 2.4 );

    //----- objs -----//
    // center
    scene.add( std::make_shared<Sphere>( 
        glm::dvec3{ 0.0, 1.0, 0.0 }, 1.0, mat_diel_glass
    ));
    // left
    scene.add( std::make_shared<Sphere>( 
        glm::dvec3{-4.0, 1.0, 0.0 }, 1.0, mat_lambt_1
    ));
    // right
    scene.add( std::make_shared<Sphere>( 
        glm::dvec3{ 4.0, 1.0, 0.0 }, 1.0, mat_metal_1
    ));

    //-----//
    bvhSPtr = std::make_shared<BVH_Node>( scene, 0.0, 1.0 );
}



// two big spheres with checker-texture
void create_scene_4(){

    HittableList scene {};

    auto checker_tex = std::make_shared<CheckerTexture>(  
        glm::dvec3{ 0.2, 0.3, 0.1 },
        glm::dvec3{ 0.9, 0.9, 0.9 }
    );
    auto mat_checker = std::make_shared<Lambertian>( checker_tex );

    scene.add( std::make_shared<Sphere>( 
        glm::dvec3{ 0.0, -10.0, 0.0 }, 10.0, mat_checker
    ));
    scene.add( std::make_shared<Sphere>( 
        glm::dvec3{ 0.0,  10.0, 0.0 }, 10.0, mat_checker
    ));

    //-----//
    bvhSPtr = std::make_shared<BVH_Node>( scene, 0.0, 1.0 );
}




// two perlin sphere (big + sml)
void create_scene_5(){

    HittableList scene {};

    auto perlinTex = std::make_shared<NoiseTexture>( 4.0 );
    auto mat_perlin = std::make_shared<Lambertian>( perlinTex );

    scene.add( std::make_shared<Sphere>( 
        glm::dvec3{ 0.0, -1000.0, 0.0 }, 1000.0, mat_perlin
    ));
    scene.add( std::make_shared<Sphere>( 
        glm::dvec3{ 0.0,  2.0, 0.0 }, 2.0, mat_perlin
    ));

    //-----//
    bvhSPtr = std::make_shared<BVH_Node>( scene, 0.0, 1.0 );
}





