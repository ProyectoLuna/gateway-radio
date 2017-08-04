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
private:
	char name[DAEMON_NAME_LEN];
	char pidfilename[DAEMON_NAME_LEN];
	struct sigaction sact;

public:
	DaemonLuna(const char *dname, void (*inthandler)(int));
	virtual ~DaemonLuna();
	void int_handler(int sig);
	static pid_t read_pid(const char* name);
	int write_pid(void);
	static int check_pid(const char* name);
	void daemonize(void);
	void finalize(void);
};

#endif /* DAEMONLUNA_H_ */
