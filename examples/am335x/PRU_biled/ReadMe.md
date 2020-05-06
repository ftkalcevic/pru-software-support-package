Simple LED flasher example, using shared memory to configure the flashing.  A biled (bipolar/bicolor led) with current limiting resistor is placed across output pins P9_27/P9_30.

Use "make" to compile the PRU.

Install with...

> sudo ./install_pru

The command line utility to control it is in ./cli

Build with make.

Run with...

> sudo ./cli 1 1000

The argments are 

mode - 0 off, 1 green flash, 2 is red flash, 3 is green/red flash

period - the flash period in ms.

