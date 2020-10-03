/*
 * ======================== tprMath.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_MATH_H
#define TPR_MATH_H
//--- glm - 0.9.9.5 ---
#include "glm_no_warnings.h"

#include <cmath>
#include <limits>
#include <random>

#include "tprAssert.h"


//-------------------- CPP --------------------//


namespace tprMath {//-------- namespace: tprMath --------------//


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


inline glm::dvec3 lerp( const glm::dvec3 &a_, const glm::dvec3 &b_, double t_ ){
    tprAssert( t_>=0.0 && t_<=1.0 );
    return (1.0-t_) * a_ + t_*b_;
}


double get_random_double();
double get_random_double( double min_, double max_ );



}//------------- namespace: tprMath end --------------//
#endif 
