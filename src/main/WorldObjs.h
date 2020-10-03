/*
 * ======================= WorldObjs.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  all objs in world ( only spheres now... )
 * ----------------------------
 */
#ifndef TPR_WORLD_OBJS_H
#define TPR_WORLD_OBJS_H

#include "pch.h"

#include "IHittable.h"
#include "HitRecord.h"
#include "RGBA.h"


class WorldObjs{
public:
    explicit WorldObjs()=default;

    void add( IHittable *objPtr_ ){
        this->objPtrs.push_back( objPtr_ );
    }

    // check all objs in random order
    // reduce the closest_tmax to find the closest hit
    std::optional<HitRecord> hit( const Ray &r_, double tmin_, double tmax_ ){

        HitRecord hRet  {};
        HitRecord tmpHRet  {};
        bool      isHit {false};
        double    closest_tmax {tmax_};// 步步缩小

        for( auto objPtr : this->objPtrs ){
            if( objPtr->hit( r_, tmin_, closest_tmax, tmpHRet ) ){
                isHit = true;
                closest_tmax = tmpHRet.t;
                hRet = tmpHRet; // copy
            }
        }

        return isHit ?
            std::optional<HitRecord>{ hRet } :
            std::nullopt;
    }


private:

    std::vector<IHittable*> objPtrs {};


};




#endif 
