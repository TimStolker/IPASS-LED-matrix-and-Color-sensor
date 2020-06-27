// ==========================================================================
//
// File      : main.cpp
// Part of   : IPASS LED matrix and Color sensor
// Copyright : Tim Stolker - stolkertim@gmail.com 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================
#define SAM3XA 1

#define register 
#include "sam.h"
#undef register
#include "hwlib.hpp"
#include <cstdint>
#include "pmc.h"
#include "sensor.hpp"


#define OUT_num 8
#define OUT_port PIOC
#define OUT_mask (1<<22)

#define S0_num 4
#define S0_port PIOA
#define S0_mask (1<<29)

#define S1_num 5
#define S1_port PIOC
#define S1_mask (1<<25)

#define S2_num 6
#define S2_port PIOC
#define S2_mask (1<<24)

#define S3_num 7
#define S3_port PIOC
#define S3_mask (1<<23)


int main(){
    sam3xa::SystemInit();
    hwlib::target::uart_init();

    WDT->WDT_MR = WDT_MR_WDDIS; //turn off watchdog timer so the arduino doesn't reset itself

    pmc_enable_periph_clk(ID_PIOA);
    pmc_enable_periph_clk(ID_PIOB);
    pmc_enable_periph_clk(ID_PIOC);
    pmc_enable_periph_clk(ID_PIOD);

    
    sensor sensor(OUT_port, OUT_mask, S0_port, S0_mask, S1_port, S1_mask, S2_port, S2_mask, S3_port, S3_mask);
   
    while(1){
        uint32_t color = sensor.getRGB();
        hwlib::cout << "R: " << C_red(color) << " G: " << C_green(color) << " B: " << C_blue(color) << "\n";
        hwlib::wait_ms(500);
    }
    

    
}