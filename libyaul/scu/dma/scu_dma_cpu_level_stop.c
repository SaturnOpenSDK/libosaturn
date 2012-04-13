/*
 * Copyright (c) 2011 Israel Jacques
 * See LICENSE for details.
 *
 * Israel Jacques <mrko@eecs.berkeley.edu>
 */

#include <dma/dma.h>

#include "dma_internal.h"

void
scu_dma_cpu_level_stop(enum dma_level lvl)
{

        switch (lvl) {
        case DMA_LEVEL_0:
                MEM_POKE(DMA_LEVEL(0, D0EN), 0x00000000);
                return;
        case DMA_LEVEL_1:
                MEM_POKE(DMA_LEVEL(1, D1EN), 0x00000000);
                return;
        case DMA_LEVEL_2:
                MEM_POKE(DMA_LEVEL(2, D2EN), 0x00000000);
                return;
        default:
                /* Panic */
                return;
        }
}