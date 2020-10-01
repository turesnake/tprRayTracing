/*
 * ========================= main.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2018.11.21
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"

//-------------------- Main --------------------//
#include "SysConfig.h" // 此文件由 cmake 自己生成，并不在 src/ 目录中


int main( int argc, char* argv[] ){

    
    //===== debug::log 示例：
    debug::log(
        "tpr ray tracing"
    );



    //======= 检查当前所处的 操作系统：
    #if defined TPR_OS_MACOSX_
        debug::log( "\n___OS_MACOSX___\n\n" );
    #elif defined TPR_OS_LINUX_
        debug::log( "\n___OS_LINUX___\n\n" );
    #elif defined TPR_OS_WIN32_
        debug::log( "\n___OS_WIN32___\n\n" );
    #endif 



    return(0);
}


