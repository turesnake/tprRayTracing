/*
 * ========================= global.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *   全局变量
 *   使用 cpp 特有的 inline 关键词来 一步到位地 声明+初始化
 * ----------------------------
 */
#ifndef TPR_GLOBAL_H
#define TPR_GLOBAL_H

//-------------------- CPP --------------------//
#include <string>

//----------------------------------------------------//
//                     file
//----------------------------------------------------//
extern int         fd_cwd;   //-- 项目 主目录 fd

extern std::string path_cwd; //-- exe 所在目录的 path
                
extern std::string path_shaders; //-- .../build/publish/shaders/
extern std::string path_textures; //-- .../build/publish/textures/ // 暂未被使用 ...
extern std::string path_jsons; //-- .../build/publish/jsons/
extern std::string path_tprLog; //-- .../build/publish/tprLog/
extern std::string path_output; //-- .../build/publish/output/


//----------------------------------------------------//
//                       OS
//----------------------------------------------------//
//-- 当前身处什么 操作系统 --//
#define OS_NULL  0
#define OS_APPLE 1
#define OS_UNIX  2
#define OS_WIN32 3
extern int current_OS;


#endif
