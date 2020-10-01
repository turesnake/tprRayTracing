/*
 * ========================= main.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- Main --------------------//
#include "SysConfig.h" // 此文件由 cmake 自己生成，并不在 src/ 目录中
#include "prepare.h"

#include "write_2_png.h"

#include "global.h"


//tmp
#include "tprGeneral.h"


int main( int argc, char* argv[] ){
    debug::log( "\n---------------------- BEGIN ----------------------\n\n" );
    //==========================================//
    //                 prepare
    //------------------------------------------//
    prepare( argv[0] );

    
    //===== test
    debug::log( "\ntpr ray tracing" );


    //==========================================//
    //               tmp works
    //------------------------------------------//

    std::string pngPath = tprGeneral::path_combine( path_output, "akokoa.png" );

    int W = 500;
    int H = 500;

    std::vector<RGBA> pngData ( W*H, RGBA{ 180, 210, 200, 255 } );







    write_2_png(pngPath,
                W,
                H,
                pngData
                );





    debug::log( "\n\n---------------------- END ----------------------\n" );
    return(0);
}


