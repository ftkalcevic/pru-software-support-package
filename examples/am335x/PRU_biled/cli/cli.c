//#include <prussdrv.h>
//#include <pruss_intc_mapping.h> 
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>


#define PRU_NUM 	0
#define PRU_ADDR 0x4A300000


struct SInterface
{
	uint32_t mode;		// 0-off, 1-red, 2-green, 3-red/green
	uint32_t period;	// ms
};

volatile struct SInterface *interface;



int main( int argc, char *argv[] )
{
	if ( argc != 3 )
	{
		fprintf( stderr, "Usage: cli mode period_ms\n");
		exit(0);
	}

	int mode = atoi(argv[1]);
	if ( mode < 0 )
		mode = 0;
	else if ( mode > 3 )
		mode = 3;

	int period = atoi(argv[2]);
	if ( period < 1 )
		period = 1;
	else if ( period > 20000)
		period = 20000;

	//prussdrv_init();
	//prussdrv_open( PRU_EVTOUT_0 );
	//prussdrv_pruintc_init(&pruss_intc_initdata);

	/* Get pointers to PRU local memory */
	//void *pruDataMem;
	//prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruDataMem);
	//interface = (struct SInterface *) pruDataMem;

	int fd = open("/dev/mem",O_RDWR | O_SYNC);
	fprintf(stderr, "mem open\n");
    	interface = (struct SInterface *)
        	(struct SInterface *) mmap(NULL, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 
                PRU_ADDR);
	fprintf(stderr, "got pointer %p\n",interface);



	interface->mode = mode;
	interface->period = period;

	fprintf(stderr, "done\n");
	//prussdrv_exit();
}


