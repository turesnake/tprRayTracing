/*
 * ========================= BVH.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  Bounding Volume Hierarchies
 * ------------------------------
 */
#ifndef TPR_BVH_H
#define TPR_BVH_H

#include "pch.h"

#include "Ray.h"
#include "IHittable.h"
#include "HittableList.h"


class BVH_Node : public IHittable{
public:

    BVH_Node()=default;
    
    BVH_Node( HittableList &hlist_, double time0_, double time1_ ):
        BVH_Node(   hlist_.get_objSPtrs(),
                    0,
                    hlist_.get_objSPtrs().size(),
                    time0_,
                    time1_,
                    0
                )
                {}

    BVH_Node(   std::vector<std::shared_ptr<IHittable>> &objs_,
                size_t begin_,
                size_t end_, // not include
                double time0_, 
                double time1_,
                int    axis_
                ){

        //--- fanctor ---
        // random comparator
        //int axis = tprMath::get_random_int( 0, 2 );
        auto comparator =   ((axis_%3)==0) ? aabb_x_compare :
                            ((axis_%3)==1) ? aabb_y_compare :
                                        aabb_z_compare;
            // 测试表明，使用交替的 axis，要比 随机值 性能更好
            // 随机值：177 单位时间
            // 交替值：150 单位时间

        //--- left, right ---
        size_t span = end_ - begin_;
        if( span == 1 ){
            // both child have the same obj
            this->left = objs_.at(begin_);
            this->right = objs_.at(begin_);

        }else if( span == 2 ){
            // one side, one obj
            std::shared_ptr<IHittable> a = objs_.at(begin_);
            std::shared_ptr<IHittable> b = objs_.at(begin_+1);
            if( comparator( a, b ) ){
                this->left = a;
                this->right = b;
            }else{
                this->left = b;
                this->right = a;
            }

        }else{
            std::sort(  objs_.begin() + begin_,
                        objs_.begin() + end_,
                        comparator );

            size_t mid = begin_ + span/2;
            this->left  = std::make_shared<BVH_Node>( objs_, begin_, mid, time0_, time1_, axis_+1 );
            this->right = std::make_shared<BVH_Node>( objs_, mid,    end_,time0_, time1_, axis_+1 );
        }

        //--- aabb ---//
        AABB boxL {};
        AABB boxR {};
        bool retL = this->left->bounding_box( time0_, time1_, boxL );
        bool retR = this->right->bounding_box( time0_, time1_, boxR );
            tprAssert( retL && retR );
        this->aabb = AABB::surrounding_box( boxL, boxR );

    }

    void print()const{
        this->aabb.print();
    }



    bool hit( const Ray &r_, double tmin_, double tmax_, HitRecord &record_ )const override{
        if( !this->aabb.hitBox( r_, tmin_, tmax_ ) ){
            return false;
        }
        bool retL = this->left->hit(  r_, tmin_, tmax_, record_ );
        bool retR = this->right->hit( r_, tmin_, 
                                    retL ?  record_.t : tmax_,
                                    record_ );
            // left, right 是平等的，就算确认命中 left，也要再去检测 right
            // 每次调用 hit(), 通过参数 tmax_ 来做约束，一点点收缩 检测范围
            // 从而寻找到 t 值最小的 那个 obj,
            // ...
        return retL || retR;
    }


    bool bounding_box( double t0_, double t1_, AABB &output_aabb_ )const override{
        output_aabb_ = this->aabb;
        return true;
    }


private:

    std::shared_ptr<IHittable> left  {nullptr};
    std::shared_ptr<IHittable> right {nullptr};
    AABB  aabb {};
};


#endif 
