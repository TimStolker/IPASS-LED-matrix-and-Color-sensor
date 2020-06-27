
/**
 * \file
 *
 * \brief Power Management Controller (PMC) driver for SAM.
 *
 * Copyright (c) 2011-2019 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */


#include "pmc.h"

#if (SAM3N)
# define MAX_PERIPH_ID    31
#elif (SAM3XA)
# define MAX_PERIPH_ID    44
#elif (SAM3U)
# define MAX_PERIPH_ID    29
#elif (SAM3S || SAM4S)
# define MAX_PERIPH_ID    34
#elif (SAM4E)
# define MAX_PERIPH_ID    47
#elif (SAMV71)
# define MAX_PERIPH_ID    63
#elif (SAMV70)
# define MAX_PERIPH_ID    63
#elif (SAME70)
# define MAX_PERIPH_ID    63
#elif (SAMS70)
# define MAX_PERIPH_ID    63
#elif (SAM4N)
# define MAX_PERIPH_ID    31
#elif (SAM4C || SAM4CM || SAM4CP)
# define MAX_PERIPH_ID    43
#elif (SAMG51)
# define MAX_PERIPH_ID    47
#elif (SAMG53)
# define MAX_PERIPH_ID    47
#elif (SAMG54)
# define MAX_PERIPH_ID    47
#elif (SAMG55)
# define MAX_PERIPH_ID    50
#endif



uint32_t pmc_enable_periph_clk(uint32_t ul_id)
{
#if defined(REG_PMC_PCR) && !SAMG55
    uint32_t pcr;
    PMC->PMC_PCR = ul_id & 0x7F;
    pcr = PMC->PMC_PCR | PMC_PCR_EN | PMC_PCR_CMD;
    PMC->PMC_PCR = pcr;
    return 0;
#else
    if (ul_id > MAX_PERIPH_ID) {
        return 1;
    }

    if (ul_id < 32) {
        if ((PMC->PMC_PCSR0 & (1u << ul_id)) != (1u << ul_id)) {
            PMC->PMC_PCER0 = 1 << ul_id;
        }
#if (SAM3S || SAM3XA || SAM4S || SAM4E || SAM4C || SAM4CM || SAM4CP || SAMG55 || SAMV71 || SAMV70 || SAME70 || SAMS70)
    } else {
        ul_id -= 32;
        if ((PMC->PMC_PCSR1 & (1u << ul_id)) != (1u << ul_id)) {
            PMC->PMC_PCER1 = 1 << ul_id;
        }
#endif
    }

    return 0;
#endif /* defined(REG_PMC_PCR) && !SAMG55 */
}
