/*
 * ========================= RGBA.h ==========================
 *                          -- tpr --
 *                                        CREATE -- 2020.10.01
 *                                        MODIFY --
 * ----------------------------------------------------------
 *   png pix color: RGBA
 * ----------------------------
 */
#ifndef TPR_RGBA_H
#define TPR_RGBA_H

#include "pch.h"



enum class RGBA_ChannelType : uint8_t {
    R = 1,
    G,
    B,
    A
};


//-- 1个 png 像素 的 data 结构 --
class RGBA{
public: 
    //---- constructor -----//
    constexpr RGBA() = default;
    constexpr RGBA( uint8_t r_, uint8_t g_, uint8_t b_, uint8_t a_ ):
        r(r_),
        g(g_),
        b(b_),
        a(a_)
        {}
    
    inline bool is_near( const RGBA &a_, uint8_t off_ )const noexcept {
        int rr = static_cast<int>(this->r) - static_cast<int>(a_.r);
        int gg = static_cast<int>(this->g) - static_cast<int>(a_.g);
        int bb = static_cast<int>(this->b) - static_cast<int>(a_.b);
        int aa = static_cast<int>(this->a) - static_cast<int>(a_.a);
        return (
        (std::abs(rr) <= static_cast<int>(off_)) &&
        (std::abs(gg) <= static_cast<int>(off_)) &&
        (std::abs(bb) <= static_cast<int>(off_)) &&
        (std::abs(aa) <= static_cast<int>(off_))
        );
    }

    //-- 支持更复杂的数据 累加 --
    inline RGBA add( int r_, int g_, int b_, int a_ )const noexcept {
        int rr = static_cast<int>(this->r) + r_;
        int gg = static_cast<int>(this->g) + g_;
        int bb = static_cast<int>(this->b) + b_;
        int aa = static_cast<int>(this->a) + a_;
        tprAssert(  (rr>=0) && (rr<256) &&
                    (gg>=0) && (gg<256) &&
                    (bb>=0) && (bb<256) );
        return RGBA {   static_cast<uint8_t>(rr), 
                        static_cast<uint8_t>(gg), 
                        static_cast<uint8_t>(bb), 
                        static_cast<uint8_t>(aa) };
    }


    inline std::string to_string()const noexcept{ // for debug
        return fmt::format( "RGBA:{0},{1},{2},{3};", this->r, this->g, this->b, this->a );
    }

    //======== vals ========//
    uint8_t r {0};
    uint8_t g {0};
    uint8_t b {0};
    uint8_t a {255};
};

/* ===========================================================
 *                  operator  ==, !=
 * -----------------------------------------------------------
 */
inline constexpr bool operator==( const RGBA &a_, const RGBA &b_  ) noexcept {
    return ( (a_.r==b_.r) && (a_.g==b_.g) && (a_.b==b_.b) && (a_.a==b_.a) );
}
inline constexpr bool operator!=( const RGBA &a_, const RGBA &b_  ) noexcept {
    return ( (a_.r!=b_.r) || (a_.g!=b_.g) || (a_.b!=b_.b) || (a_.a!=b_.a) );
}


/* ===========================================================
 *                   operator +
 * -----------------------------------------------------------
 */
inline constexpr RGBA operator + ( const RGBA &a_, const RGBA &b_ ) noexcept {
    int rr = static_cast<int>(a_.r) + static_cast<int>(b_.r);
    int gg = static_cast<int>(a_.g) + static_cast<int>(b_.g);
    int bb = static_cast<int>(a_.b) + static_cast<int>(b_.b);
    int aa = static_cast<int>(a_.a) + static_cast<int>(b_.a);
        tprAssert( (rr<256) && (gg<256) && (bb<256) && (aa<256) );
    return RGBA {   static_cast<uint8_t>(rr), 
                    static_cast<uint8_t>(gg),
                    static_cast<uint8_t>(bb),
                    static_cast<uint8_t>(aa) };
}


namespace rgba {//-------- namespace: rgba --------------//

//-- 只要两个 RGBA 值 足够接近，就算命中 [-常用-] --
inline bool is_rgba_near( const RGBA &a_, const RGBA &b_, uint8_t off_ ) noexcept {

    int rr = static_cast<int>(a_.r) - static_cast<int>(b_.r);
    int gg = static_cast<int>(a_.g) - static_cast<int>(b_.g);
    int bb = static_cast<int>(a_.b) - static_cast<int>(b_.b);
    int aa = static_cast<int>(a_.a) - static_cast<int>(b_.a);
    return (
       (std::abs(rr) <= static_cast<int>(off_)) &&
       (std::abs(gg) <= static_cast<int>(off_)) &&
       (std::abs(bb) <= static_cast<int>(off_)) &&
       (std::abs(aa) <= static_cast<int>(off_))
    );
}

}//------------- namespace: rgba end --------------//
#endif

