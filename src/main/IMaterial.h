/*
 * ======================= IMaterial.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_I_MATERIAL_H
#define TPR_I_MATERIAL_H

#include "pch.h"

class Ray;
class HitRecord;



class IMaterial{
public:
    IMaterial()=default;
    virtual ~IMaterial()=default;//MUST

    virtual bool scatter(   const Ray       &rIn_, 
                            const HitRecord &hitRec_,
                            glm::dvec3      &attenuation_, // out
                            Ray             &rScattered_ // out
                        )const=0;
};







#endif 
