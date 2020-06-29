# IPASS-LED-matrix-and-Color-sensor

## parts used:
   - WS2812b LED strip
   - TCS3200D-TCS230
   - Arduino Due

## How to use:
1. install the whole lib directory
2. include the headers:
      ```C++
      #define register 
      #include "sam.h"
      #undef register
      #include "hwlib.hpp"
      #include <cstdint>
      #include "pmc.h"
      #include "tc.h"
      #include "display.hpp"
      ```
3. Define SAM3XA `#define SAM3XA 1`
4. Define the pins that you connected to the Arduino Due 
   Here is what you should define if you are using the Fritzing layout from layout.png: 
      ```C++
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
      ```
5. To be able to controll the leds, you need to add this at the top of you main() :
      ```C++
      sam3xa::SystemInit();
      hwlib::target::uart_init();

      WDT->WDT_MR = WDT_MR_WDDIS; //turn off watchdog timer so the arduino doesn't reset itself

      pmc_enable_periph_clk(ID_PIOA);
      pmc_enable_periph_clk(ID_PIOB);
      pmc_enable_periph_clk(ID_PIOC);
      pmc_enable_periph_clk(ID_PIOD);
      ```
6. You now have everything to start using the matrix and the sensor. You can make a matrix like so: matrix<amount of columns, amount of rows> name (P_port, P_mask);
   You can add a sensor like this: sensor name (OUT_port, OUT_mask, S0_port, S0_mask, S1_port, S1_mask, S2_port, S2_mask, S3_port, S3_mask);
   From here on you can use the functions described in the doxygen.

## layout
![layout](https://github.com/TimStolker/IPASS-LED-matrix-and-Color-sensor/raw/master/Layout.png "layout") 
                                                                                          
