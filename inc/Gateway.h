/*
 * Gateway.h
 *
 *  Created on: Jul 18, 2017
 *      Author: migarna
 */

#ifndef GATEWAY_H_
#define GATEWAY_H_

#include "RadioRF24.h"

class Gateway {
private:
	RadioRF24 radiorf24;
public:
	Gateway();
	virtual ~Gateway();
	int start(void);
};

#endif /* GATEWAY_H_ */
