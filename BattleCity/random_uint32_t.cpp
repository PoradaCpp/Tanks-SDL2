#include <ctime>

#include "random_uint32_t.h"

Random_uint32_t::Random_uint32_t( uint32_t nMinValue, uint32_t nMaxValue )
{
    uint32_t m_nMinValue( nMinValue < nMaxValue ? nMinValue : nMaxValue );
    uint32_t m_nMaxValue( nMinValue < nMaxValue ? nMaxValue : nMinValue );
    //std::random_device device;

    m_Engine.seed( static_cast <uint32_t> ( std::time(nullptr) ) );
    m_Generator = std::bind( std::uniform_int_distribution <uint32_t> ( m_nMinValue, m_nMaxValue ), m_Engine );
}

Random_uint32_t::~Random_uint32_t() {}

uint32_t Random_uint32_t::operator()() const
{
    return m_Generator();
}
