/*
 * ========================= AABB.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.07
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  Axis-Aligned Bounding Boxes
 * ------------------------------
 */
#ifndef TPR_AABB_H
#define TPR_AABB_H

#include "pch.h"

#include "Ray.h"

#include <algorithm>


class AABB{
public:
    AABB()=default;
    AABB(   const glm::dvec3 minVal_,
            const glm::dvec3 maxVal_ 
        ):
        minVal(minVal_),
        maxVal(maxVal_)
        {}

    const glm::dvec3 get_min()const{ return this->minVal; }
    const glm::dvec3 get_max()const{ return this->maxVal; }

    void print()const{
        debug::log("   min:{},{},{}; max{},{},P{}",
            minVal.x, minVal.y, minVal.z,
            maxVal.x, maxVal.y, maxVal.z
        );
    }

    // just check if ray hit the AABB
    // 书本记录的第一版方法
    /*
    bool hitBox( const Ray &r_, double tmin_, double tmax_ ){

        const glm::dvec3 &r_Ori = r_.get_origin();
        const glm::dvec3 &r_Dir = r_.get_dir();

        // x,y,z
        for( int i=0; i<3; i++ ){

            double ta = (minVal[i] - r_Ori[i]) / r_Dir[i];
            double tb = (maxVal[i] - r_Ori[i]) / r_Dir[i];
                // if dividend is 0.0, the result is infinity 
                // if both dividend and divisor are 0.0, the result is NaN

            double t0 = std::min<double>( ta, tb );
            double t1 = std::max<double>( ta, tb );
                // 防止 r_ 是朝着 xyz 某个轴的反防线的

            double tmin = std::max<double>( t0, tmin_ );
            double tmax = std::min<double>( t1, tmax_ );
                // 只有在两个比较值皆为 nan，返回值才可能是 nan
                // 鉴于 参数 tmin_，tmax_ 一定不为 nan
                // 所以 tmin,tmax 绝对不可能为 nan
                tprAssert( !std::isnan(tmin) );
                tprAssert( !std::isnan(tmax) );

            if( tmax <= tmin ){ return false; }
                // if any or both al of {tmin,tmax} is NaN
                // the compare will return false
                // -----
                // 尽管使用 <= 来比较两个 浮点数 是不精确的，
                // 但也千万不能使用 !(tmax>tmin) 来代替之
                // 只要 {tmin,tmax} 中存在一个以上 nan
                // 那么不管是 <=, 还是 >, 都会返回 false
                // 但 !() 却会将结果取反，获得 true
                // 这显然不是我们希望得到的
        }
        return true;
    }
    */


    // just check if ray hit the AABB
    // 书本推荐的方法
    bool hitBox( const Ray &r_, double tmin_, double tmax_ )const{

        const glm::dvec3 r_Ori = r_.get_origin();
        const glm::dvec3 r_Dir = r_.get_dir();

        // x,y,z
        for( int i=0; i<3; i++ ){

            double invD = 1.0 / r_Dir[i];

            double t0 = (minVal[i] - r_Ori[i]) * invD;
            double t1 = (maxVal[i] - r_Ori[i]) * invD;
                // if dividend is 0.0, the result is infinity 
                // if both dividend and divisor are 0.0, the result is NaN

            // reversed
            if( invD < 0.0 ){
                std::swap( t0, t1 );
            }

            double tmin = t0 > tmin_ ? t0 : tmin_;
            double tmax = t1 < tmax_ ? t1 : tmax_;
                // 参数 {tmin_,tmax_} 绝不可能是 nan
                // 此时，若 {t0,t1} 为nan，比较运算一定返回 false，
                // 三目运算将正好选择 参数 {tmin_,tmax_}
                // 从而保证，{tmin,tmax} 既不是 inf，也不是 nan

                tprAssert( !std::isinf(tmin) );
                tprAssert( !std::isinf(tmax) );
                tprAssert( !std::isnan(tmin) );
                tprAssert( !std::isnan(tmax) );

            if( tmax <= tmin ){ return false; }
        }
        return true;
    }

    static AABB surrounding_box( const AABB &b0_, const AABB &b1_ ){

        const glm::dvec3 min0 = b0_.get_min();
        const glm::dvec3 max0 = b0_.get_max();
        const glm::dvec3 min1 = b1_.get_min();
        const glm::dvec3 max1 = b1_.get_max();

        glm::dvec3 sml {
            std::min<double>( min0.x, min1.x ),
            std::min<double>( min0.y, min1.y ),
            std::min<double>( min0.z, min1.z )
        };
        glm::dvec3 big {
            std::max<double>( max0.x, max1.x ),
            std::max<double>( max0.y, max1.y ),
            std::max<double>( max0.z, max1.z )
        };
        return AABB{ sml, big };
    }

private:
    glm::dvec3 minVal {};
    glm::dvec3 maxVal {};
};


#endif 
