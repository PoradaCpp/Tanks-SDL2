#ifndef RANDOM_UINT32_T_H
#define RANDOM_UINT32_T_H

#include <random>
#include <stdint.h>
#include <functional>

class Random_uint32_t
{
public:
    Random_uint32_t( uint32_t nMinValue, uint32_t nMaxValue );
    ~Random_uint32_t();

    uint32_t operator()() const;

private:
    std::mt19937 m_Engine;
    std::function <uint32_t()> m_Generator;
};

#endif // RANDOM_UINT32_T_H
