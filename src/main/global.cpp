/*
 * ======================== global.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "pch.h"
#include "global.h"


int         fd_cwd {};   //-- 项目 主目录 fd

std::string path_cwd {}; //-- exe 所在目录的 path
                
std::string path_shaders  {}; //-- .../build/publish/shaders/
std::string path_textures {}; //-- .../build/publish/textures/
std::string path_jsons    {}; //-- .../build/publish/jsons/
std::string path_tprLog   {}; //-- .../build/publish/tprLog/
std::string path_output   {}; //-- .../build/publish/output/



int current_OS {};

