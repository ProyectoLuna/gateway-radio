/*
 * Gateway.cpp
 *
 *  Created on: Jul 18, 2017
 *      Author: migarna
 */

#include "Gateway.h"

Gateway::Gateway() {
	radiorf24 = RadioRF24();
}

Gateway::~Gateway() {
	// TODO Auto-generated destructor stub
}

int Gateway::start(void) {
	radiorf24.start();
	return 0;
}
