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
    
    static BVH_Node *factory( HittableList &hlist_, double time0_, double time1_ ){
        std::vector<IHittable*> &objPtrs = hlist_.get_objPtrs();
        return BVH_Node::factory(   objPtrs, 
                                    0,
                                    objPtrs.size(),
                                    time0_,
                                    time1_,
                                    0
                                    );
    }

    static BVH_Node *factory(  std::vector<IHittable*> &objs_,
                                size_t begin_,
                                size_t end_, // not include
                                double time0_, 
                                double time1_,
                                int    axis_
                                ){
        
        //debug::log( "beg:{},end{}", begin_, end_ );

        BVH_Node::nodeUPtrs.emplace_back( std::move( new BVH_Node() ));
        BVH_Node *node = BVH_Node::nodeUPtrs.back().get();
        //-----//

        //--- fanctor ---
        // random comparator
        //int axis = tprMath::get_random_int( 0, 2 );
        //debug::log( "axis:{}", axis );
        auto comparator =   ((axis_%3)==0) ? aabb_x_compare :
                            ((axis_%3)==1) ? aabb_y_compare :
                                        aabb_z_compare;
        //--- left, right ---
        size_t span = end_ - begin_;
        if( span == 1 ){
            // both child have the same obj
            node->left = objs_.at(begin_);
            node->right = objs_.at(begin_);

        }else if( span == 2 ){
            // one side, one obj
            IHittable *a = objs_.at(begin_);
            IHittable *b = objs_.at(begin_+1);
            if( comparator( a, b ) ){
                node->left = a;
                node->right = b;
            }else{
                node->left = b;
                node->right = a;
            }

        }else{
            std::sort(  objs_.begin() + begin_,
                        objs_.begin() + end_,
                        comparator );

            size_t mid = begin_ + span/2;
            node->left  = BVH_Node::factory( objs_, begin_, mid, time0_, time1_, axis_+1 );
            node->right = BVH_Node::factory( objs_, mid,    end_,time0_, time1_, axis_+1 );
        }

        //--- aabb ---//
        AABB boxL {};
        AABB boxR {};
        bool retL = node->left->bounding_box( time0_, time1_, boxL );
        bool retR = node->right->bounding_box( time0_, time1_, boxR );
            tprAssert( retL && retR );
        node->aabb = AABB::surrounding_box( boxL, boxR );

        //-----//
        return node;
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

    BVH_Node()=default;

    IHittable *left  {nullptr};
    IHittable *right {nullptr};
    AABB  aabb {};

    static std::vector<std::unique_ptr<BVH_Node>> nodeUPtrs;

};


std::vector<std::unique_ptr<BVH_Node>> BVH_Node::nodeUPtrs {};



#endif 
