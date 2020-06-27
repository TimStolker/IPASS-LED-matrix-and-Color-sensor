//This will let you cicle trough the matrix with a button
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

#define BUTTON_num 11
#define BUTTON_port PIOD
#define BUTTON_mask (1<<7)

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

    BUTTON_port -> PIO_PER = BUTTON_mask;
    BUTTON_port -> PIO_ODR = BUTTON_mask;
    BUTTON_port -> PIO_PUER = BUTTON_mask; 

    matrix<5,5> leds (P_port, P_mask);
    sensor sensor(OUT_port, OUT_mask, S0_port, S0_mask, S1_port, S1_mask, S2_port, S2_mask, S3_port, S3_mask);
    int count = 0;
    while(1){
        hwlib::wait_ms(200);
        bool is_pressed = ((BUTTON_port -> PIO_PDSR) & BUTTON_mask) == 0;
        if(is_pressed){
            leds.set_led(count%5,count/5,sensor.getRGB());
            leds.update();
            count = (count+1)%(5*5);
        }
        else{
        }
         
    }
    

    
}
