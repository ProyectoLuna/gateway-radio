/** @mainpage proyecto-luna - None
 *
 * @author migarma <migarma2@gmail.com>
 * @version 0.0.1
**/

#include <iostream>
#include <poll.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <syslog.h>

using namespace std;

#include "Gateway.h"
#include "DaemonLuna.h"

#define DAEMON_GATEWAY_SOCKET_PATH_LEN 64
#define DAEMON_GATEWAY_API_SOCKET_TIMEOUT 100 // ms

int gameover = 0;
static void INThandler(int sig);
void INThandler(int sig)
{
	gameover = 1;
}

class DaemonGateway: public DaemonLuna, public Gateway {
public:
	// UNIX Socket
	int socketfd;
	struct sockaddr_un sock_addr;
	struct pollfd pollfd;
	char socket_path[DAEMON_GATEWAY_SOCKET_PATH_LEN];

	DaemonGateway(const char *dname, void (*inthandler)(int));
	virtual ~DaemonGateway();
	int handle_request();
};

DaemonGateway::DaemonGateway(const char *dname, void (*inthandler)(int)) : DaemonLuna(dname, inthandler), Gateway() {
	strncpy(socket_path, "/var/run/dgateway", DAEMON_GATEWAY_SOCKET_PATH_LEN);

	// UNIX socket
	if((socketfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
		return;

	// Fill addr structure
	memset(&sock_addr, 0x00, sizeof(struct sockaddr_un));
	sock_addr.sun_family = AF_UNIX;
	strcpy(sock_addr.sun_path, socket_path);

	if(bind(socketfd, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr_un))) {
		//syslog(0, "SECTORBOOT: Error in bind function (file %s). %s", SECTORBOOT_SOCKET_PATH, strerror(errno));
		return;
	}

	// Prepare timeout
	pollfd.fd		= socketfd;
	pollfd.events	= POLLIN;
	pollfd.revents	= 0x00;
}

DaemonGateway:: ~DaemonGateway() {
	finalize();
	unlink(socket_path);
	close(socketfd);
}

int DaemonGateway::handle_request(void)
{
	return 0;
}

int main(int argc, char** argv) {
	pid_t pid;
	const char* daemon_name = "gateway";

	if ((pid = DaemonLuna::check_pid(daemon_name))) {
		cout << "ERROR Gateway daemon already exists, pid: " << pid << endl;
		return -1;
	}

	DaemonGateway* gateway = new DaemonGateway(daemon_name, INThandler);

	gateway->daemonize();

	gateway->write_pid();

	int ret;
	while(!gameover) {
		// Polling
		ret = poll(&gateway->pollfd, 1, DAEMON_GATEWAY_API_SOCKET_TIMEOUT);

		// Check poll result
		if (ret > 0) {
			// Handle request from API
			gateway->handle_request();
		}else if (ret < 0) {
			continue;
		}

		gateway->check_remotes();
	}

	delete gateway;
	return 0;
}
