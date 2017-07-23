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

using namespace std;

#include "DaemonLuna.h"

static void INThandler(int sig);
int gameover;

void INThandler(int sig)
{
	// Set inverse singleton variable
	gameover = 1;
}

DaemonLuna::DaemonLuna(const char* dname) {
	gameover = 0;
	stringstream str_stream;
	sact.sa_handler = INThandler;
	sact.sa_flags = 0;

	strncpy(name, dname, DAEMON_NAME_LEN);
	str_stream << "/var/run/" << name << "d.pid";
	strncpy(pidfilename, name, DAEMON_PIDFILE_LEN);

	// Register signal handler
	sigaction(SIGINT, &sact, NULL);
	sigaction(SIGKILL, &sact, NULL);
	sigaction(SIGTERM, &sact, NULL);

	cout << "DEBUG: daemon name: " << name << " pidfile: " << pidfilename << endl;
}

DaemonLuna::~DaemonLuna() {
	// TODO Auto-generated destructor stub
	finalize();
}

pid_t DaemonLuna::read_pid(void)
{
	int nsize = 16;
	char *data;
	pid_t pid;
	ifstream file(pidfilename);
	stringstream strvalue;

	if (!file.is_open()) {
		cout << "Error open file, " << strerror(errno);
		return 0;
	}

	file.read(data, nsize);
	file.close();

	strvalue << data;
	strvalue >> pid;

	cout << "Pid read: " << pid;

	return pid;
}

int DaemonLuna::write_pid(void)
{
	ofstream file(pidfilename);

	if (!file.is_open()) {
		cout << "Error open file, " << strerror(errno);
		return 0;
	}

	pid_t pid = getpid();

	file << pid;
	file.close();
	return 0;
}

int DaemonLuna::check_pid(void)
{
	pid_t pid = read_pid();

	// Daemon is already holding the pid file
	if ((!pid) || (pid == getpid()))
		return 0;

	return pid;
}

void DaemonLuna::daemonize(void)
{
	// Daemonize process
	if(daemon(0, 0))
		exit(0);
}

bool DaemonLuna::is_alive(void)
{
	return !gameover;
}

void DaemonLuna::finalize(void)
{
	unlink(pidfilename);
}
