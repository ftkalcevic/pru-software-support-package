/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <string.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "dcc.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

#define PRU_SHARED_MEM_ADDR 0
#define PRU_CLK	    200000000
#define ONE	        (PRU_CLK*0.000058)		// 58 us
#define ZERO	    (PRU_CLK*0.000100)		// 100 us

#define DCC_BIT (1 << 2)    // P9_27

#pragma LOCATION(PRU_SHARED_MEM_ADDR)
SDCCMessageQueue queue;

SDCCInterface interface;

static void SendOne()
{
	__R30 |= DCC_BIT;
	__delay_cycles(ONE);
	__R30 &= ~DCC_BIT;
	__delay_cycles(ONE);
}

static void SendZero()
{
	__R30 |= DCC_BIT;
	__delay_cycles(ZERO);
	__R30 &= ~DCC_BIT;
	__delay_cycles(ZERO);
}

void main(void)
{
	/* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    
    // Init structure
    memset(&interface, 0, sizeof(interface));
    interface.trackEnabled = false;

    // Add idle message 
    interface.records[0].type = Broadcast;
    interface.records[0].len = 3;
    interface.records[0].msg[0] = 0b11111111;
    interface.records[0].msg[1] = 0b00000000;
    interface.records[0].msg[2] = 0b11111111;

    for (;;) 
    {
        if ( interface.trackEnabled )
        {
            // Find next message
            SDCCRecord *r = interface.records+0;
            int len = r->len;


            // Preamble
            for ( int i = 0; i < 14; i++ )
            {
                SendOne();
            }
            for ( int i = 0; i < len; i++ )
            {
                uint8_t b = r->msg[i];

                // Start
                SendZero();

                // 8bit data
                for ( int bit = 0; bit < 8; bit++ )
                {
                    if ( b & 0x80 )
                        SendOne();
                    else
                        SendZero();
                    b <<= 1;
                }
            }
            // End
            SendOne();
        }


        __delay_cycles(PRU_CLK*0.1);// 0.1s
    }
}

