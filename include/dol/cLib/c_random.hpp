#pragma once
#include <types.h>

// [Unofficial name]
/// @brief Random number generation helper class.
class cM_rand_c {
public:

    /// @brief Initializes the class with the given seed.
    cM_rand_c(u32 seed) { mSeed = seed; };

    u32 ranqd1(u32 max); ///< Generates an integer between 0 and the given max.
    float ranqd2(); ///< Generates a floating point number between 0 and 1.
    float ranqd3(); ///< Generates a floating point number between -0.5 and 0.5.

    u32 mSeed; ///< The current seed.

private:
    /**
     * @brief Flawed implementation of the @p ranqd1 algorithm.
     * <a href="https://roadrunnerwmc.github.io/blog/2020/05/08/nsmb-rng.html">Here is why.</a>
    */
    inline u32 ranqdStep();

    static u32 mConst1; ///< Constant @p a from the @p ranqd1 algorithm.
    static u32 mConst2; ///< Constant @p c from the @p ranqd1 algorithm.
};
