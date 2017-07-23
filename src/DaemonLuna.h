/*
 * DaemonLuna.h
 *
 *  Created on: Jul 19, 2017
 *      Author: migarna
 */

#ifndef DAEMONLUNA_H_
#define DAEMONLUNA_H_

#include <signal.h>
#include <sys/socket.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <cstdbool>

#define DAEMON_NAME_LEN 	32
#define DAEMON_PIDFILE_LEN	64

class DaemonLuna {
public:
	char name[DAEMON_NAME_LEN];
	char pidfilename[DAEMON_NAME_LEN];
	struct sigaction sact;

	DaemonLuna(const char *dname);
	virtual ~DaemonLuna();
	void int_handler(int sig);
	pid_t read_pid(void);
	int write_pid(void);
	int check_pid(void);
	void daemonize(void);
	bool is_alive(void);
	void finalize(void);
};

#endif /* DAEMONLUNA_H_ */
