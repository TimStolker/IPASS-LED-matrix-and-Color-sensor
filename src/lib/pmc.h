#ifndef PMC_H
#define PMC_H

#define SAM3XA 1


#ifdef __cplusplus
extern "C" {
#endif


#include "sam.h"
#include <stdint.h>

uint32_t pmc_enable_periph_clk(uint32_t ul_id);

#ifdef __cplusplus
}
#endif

#endif
