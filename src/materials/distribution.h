/*
 * ====================== distribution.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.03
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 * some distribution funcs
 */
#ifndef TPR_DISTRIBUTION_H
#define TPR_DISTRIBUTION_H

#include "pch.h"


// calc the reflect dir(out)
// vin_ point to hitPoint, and we need vOut
// vOut = vin_ + 2b
// ---
// do not care if vin_ is a normal-vector
inline glm::dvec3 reflect( const glm::dvec3 &vin_, const glm::dvec3 &n_ ){
    // b: a vec with same dir to n_
    // b.len = cos() * vin_.len;
    glm::dvec3 b = - glm::dot(vin_,n_) * n_;
    return vin_ + 2.0*b;
}


// find a point on sphere surface
// create a vector from {0,0,0} to that point
// unit sphere: center={0,0,0}, radius=1;
// for Lambertian Reflection probability distribution
inline glm::dvec3 create_random_pos_on_unitSphere(){

    double angle = tprMath::get_random_double( 0.0, 2.0*tprMath::pi );
    double z = tprMath::get_random_double( -1.0, 1.0 );
    double r = sqrt( 1.0 - z*z );
    return glm::dvec3{ r*cos(angle), r*sin(angle), z };
}


// find a point in sphere
// create a vector from {0,0,0} to that point
// unit sphere: center={0,0,0}, radius=1;
// for Lambertian Reflection probability distribution
inline glm::dvec3 create_random_pos_in_unitSphere(){
    while( true ){
        glm::dvec3 v {
            tprMath::get_random_double( -1.0, 1.0 ),
            tprMath::get_random_double( -1.0, 1.0 ),
            tprMath::get_random_double( -1.0, 1.0 )
        };
        double len = glm::length(v);
        if( len*len < 1.0 ){
            return v;
        }
    }
}


inline glm::dvec3 create_random_pos_in_unitDisk(){
    while (true){
        glm::dvec3 v {
            tprMath::get_random_double( -1.0, 1.0 ),
            tprMath::get_random_double( -1.0, 1.0 ),
            0.0
        };
        double len = glm::length(v);
        if( len*len < 1.0 ){
            return v;
        }
    }
}



// ray from mat_A to mat_B
// etaA_over_etaB_ = refractIndex_A / refractIndex_B
// ...
// param: vin_,n_ are normal-vectors
inline glm::dvec3 refract(  const glm::dvec3 &vin_,
                            const glm::dvec3 &n_,
                            double etaA_over_etaB_ ){

    double cosA = glm::dot( -vin_, n_ );
    glm::dvec3 Bx = etaA_over_etaB_ * ( vin_ + cosA*n_ );
    double BxLen = glm::length( Bx );
    glm::dvec3 By = - sqrt(abs(1.0 - BxLen*BxLen)) * n_;
    return Bx + By;
}



// Schlick Approximation (for refraction)
// calc the reflect probability [0.0, 1.0]
// if ray-angle close to n, more refract (looks like a window)
// if ray-angle steep to n, less refract (looks like a mirror)
// ...
inline double schlick( double cos_, double refractIndex_ ){
    double r0 = (1.0-refractIndex_) / (1.0+refractIndex_);
    r0 *= r0;
    return r0 + (1.0-r0) * pow( (1.0-cos_) , 5.0);
}




#endif 
