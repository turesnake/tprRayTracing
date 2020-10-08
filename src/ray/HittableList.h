/*
 * ======================= HittableList.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.03
 *                                        MODIFY --
 * ----------------------------------------------------------
 *  all objs in world ( only spheres now... )
 * ----------------------------
 */
#ifndef TPR_HITTABLE_LIST_H
#define TPR_HITTABLE_LIST_H

#include "pch.h"

#include "IHittable.h"
#include "HitRecord.h"
#include "RGBA.h"




class HittableList : public IHittable{
public:
    HittableList()=default;

    void add( IHittable *objPtr_ ){
        this->objPtrs.push_back( objPtr_ );
    }

    std::vector<IHittable*> &get_objPtrs(){
        return this->objPtrs;
    }


    // check all objs in random order
    // reduce the closest_tmax to find the closest hit
    bool hit( const Ray &r_, double tmin_, double tmax_, HitRecord &record_ )const override{

        HitRecord tmpHRet  {};
        bool      isHit {false};
        double    closest_tmax {tmax_};// 步步缩小

        for( auto objPtr : this->objPtrs ){
            if( objPtr->hit( r_, tmin_, closest_tmax, tmpHRet ) ){
                isHit = true;
                closest_tmax = tmpHRet.t;
                record_ = tmpHRet; // copy
            }
        }
        return isHit;
    }

    // bound all objs to a aabb box
    bool bounding_box( double t0_, double t1_, AABB &output_aabb_ )const override{
        if( this->objPtrs.empty() ){ return false; }

        AABB tmp_aabb {};
        bool isFstAABB {true};

        for( auto objPtr : this->objPtrs ){
            if( !objPtr->bounding_box( t0_,t1_,tmp_aabb ) ){
                return false;
            }
            if( isFstAABB ){
                isFstAABB = false;
                output_aabb_ = tmp_aabb;
            }else{
                output_aabb_ = AABB::surrounding_box( output_aabb_, tmp_aabb );
            }
        }
        return false;
    }

private:

    std::vector<IHittable*> objPtrs {};

};




#endif 
