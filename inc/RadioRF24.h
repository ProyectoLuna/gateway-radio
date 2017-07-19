/*
 * RadioRF24.h
 *
 *  Created on: Jul 19, 2017
 *      Author: migarna
 */

#ifndef RADIORF24_H_
#define RADIORF24_H_

#include "Radio.h"

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>

class RadioRF24 {
private:
public:
	RadioRF24();
	virtual ~RadioRF24();
	int start(void);
};

#endif /* RADIORF24_H_ */
