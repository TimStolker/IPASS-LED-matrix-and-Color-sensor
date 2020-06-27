// ==========================================================================
//
// File      : display.hpp
// Part of   : IPASS LED matrix and Color sensor
// Copyright : Tim Stolker - stolkertim@gmail.com 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

//this file contains Doxygen lines
///file display.hpp
/// \brief Datastruct for a LED matrix
/// \details  Contains all the necessary information about the matrix


#ifndef DISPLAY_HPP
#define DISPLAY_HPP


#define SAM3XA 1

#define register 
#include "sam.h"
#undef register
#include <cstdint>
#include "pmc.h"
#include "tc.h"


///////////////////////////////////////Used from FastLED library////////////////////////////////
//This part is used for the timers that send the bits to the WS2812B
#ifndef DUE_TIMER_CHANNEL
#define DUE_TIMER_GROUP 0
#endif

#ifndef DUE_TIMER_CHANNEL
#define DUE_TIMER_CHANNEL 0
#endif

#define DUE_TIMER ((DUE_TIMER_GROUP==0) ? TC0 : ((DUE_TIMER_GROUP==1) ? TC1 : TC2))
#define DUE_TIMER_ID (ID_TC0 + (DUE_TIMER_GROUP*3) + DUE_TIMER_CHANNEL)
#define DUE_TIMER_VAL (DUE_TIMER->TC_CHANNEL[DUE_TIMER_CHANNEL].TC_CV << 1)
#define DUE_TIMER_RUNNING ((DUE_TIMER->TC_CHANNEL[DUE_TIMER_CHANNEL].TC_SR & TC_SR_CLKSTA) != 0)
///////////////////////////////////////////////////////////////////////////////////////////////////




static inline void write_byte(uint_fast8_t byte, Pio* port, uint_fast32_t mask, uint32_t & next_time) __attribute__((always_inline, unused)); 
static inline void write_byte(uint_fast8_t byte, Pio* port, uint_fast32_t mask, uint32_t & next_time){

    for(uint_fast8_t i = 0; i < 8; i++){
        while(DUE_TIMER_VAL < next_time);
        next_time = DUE_TIMER_VAL + 105; // 1,25 us * 84MHz = 105 // 1,25 us is the time of a complete bit writing (amount of cycles)
        port -> PIO_SODR = mask; 

        if(byte & 0b10000000){ //check MSB 
                                        
            while( (next_time - DUE_TIMER_VAL) > 31 ); // Timing from FastLED's library // Write a 1

        }
        else{
            while( (next_time - DUE_TIMER_VAL) > 84 ); // Timing from FastLED's library // write a 0

        }
        port -> PIO_CODR = mask;
        byte = byte << 1;
    }
    
}

#define RGB(R,G,B) ( ( (uint32_t) ( R ) << 16 & 0x00FF0000 ) | ((uint32_t) ( G ) << 8 & 0x0000FF00 ) | ((uint32_t) ( B ) & 0x000000FF )  ) 


template <unsigned int rows, unsigned int columns> class matrix{
private:
    Pio* const port;
    const uint32_t mask;
    uint32_t leds[rows*columns] = {0};
public:
    /// \brief Constructor for the matrix.
    /// \details Accepts a port and it's mask.
    matrix(Pio* const port, const uint32_t mask):
    port(port),
    mask(mask)
    {
        port -> PIO_PER = mask;
        port -> PIO_OER = mask;
        port -> PIO_PUDR = mask;
    }


    /// \brief Lights up a single LED.
    /// \details This function requires an x coordinate, a y coordinate and a color. The color is a 32 bit value with each 8 bits being a color. From left to right: Red,Green,Blue. The first 8 bits are valued 0.
    void set_led(unsigned int x, unsigned int y, uint32_t color){
    size_t index;
    if(y%2==0){
        index = y*columns + x;
    }
    else{
        index = (y+1)*columns - 1 - x;
    }
    leds[index] = color;
}
    /// \brief Updates the LED matrix.
    /// \details Uses the write_byte function to write the color on the LED strip.
    void update(){
    tc_init(DUE_TIMER,DUE_TIMER_CHANNEL,TC_CMR_TCCLKS_TIMER_CLOCK1);
    pmc_enable_periph_clk(DUE_TIMER_ID);
    tc_start(DUE_TIMER,DUE_TIMER_CHANNEL);
    port -> PIO_CODR = mask;
    __disable_irq(); __disable_fault_irq();

    uint32_t next_time = DUE_TIMER_VAL + 105;

    for(size_t i = 0; i < rows*columns; i++){
        write_byte(((leds[i] >> 8) & 0xFF), port, mask, next_time);
        write_byte(((leds[i] >> 16) & 0xFF), port, mask, next_time);
        write_byte(((leds[i]) & 0xFF), port, mask, next_time);
    }
    
    __enable_irq(); __enable_fault_irq();
    tc_stop(DUE_TIMER,DUE_TIMER_CHANNEL);

}

    /// \brief Lights up all the leds.
    /// \details Requires a color. The color is a 32 bit value with each 8 bits being a color. From left to right: Red,Green,Blue. The first 8 bits are valued 0. This will light up all the leds in the matrix with the same color.
    void all_leds(uint32_t color){

    for(unsigned int i = 0; i < rows*columns; i++){ 
        leds[i] = color; 
    }
}

    /// \brief Lights up a horizontal row of the same color on the matrix.
    /// \details Requires a y coordinate and a color. The color is a 32 bit value with each 8 bits being a color. From left to right: Red,Green,Blue. The first 8 bits are valued 0.
    void horizontal_row(unsigned int y, uint32_t color){

    for(unsigned int i = 0; i < columns; i++){
        set_led(i,y,color);
    }

}


    /// \brief Lights up a vertical row of the same color on the matrix.
    /// \details Requires an x coordinate and a color. The color is a 32 bit value with each 8 bits being a color. From left to right: Red,Green,Blue. The first 8 bits are valued 0.
    void vertical_row(unsigned int x, uint32_t color){

    for(unsigned int i = 0; i < rows; i++){
        set_led(x,i,color);
    }
}


    
};

#endif
