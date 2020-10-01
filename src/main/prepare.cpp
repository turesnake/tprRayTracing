/*
 * ========================= prepare.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "prepare.h"

//---------------- from cmake ------------------//
#include "SysConfig.h" // MUST BEFORE TPR_OS_XXX macros !!!

//-------------------- C ----------------------//
#if defined TPR_OS_WIN32_
	#include <windows.h>
#elif defined TPR_OS_UNIX_
    #include <limits.h>  //- PATH_MAX
    #include <stdlib.h>  //- realpath
	#include <unistd.h>  //- fchdir
	#include <fcntl.h>    //-- open，openat, AT_FDCWD
#endif

//------------------- Libs --------------------//
#include "tprGeneral.h"
#include "magic_enum.hpp"


#if defined TPR_OS_WIN32_
    #include "tprFileSys_win.h" 
#elif defined TPR_OS_UNIX_
    #include "wrapUnix.h"
    #include "tprFileModeT.h"
    #include "tprFileSys_unix.h" 
#endif

//-------------------- Engine --------------------//
#include "global.h"


//----- type detection -----//
static_assert( sizeof(short)==2, "sizeof(short)==2" );
static_assert( sizeof(unsigned short)==2, "sizeof(unsigned short)==2" );
static_assert( sizeof(int)==4,   "sizeof(int)==4" );
static_assert( sizeof(unsigned int)==4,   "sizeof(unsigned int)==4" );
static_assert( sizeof(long long)==8,   "sizeof(long long)==8" );
static_assert( sizeof(unsigned long long)==8,   "sizeof(unsigned long long)==8" );
static_assert( sizeof(float)==4,   "sizeof(float)==4" );
static_assert( sizeof(double)==8,   "sizeof(double)==8" );
//static_assert( sizeof(off_t)==8,   "sizeof(off_t)==8" ); //- in unix is 8, in win is 4


static_assert(magic_enum::is_magic_enum_supported, "magic_enum: Unsupported compiler.");



namespace prepare_inn {//------------ namespace: prepare_inn ------------//
    void build_path_cwd( char *exeDirPath_ );
    void check_OS();
    void check_and_creat_important_dir();
}//------------ namespace: prepare_inn end ------------//


/*===========================================================
 *                      prepare  
 *-----------------------------------------------------------
 */
void prepare( char *exeDirPath_ ){

    //------------------------//
    //   查看当前所在的 操作系统
    //------------------------//
    prepare_inn::check_OS();

    //------------------------//
    //    初始化 path_cwd
    //------------------------//
    prepare_inn::build_path_cwd( exeDirPath_ );

    //------------------------//
    // 检测 关键 目录 的存在，若没有，创建之
    //------------------------//
    prepare_inn::check_and_creat_important_dir();

    return;
}

namespace prepare_inn {//------------ namespace: prepare_inn ------------//


/* ===========================================================
 *                  build_path_cwd   
 * -----------------------------------------------------------
 */
void build_path_cwd( char *exeDirPath_ ){

#if defined TPR_OS_WIN32_

	char buf[MAX_PATH];
	GetModuleFileName( nullptr, buf, MAX_PATH ); //- exe文件path
	// 当前 buf数据 为 ".../xx.exe"
	// 需要将 最后一段 截掉
	std::string::size_type pos = std::string(buf).find_last_of( "\\/" );
	path_cwd = std::string(buf).substr( 0, pos );

#elif defined TPR_OS_UNIX_

    char ubuf[ PATH_MAX + 1 ];
    char *res = realpath( exeDirPath_, ubuf);
    if (!res) {
        debug::log( "realpath ERROR; exeDirPath_ = {}", exeDirPath_ );
        tprAssert(0);
    }
    //- ubuf 暂为 .../xxx.exe 的 path，需要截去最后一段 
	std::string::size_type pos = std::string(ubuf).find_last_of( "/" );
	path_cwd = std::string(ubuf).substr( 0, pos );
    
#endif

}


/*===========================================================
 *           check_and_creat_important_dir    
 *-----------------------------------------------------------
 */
void check_and_creat_important_dir(){

    std::string err_info = "check_and_creat_important_dir(): ";

    //----------------------------//
    debug::log( "path_cwd = {}", path_cwd );

#if defined TPR_OS_WIN32_

    
    //---------------------------------//
    //           path_shaders
    //---------------------------------//
    path_shaders = tprWin::mk_dir( path_cwd,
                                "shaders/",
                                err_info );
    //---------------------------------//
    //           path_textures
    //---------------------------------//
    path_textures = tprWin::mk_dir( path_cwd,
                                "textures/",
                                err_info );
    //---------------------------------//
    //           path_jsons
    //---------------------------------//
    path_jsons = tprWin::mk_dir( path_cwd,
                                "jsons/",
                                err_info );
    //---------------------------------//
    //           path_tprLog
    //---------------------------------//
    path_tprLog = tprWin::mk_dir( path_cwd,
                                "tprLog/",
                                err_info );
    //---------------------------------//
    //           path_output
    //---------------------------------//
    path_output = tprWin::mk_dir( path_cwd,
                                "output/",
                                err_info );
    
    

#elif defined TPR_OS_UNIX_

    
    //---------------------------------//
    //           path_shaders
    //---------------------------------//
    path_shaders = tprUnix::mk_dir( path_cwd,
                        "shaders/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_textures
    //---------------------------------//
    path_textures = tprUnix::mk_dir( path_cwd,
                        "textures/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_jsons
    //---------------------------------//
    path_jsons = tprUnix::mk_dir( path_cwd,
                        "jsons/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_tprLog
    //---------------------------------//
    path_tprLog = tprUnix::mk_dir( path_cwd,
                        "tprLog/",
                        RWXR_XR_X,
                        err_info );
    //---------------------------------//
    //           path_output
    //---------------------------------//
    path_output = tprUnix::mk_dir( path_cwd,
                        "output/",
                        RWXR_XR_X,
                        err_info );
    
    

#endif
}



/* ===========================================================
 *                     check_OS
 * -----------------------------------------------------------
 * -- 检测 当前 程序 身处什么 操作系统 
 */
void check_OS(){

    debug::log( "{}", SYSTEM_NAME );

    current_OS = OS_NULL; //-- 先设置为 未找到状态

    #if defined TPR_OS_MACOSX_
        current_OS = OS_APPLE;
        debug::log( "TPR_OS_MACOSX_" );

    #elif defined TPR_OS_LINUX_
        current_OS = OS_UNIX;
        debug::log( "TPR_OS_LINUX_" );

    #elif defined TPR_OS_WIN32_
        current_OS = OS_WIN32;
        debug::log( "TPR_OS_WIN32_" );
    #endif 
    
}



}//------------- namespace: prepare_inn ----------------//

