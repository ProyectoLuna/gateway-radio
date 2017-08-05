/*
 * DaemonLuna.cpp
 *
 *  Created on: Jul 19, 2017
 *      Author: migarna
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>

using namespace std;

#include "DaemonLuna.h"

static void name_to_pidfile(const char* name, char* pidname)
{
	stringstream str_stream;
	str_stream << "/var/run/" << name << "d.pid";

	strncpy(pidname, (char*)str_stream.str().c_str(), DAEMON_PIDFILE_LEN);
}

DaemonLuna::DaemonLuna(const char* dname, void (*inthandler)(int)) {
	char pidfileaux[DAEMON_PIDFILE_LEN] = {};

	strncpy(name, dname, DAEMON_NAME_LEN);

	name_to_pidfile(name, pidfileaux);
	strncpy(pidfilename, pidfileaux, DAEMON_PIDFILE_LEN);
	sighandler = inthandler;

	cout << "DEBUG: daemon name: " << name << " pidfile: " << pidfilename << endl;
}

DaemonLuna::~DaemonLuna() {
	// TODO Auto-generated destructor stub
	finalize();
}

pid_t DaemonLuna::read_pid(const char* name)
{
	int nsize = 16;
	char *data;
	pid_t pid;
	char pidfileaux[DAEMON_PIDFILE_LEN];
	name_to_pidfile(name, pidfileaux);
	ifstream file(pidfileaux);
	stringstream strvalue;

	if (!file.is_open())
		return 0;

	file.read(data, nsize);
	file.close();

	strvalue << data;
	strvalue >> pid;

	return pid;
}

int DaemonLuna::write_pid(void)
{
	ofstream file(pidfilename);

	if (!file.is_open()) {
		cout << __func__ << " Error open file, " << strerror(errno) << endl;
		return 0;
	}

	pid_t pid = getpid();

	file << pid;
	file.close();
	return 0;
}

int DaemonLuna::check_pid(const char* name)
{
	pid_t pid = read_pid(name);

	// Daemon is already holding the pid file
	if ((!pid) || (pid == getpid()))
		return 0;

	return pid;
}

void DaemonLuna::daemonize(void)
{
	sact.sa_handler = sighandler;
	sact.sa_flags = 0;

	// Daemonize process
	if(daemon(0, 0))
		exit(0);

	// Register signal handler
	if (sigaction(SIGINT, &sact, NULL))
		cout << "sigaction error... SIGINT, " << strerror(errno) << endl;
	if (sigaction(SIGTERM, &sact, NULL))
		cout << "sigaction error... SIGTERM, " << strerror(errno) << endl;
}

void DaemonLuna::finalize(void)
{
	unlink(pidfilename);
}
