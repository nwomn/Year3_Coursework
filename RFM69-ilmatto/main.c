#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

//#include the library for the RFM69 module and the UART
#include "RFM69.h"
#include "RFM69registers.h"
#include "uart.h"
#include "millis.h"

#define NETWORKID 100
#define NODEID    144 //Modify this value, 0-255
#define MASTERID  144


int main(void)
{
	millis_init(); // Required for RFM69
	init_debug_uart0();
	rfm69_init(433, NODEID, NETWORKID);    //init the RFM69 
	setPowerLevel(31); //set to maximum transmit power
	promiscuous(1); //Receive from anyone on the network
	encrypt("1111111111111111");
	char testing[] = "Wom sending";

	while(1)
   	{ 
		// printf("Running...\n");
		// _delay_ms(1000);
		setMode(RF69_MODE_RX);
		receiveDone();
		if (!(millis()%1000)) {
			if (own_canSend_p0()) {
				send(MASTERID, (const void *)&testing, sizeof(testing));
				printf("message transmitted\n");
			}
		}
		// if (receiveDone()) {
		// 	printf("%s\n", (char*) DATA);
		// }
   	}
}
