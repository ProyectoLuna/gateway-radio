/*
 * Gateway.cpp
 *
 *  Created on: Jul 18, 2017
 *      Author: migarna
 */

#include "Gateway.h"

Gateway::Gateway() {
	printf("Gateway constructor\n");
}

Gateway::~Gateway() {
	// TODO Auto-generated destructor stub
}

int Gateway::check_remotes(void) {
	radiorf24.check_remotes();
	return 0;
}
