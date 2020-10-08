/*
 * ======================= IHittable.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_I_HIT_ABLE_H
#define TPR_I_HIT_ABLE_H

#include "pch.h"
#include "AABB.h"

class Ray;
class HitRecord;



class IHittable{
public:
    IHittable()=default;
    virtual ~IHittable()=default;// MUST
    virtual bool hit(   const Ray &r_, 
                        double tmin_, 
                        double tmax_, 
                        HitRecord &record_ 
                    )const=0;
    virtual bool bounding_box(  double t0_,
                                double t1_,
                                AABB   &output_aabb_
                    )const=0;
};


// compare AABB.min
// axis_ {0,1,2} <--> {x,y,z}
inline bool aabb_compare( IHittable *a_, IHittable *b_, int axis_ ){

    AABB boxA {};
    AABB boxB {};
    bool retA = a_->bounding_box( 0.0, 0.0, boxA );
    bool retB = b_->bounding_box( 0.0, 0.0, boxB );
        tprAssert( retA && retB );
    return boxA.get_min()[axis_] < boxB.get_min()[axis_];
}


inline bool aabb_x_compare( IHittable *a_, IHittable *b_ ){
    return aabb_compare( a_, b_, 0 );
}
inline bool aabb_y_compare( IHittable *a_, IHittable *b_ ){
    return aabb_compare( a_, b_, 1 );
}
inline bool aabb_z_compare( IHittable *a_, IHittable *b_ ){
    return aabb_compare( a_, b_, 2 );
}



#endif 
