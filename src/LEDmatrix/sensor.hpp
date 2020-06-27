// ==========================================================================
//
// File      : sensor.hpp
// Part of   : IPASS LED matrix and Color sensor
// Copyright : Tim Stolker - stolkertim@gmail.com 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

//this file contains Doxygen lines
///file sensor.hpp
/// \brief Datastruct for returning the RGB values.
/// \details This class needs 5 digital datapins to work correctly.




#ifndef SENSOR_HPP
#define SENSOR_HPP

#define SAM3XA 1

#define register 
#include "sam.h"
#undef register
#include <cstdint>




class sensor{
    private:
    Pio* port_out;
    uint32_t mask_out;
    Pio* port_s0;
    uint32_t mask_s0;
    Pio* port_s1;
    uint32_t mask_s1;
    Pio* port_s2;
    uint32_t mask_s2;
    Pio* port_s3;
    uint32_t mask_s3;

    public:
    /// \brief Constructor for all the digital data pins.
    sensor(Pio* port_out, uint32_t mask_out, 
    Pio* port_s0,uint32_t mask_s0,
    Pio* port_s1,uint32_t mask_s1,
    Pio* port_s2,uint32_t mask_s2,
    Pio* port_s3,uint32_t mask_s3);

    /// \brief This returns a 32 bit value with each 8 bits being a color. From left to right: Red,Green,Blue. The first 8 bits are valued 0.
    uint32_t getRGB();


};

#endif
