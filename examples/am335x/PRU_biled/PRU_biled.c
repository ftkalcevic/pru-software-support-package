
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

#define PRU_SHARED_MEM_ADDR 0
#define PRU_CLK 200000000	// 200 MHz

#define	RED		(1<<2)
#define GREEN		(1<<5)

struct SInterface
{
	uint32_t mode;		// 0-off, 1-red, 2-green, 3-red/green
	uint32_t period;	// ms
};

//#pragma NOINIT(interface)
#pragma LOCATION(interface, PRU_SHARED_MEM_ADDR)
volatile struct SInterface interface;

void main(void)
{
	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
	
	// Reset interface
	interface.mode = 0;
	interface.period = 1000;

	while (1) {
		uint32_t period = interface.period;
		uint32_t mode = interface.mode;
		uint32_t i;

		if ( mode == 1 || mode == 3)
			__R30 = RED;
		else if ( mode == 2 )
			__R30 = GREEN;
		else
			__R30 = 0;

		for ( i = 0; i < period; i++ )
			__delay_cycles(PRU_CLK*0.0005);

		if ( mode == 3 )
			__R30 = GREEN;
		else
			__R30 = 0;

		for ( i = 0; i < period; i++ )
			__delay_cycles(PRU_CLK*0.0005);
	}
}

