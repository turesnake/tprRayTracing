/*
 * ========================= config.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.02
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  config Vals
 */
#ifndef TPR_CONFIG_H
#define TPR_CONFIG_H


template<typename T = double> constexpr T ASPECT_RATIO { 16.0 / 9.0 };

// image size [pix]
template<typename T = int> constexpr T IMAGE_H { 900 };
template<typename T = int> constexpr T IMAGE_W { 
    static_cast<int>(IMAGE_H<double> * ASPECT_RATIO<>) 
};


template<typename T = double> constexpr T SAMPLES_PER_PIX { 40 };// spp

template<typename T = int> constexpr T BOUNDS_NUM { 17 };




#endif
