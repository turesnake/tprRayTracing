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

    void add( std::shared_ptr<IHittable> sptr_ ){
        this->objSPtrs.push_back( sptr_ );
    }

    std::vector<std::shared_ptr<IHittable>> &get_objSPtrs(){
        return this->objSPtrs;
    }


    // check all objs in random order
    // reduce the closest_tmax to find the closest hit
    bool hit( const Ray &r_, double tmin_, double tmax_, HitRecord &record_ )const override{

        HitRecord tmpHRet  {};
        bool      isHit {false};
        double    closest_tmax {tmax_};// 步步缩小

        for( auto objSPtr : this->objSPtrs ){
            if( objSPtr->hit( r_, tmin_, closest_tmax, tmpHRet ) ){
                isHit = true;
                closest_tmax = tmpHRet.t;
                record_ = tmpHRet; // copy
            }
        }
        return isHit;
    }

    // bound all objs to a aabb box
    bool bounding_box( double t0_, double t1_, AABB &output_aabb_ )const override{
        if( this->objSPtrs.empty() ){ return false; }

        AABB tmp_aabb {};
        bool isFstAABB {true};

        for( auto objSPtr : this->objSPtrs ){
            if( !objSPtr->bounding_box( t0_,t1_,tmp_aabb ) ){
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

    std::vector<std::shared_ptr<IHittable>> objSPtrs {};

};




#endif 
