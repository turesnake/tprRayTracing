/*
 * ====================== write_2_png.cpp ====================
 *                          -- tpr --
 *                                        CREATE -- 2010.10.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *    加载 解析 “图元帧式的png文件” 
 * ----------------------------
 */
#include "pch.h"
#include "stb_image_no_warnings.h"

//------------------- CPP --------------------//
#include <iterator>

//------------------- Engine --------------------//
#include "write_2_png.h"
#include "tprAssert.h"





void write_2_png(   const std::string &path_,
                    int w,
                    int h,
                    const std::vector<RGBA> &data_
                    ){ 

    tprAssert( !data_.empty() );
    tprAssert( w*h == static_cast<int>(data_.size()) );

    stbi_write_png( path_.c_str(), 
                    w, 
                    h, 
                    4, // nrChannels 
                    static_cast<const void*>(&data_[0]), 
                    4  // stride_in_bytes
                    );
    
}










