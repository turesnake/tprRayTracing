/*
 * ========================= Perlin.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 */
#ifndef TPR_PERLIN_H
#define TPR_PERLIN_H

#include "pch.h"


inline double trilinear_interp( double c_[2][2][2], double u_, double v_, double w_ ){

    double accum = 0.0;
    for( int i=0; i<2; i++ ){
        for( int j=0; j<2; j++ ){
            for( int k=0; k<2; k++ ){

                accum +=( i*u_ + (1-i)*(1.0-u_) ) *
                        ( j*v_ + (1-j)*(1.0-v_) ) *
                        ( k*w_ + (1-k)*(1.0-w_) ) *
                        c_[i][j][k];
            }
        }
    }
    return accum;
}





class Perlin{
public:
    Perlin(){
        this->ranvec = new glm::dvec3[Perlin::point_count];
        for( int i=0; i<Perlin::point_count; i++ ){
            glm::dvec3 v {
                tprMath::get_random_double( -1.0,1.0 ),
                tprMath::get_random_double( -1.0,1.0 ),
                tprMath::get_random_double( -1.0,1.0 )
            };
            this->ranvec[i] = glm::normalize( v );
        }
        this->perm_x = Perlin::perlin_generate_perm();
        this->perm_y = Perlin::perlin_generate_perm();
        this->perm_z = Perlin::perlin_generate_perm();
    }

    ~Perlin(){
        delete[] this->ranvec;
        delete[] this->perm_x;
        delete[] this->perm_y;
        delete[] this->perm_z;
    }

    double noise( const glm::dvec3 &pos_ )const{

        // 小数部分
        double u = pos_.x - floor(pos_.x);
        double v = pos_.y - floor(pos_.y);
        double w = pos_.z - floor(pos_.z);

        //
        int i = floor(pos_.x);
        int j = floor(pos_.y);
        int k = floor(pos_.z);

        glm::dvec3 c[2][2][2] {};

        for( int di=0; di<2; di++ ){
            for( int dj=0; dj<2; dj++ ){
                for( int dk=0; dk<2; dk++ ){

                    int idx =   perm_x[ (i+di) & 255 ] ^
                                perm_y[ (j+dj) & 255 ] ^
                                perm_z[ (k+dk) & 255 ];

                    c[di][dj][dk] = this->ranvec[ idx ];
                }
            }
        }

        return this->perlin_interp( c, u,v,w );
    }

    // 将多层 noise 叠加，获得 湍流效果
    double turb( const glm::dvec3 &pos_, int depth=7 )const{

        double accum = 0.0;
        glm::dvec3 tmp_pos = pos_;
        double weight = 1.0;

        for( int i=0; i<depth; i++ ){
            accum += weight * this->noise( tmp_pos );
            weight *= 0.5;
            tmp_pos *= 2.0;
        }

        return std::abs( accum );
    }


private:
    static const int point_count = 256;
    glm::dvec3 *ranvec;// 256 random_dvec3_val, [0.0,1.0]
    int *perm_x;// {0,...255} be shuffled
    int *perm_y;// {0,...255} be shuffled
    int *perm_z;// {0,...255} be shuffled

    static int *perlin_generate_perm(){
        auto p = new int[Perlin::point_count];

        for( int i=0; i<Perlin::point_count; i++ ){
            p[i] = i;
        }
        Perlin::permute( p, Perlin::point_count );
        return p;
    }

    // 将数组中， 除 p_[0] 之外的所有值，都和任意某个值，swap 一次
    static void permute( int *p_, int n_ ){
        for( int i=n_-1; i>0; i-- ){// not change p_[0]

            int tgtIdx = tprMath::get_random_int(0,i);
            //std::swap( p_[i], p_[tgtIdx] );
            int iVal = p_[i];
            p_[i] = p_[tgtIdx];
            p_[tgtIdx] = iVal;
        }
    }


    double perlin_interp( glm::dvec3 c_[2][2][2], double u_, double v_, double w_ )const{

        // -2x^3+3x^2 [0.0,1.0]
        // 让分布 更靠近 0.0, 1.0
        double uu = u_*u_*(3.0-2.0*u_);
        double vv = v_*v_*(3.0-2.0*v_);
        double ww = w_*w_*(3.0-2.0*w_);
        double accum = 0.0;

        for( int i=0; i<2; i++ ){
            for( int j=0; j<2; j++ ){
                for( int k=0; k<2; k++ ){

                    glm::dvec3 weightV { u_-i, v_-j, w_-k };

                    accum +=( i*uu + (1-i)*(1.0-uu) ) *
                            ( j*vv + (1-j)*(1.0-vv) ) *
                            ( k*ww + (1-k)*(1.0-ww) ) *
                            glm::dot( c_[i][j][k], weightV );
                }
            }
        }
        return accum;
    }

};






#endif 
