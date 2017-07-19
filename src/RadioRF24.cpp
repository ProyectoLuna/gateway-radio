/*
 * RadioRF24.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: migarna
 */

#include <RadioRF24.h>

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

RadioRF24::RadioRF24() {
//	RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
//	RF24Network network(radio);
//	RF24Mesh mesh(radio, network);

	// Set the nodeID to 0 for the master node
	mesh.setNodeID(0);

	// Connect to the mesh
	printf("start\n");
	mesh.begin();
	radio.printDetails();
}

RadioRF24::~RadioRF24() {
	// TODO Auto-generated destructor stub
}

int RadioRF24::start(void) {

	while (1) {

		// Call network.update as usual to keep the network updated
		mesh.update();

		// In addition, keep the 'DHCP service' running on the master node so addresses will
		// be assigned to the sensor nodes
		mesh.DHCP();

		// Check for incoming data from the sensors
		while (network.available()) {
			//printf("rcv\n");
			RF24NetworkHeader header;
			network.peek(header);

			uint32_t dat = 0;
			switch (header.type) {
			// Display the incoming millis() values from the sensor nodes
			case 'M':
				network.read(header, &dat, sizeof(dat));
				printf("Rcv %u from 0%o\n", dat, header.from_node);
				break;
			default:
				network.read(header, 0, 0);
				printf("Rcv bad type %d from 0%o\n", header.type,
						header.from_node);
				break;
			}
		}
		delay(2);
	}
	return 0;
}