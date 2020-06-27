#include "sensor.hpp"

#include "tc.h"
#include "hwlib.hpp"
#include "pmc.h"

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

#define RGB(R,G,B) ( ( (uint32_t) ( R ) << 16 & 0x00FF0000 ) | ((uint32_t) ( G ) << 8 & 0x0000FF00 ) | ((uint32_t) ( B ) & 0x000000FF )  ) 

#define set_red() port_s2 -> PIO_CODR = mask_s2; port_s3 -> PIO_CODR = mask_s3 
#define set_green() port_s2 -> PIO_SODR = mask_s2; port_s3 -> PIO_SODR = mask_s3
#define set_blue() port_s2 -> PIO_CODR = mask_s2; port_s3 -> PIO_SODR = mask_s3


static uint32_t timer(Pio* port, uint32_t mask){
    while((port -> PIO_PDSR & mask) != 0){    //waits for pin to go from high to low
    }
    while((port -> PIO_PDSR & mask) == 0){    //waits for pin to go from low to high
    }

    tc_init(DUE_TIMER,DUE_TIMER_CHANNEL,TC_CMR_TCCLKS_TIMER_CLOCK1);
    pmc_enable_periph_clk(DUE_TIMER_ID);
    tc_start(DUE_TIMER,DUE_TIMER_CHANNEL);
    __disable_irq(); __disable_fault_irq();

    uint32_t start_time = DUE_TIMER_VAL;

    while((port -> PIO_PDSR & mask) != 0){      //waits for pin to go from high to low
    }

    uint32_t end_time = DUE_TIMER_VAL;

    __enable_irq(); __enable_fault_irq();
    tc_stop(DUE_TIMER,DUE_TIMER_CHANNEL);

    return end_time - start_time;
}

//This function scales a min and max value to another min and max value
static float map(float x,float x_min,float x_max,float out_min,float out_max){
    return (x - x_min) * (out_max - out_min) / (x_max - x_min) + out_min;
}

//This function makes sure that a value contains between 2 values
static float clamp(float x, float x_min, float x_max){
    if (x < x_min){
        return x_min;
    }
    else if(x > x_max){
        return x_max;
    }
    else{
        return x;
    }
}


sensor::sensor(Pio* port_out, uint32_t mask_out, 
Pio* port_s0,uint32_t mask_s0,
Pio* port_s1,uint32_t mask_s1,
Pio* port_s2,uint32_t mask_s2,
Pio* port_s3,uint32_t mask_s3):

port_out(port_out), mask_out(mask_out),
port_s0(port_s0), mask_s0(mask_s0),
port_s1(port_s1), mask_s1(mask_s1),
port_s2(port_s2), mask_s2(mask_s2),
port_s3(port_s3), mask_s3(mask_s3)
{
    port_out -> PIO_PER = mask_out;
    port_out -> PIO_ODR = mask_out;
    port_out -> PIO_PUDR = mask_out;

    port_s0 -> PIO_PER = mask_s0;
    port_s0 -> PIO_OER = mask_s0;
    port_s0 -> PIO_PUDR = mask_s0;

    port_s1 -> PIO_PER = mask_s1;
    port_s1 -> PIO_OER = mask_s1;
    port_s1 -> PIO_PUDR = mask_s1;

    port_s2 -> PIO_PER = mask_s2;
    port_s2 -> PIO_OER = mask_s2;
    port_s2 -> PIO_PUDR = mask_s2;

    port_s3 -> PIO_PER = mask_s3;
    port_s3 -> PIO_OER = mask_s3;
    port_s3 -> PIO_PUDR = mask_s3;

    port_s0 -> PIO_CODR = mask_s0;
    port_s1 -> PIO_SODR = mask_s1;
}

uint32_t sensor::getRGB(){
    set_red();
    hwlib::wait_ms(1);
    uint32_t red = timer(port_out, mask_out);
    float red_float = map(red, 15000, 43066, 255, 0); //These are self measured minimum and maximum frequencies. These values aren't the most reliable.
    red_float = clamp(red_float, 0 , 255);

    set_green();
    hwlib::wait_ms(1);
    uint32_t green = timer(port_out, mask_out);
    float green_float = map(green, 24000, 42000, 255, 0); //These are self measured minimum and maximum frequencies. These values aren't the most reliable.
    green_float = clamp(green_float, 0 , 255);

    set_blue();
    hwlib::wait_ms(1);
    uint32_t blue = timer(port_out, mask_out);
    float blue_float = map(blue, 15000, 42000, 255, 0); //These are self measured minimum and maximum frequencies. These values aren't the most reliable.
    blue_float = clamp(blue_float, 0 , 255);

    return RGB(red_float,green_float,blue_float);


    //outputs the RGB in byte and frequency values
/*  hwlib::cout << "red: " << (int)red_float << "\n";
    hwlib::cout << "green: " << (int)green_float << "\n";
    hwlib::cout << "blue: " << (int)blue_float << "\n";
    hwlib::cout << red << " " << green << " " << blue << " " << white << "\n"; */

}
