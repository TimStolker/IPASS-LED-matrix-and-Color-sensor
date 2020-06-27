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
#include "display.hpp"


#define P_num 24
#define P_port PIOA
#define P_mask (1<<15)

int main(){
    sam3xa::SystemInit();
    hwlib::target::uart_init();

    WDT->WDT_MR = WDT_MR_WDDIS; //turn off watchdog timer so the arduino doesn't reset itself

    pmc_enable_periph_clk(ID_PIOA);
    pmc_enable_periph_clk(ID_PIOB);
    pmc_enable_periph_clk(ID_PIOC);
    pmc_enable_periph_clk(ID_PIOD);


    matrix<5,5> leds (P_port, P_mask);
    uint32_t testcolors[] = {RGB(0,0,0), RGB(255,255,255), RGB(100,100,100), RGB(255,0,0), RGB(0,255,0), RGB(0,0,255), RGB(255,0,255), RGB(255,255,0), RGB(0,255,255), UINT32_C(0xFFFFFFFF)};

    while(1){ 
        uint32_t *current = testcolors;
        while(*current != UINT32_C(0xFFFFFFFF)){ //shows all array colors on all leds
            leds.all_leds(*current);
            leds.update();
            hwlib::wait_ms(250);
            current++;
        }

        for(unsigned int i = 0;i < leds.column_count()*leds.row_count();i++){ //visits every led individually and loops through the array of colors
            leds.all_leds(0);
            current = testcolors;
            while(*current != UINT32_C(0xFFFFFFFF)){
                leds.set_led(i%leds.column_count(),i/leds.column_count(), *current);
                leds.update();
                hwlib::wait_ms(50);
                current++;
            }
            
        }
        for(unsigned int i = 0;i < leds.column_count();i++){ //visits every horizontal row individually and loops through the array of colors
            leds.all_leds(0);
            current = testcolors;
            while(*current != UINT32_C(0xFFFFFFFF)){
                leds.horizontal_row(i, *current);
                leds.update();
                hwlib::wait_ms(50);
                current++;
            }
            
        }

        for(unsigned int i = 0;i < leds.column_count();i++){ //visits every vertical row individually and loops through the array of colors
            leds.all_leds(0);
            current = testcolors;
            while(*current != UINT32_C(0xFFFFFFFF)){
                leds.vertical_row(i, *current);
                leds.update();
                hwlib::wait_ms(50);
                current++;
            }
            
        }

        for(unsigned int j = 0;j < 10;j++){ //randomizes all individual led colors
            leds.all_leds(0);
            for(unsigned int i = 0;i < leds.column_count()*leds.row_count();i++){
                leds.set_led(i%leds.column_count(),i/leds.column_count(), hwlib::rand());
            }
            leds.update();
            hwlib::wait_ms(250); 
        }

    }
    

    
}