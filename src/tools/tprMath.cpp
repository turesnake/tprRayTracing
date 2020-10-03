/*
 * ====================== tprMath.cpp =======================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   
 */
#include "pch.h"
#include "tprMath.h"

namespace tprMath {//-------- namespace: tprMath --------------//


namespace tprMath_inn {//----------- namespace: tprMath_inn ----------------//


    std::uniform_real_distribution<double> uDist_double_01( 0.0, 1.0 );

    std::mt19937 generator {}; //高性能 伪随机数 生成器


}//-------------- namespace: tprMath_inn end ----------------//



double get_random_double(){
    return tprMath_inn::uDist_double_01( tprMath_inn::generator );
}

double get_random_double( double min_, double max_ ){
    std::uniform_real_distribution<double> uDist( min_, max_ );
    return uDist( tprMath_inn::generator );
}



}//------------- namespace: tprMath end --------------//

