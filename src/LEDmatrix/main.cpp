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
#include "tc.h"
#include "display.hpp"
#include "sensor.hpp"

#define SCAN_num 11
#define SCAN_port PIOD
#define SCAN_mask (1<<7)

#define X_num 26
#define X_port PIOD
#define X_mask (1<<1)

#define Y_num 28
#define Y_port PIOD
#define Y_mask (1<<3)

#define P_num 24
#define P_port PIOA
#define P_mask (1<<15)

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

    SCAN_port -> PIO_PER = SCAN_mask;
    SCAN_port -> PIO_ODR = SCAN_mask;
    SCAN_port -> PIO_PUER = SCAN_mask; 

    X_port -> PIO_PER = X_mask;
    X_port -> PIO_ODR = X_mask;
    X_port -> PIO_PUER = X_mask; 

    Y_port -> PIO_PER = Y_mask;
    Y_port -> PIO_ODR = Y_mask;
    Y_port -> PIO_PUER = Y_mask; 

    matrix<5,5> leds (P_port, P_mask);
    sensor sensor(OUT_port, OUT_mask, S0_port, S0_mask, S1_port, S1_mask, S2_port, S2_mask, S3_port, S3_mask);
    int x = 0;
    int y = 0;
    bool xbutton = false;
    bool ybutton = false;
    bool scan = false;
    uint32_t tempcolor = 0;
    while(1){
        bool scan_pressed = ((SCAN_port -> PIO_PDSR) & SCAN_mask) == 0;
        if (!scan && scan_pressed){
            tempcolor = sensor.getRGB();
        }
        scan = scan_pressed;
        leds.set_led(x,y,tempcolor);
        
        bool x_pressed = ((X_port -> PIO_PDSR) & X_mask) == 0;
        if (!xbutton && x_pressed){
            x = (x+1)%(5);
            
        }
        xbutton = x_pressed;

        bool y_pressed = ((Y_port -> PIO_PDSR) & Y_mask) == 0;
        if (!ybutton && y_pressed){
            y = (y+1)%(5);

        }
        ybutton = y_pressed;
        tempcolor = leds.get_led(x,y);
        bool cursor = (hwlib::now_us()/250000)%2 == 0;
        if(cursor){
            leds.set_led(x,y,RGB(100,100,100));
        }
        else{
            leds.set_led(x,y,RGB(0,0,0));
        }
        hwlib::wait_ms(3); //minimum timing used for FastLED library
        leds.update();
         
    }
    

    
}