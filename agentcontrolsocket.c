#include "agentcontrolsocket.h"
#include "config.h"
#include "password.h"
#include "process.h"
#include "signal.h"
#include "util.h"
#include "util.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/un.h>
#include <unistd.h>
#if (defined(__unix__) || defined(unix)) && !defined(USG)
#include <sys/param.h>
#endif

#if !defined(SUN_LEN)
#define SUN_LEN(su) \
        (sizeof(*(su)) - sizeof((su)->sun_path) + strlen((su)->sun_path))
#endif

char *agent_control_socket_path(void)
{
	return config_path("agent.sock");
}

void assert_socket_sun_path(struct sockaddr_un *socket_address, char *path)
{
	if (strlen(path) >= sizeof(socket_address->sun_path)) {
		die("Path too large for agent control socket.");
	}

}

agentsocket _init_agent_socket(struct sockaddr_un *socket_address)
{
	agentsocket agent_socket;
	agent_socket.path = socket_address->sun_path;
	agent_socket.socket = socket_address;
	agent_socket.fd = socket(AF_UNIX, SOCK_STREAM, 0);
	return agent_socket;
}

void _init_sockaddr_un(struct sockaddr_un *socket_address, char *path)
{
	assert_socket_sun_path(socket_address, path);
	socket_address->sun_family = AF_UNIX;
	strlcpy(socket_address->sun_path, path, sizeof(socket_address->sun_path));
}

agentsocket agent_control_socket_setup()
{
	struct sockaddr_un socket_address;

	memset(&socket_address, 0, sizeof(socket_address));
	_init_sockaddr_un(&socket_address, agent_control_socket_path());

	return _init_agent_socket(&socket_address);
}

void agent_control_socket_bind(agentsocket agent_socket)
{
	int rv;
	int listen_fd;

	printf("Binding nieuwe style\n");
	rv = bind(agent_socket.fd, (struct sockaddr *)&agent_socket.socket, SUN_LEN(agent_socket.socket));
	if (rv < 0) {
		if (listen(agent_socket.fd, 16) < 0) {
			listen_fd = errno;
			close(agent_socket.fd);
			unlink(agent_socket.path);
			errno = listen_fd;
			die_errno("bind|listen");
		}
	}
}

