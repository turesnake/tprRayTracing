/*
 * ======================== Texture.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.09
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 *  
 */
#ifndef TPR_TEXTURE_H
#define TPR_TEXTURE_H

#include "pch.h"

#include "Perlin.h"



class Texture{
public:
    Texture()=default;
    virtual ~Texture()=default;// MUST
    virtual glm::dvec3 value( double u_, double v_, const glm::dvec3 &pos_ )const=0;
};



class SolidColor : public Texture{
public:
    SolidColor()=default;

    SolidColor( const glm::dvec3 &colorVal_ ):
        colorVal(colorVal_)
        {}

    SolidColor( double r_, double g_, double b_ ):
        SolidColor( glm::dvec3{r_,g_,b_} )
        {}

    // only one color
    glm::dvec3 value( double u_, double v_, const glm::dvec3 &pos_ )const override{
        return this->colorVal;
    }

private:
    glm::dvec3 colorVal {};
};





class CheckerTexture : public Texture{
public:
    CheckerTexture()=default;

    CheckerTexture( std::shared_ptr<Texture> odd_,
                    std::shared_ptr<Texture> even_
        ):odd(odd_), even(even_)
        {}

    CheckerTexture( const glm::dvec3 c1_,
                    const glm::dvec3 c2_
        ):
        odd( std::make_shared<SolidColor>(c1_) ),
        even( std::make_shared<SolidColor>(c2_) )
        {}


    glm::dvec3 value( double u_, double v_, const glm::dvec3 &pos_ )const override{
        
        double freq = 10.0;
        double sines =  sin(freq*pos_.x) *
                        sin(freq*pos_.y) *
                        sin(freq*pos_.z);
        if( sines < 0.0 ){
            return this->odd->value( u_, v_, pos_ );
        }else{
            return this->even->value( u_, v_, pos_ );
        }
    }

private:
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};




class NoiseTexture : public Texture{
public:
    NoiseTexture()=default;
    NoiseTexture( double scale_ ):scale(scale_){}

    glm::dvec3 value( double u_, double v_, const glm::dvec3 &pos_ )const override{

        // regular noise
        //return glm::dvec3{1.0,1.0,1.0} * 0.5 * (1.0 + this->noise.noise( this->scale * pos_ ));

        // marble-like noise
        return  glm::dvec3{1.0,1.0,1.0} * 
                0.5 *
                ( 1.0 + sin( this->scale*pos_.z + 10.0*noise.turb(pos_)));

    }

private:
    Perlin noise {};
    double scale {};

};




#endif 
