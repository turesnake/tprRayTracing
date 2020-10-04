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

class Ray;
class HitRecord;


class IHittable{
public:
    IHittable()=default;
    virtual ~IHittable()=default;// MUST
    virtual bool hit( const Ray &r_, double tmin_, double tmax_, HitRecord &record_ )const=0;
};



#endif 
