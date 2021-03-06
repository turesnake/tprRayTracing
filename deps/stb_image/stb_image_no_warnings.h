/*
 * =================== stb_image_no_warnings.h =================
 *                          -- tpr --
 *                                        CREATE -- 2019.06.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_STB_IMAGE_NO_WARNINGS_H_
#define TPR_STB_IMAGE_NO_WARNINGS_H_

//-- 屏蔽掉 stb_image 中的所有 warnings --
#pragma clang system_header

 //-- 加载图片数据用
#ifndef STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image_inn/stb_image.h"

//-- 将数据写到文件
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
    #define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include "stb_image_inn/stb_image_write.h"


#endif
