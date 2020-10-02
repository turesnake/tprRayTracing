/*
 * ====================== write_2_png.h ====================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.01
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_LOAD_AND_DIVIDE_PNG_H
#define TPR_LOAD_AND_DIVIDE_PNG_H

#include "pch.h"


//------------------- Engine --------------------//
#include "RGBA.h"

void write_2_png(   const std::string &path_,
                    int w,
                    int h,
                    const std::vector<RGBA> &data_,
                    bool isFlip
                );


#endif 
