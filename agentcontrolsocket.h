#ifndef AGENTCONTROLSOCKET_H
#define AGENTCONTROLSOCKET_H

typedef struct {
	char *path;
	struct sockaddr_un *socket;
	int fd;
} agentsocket;

char *agent_control_socket_path(void);
agentsocket agent_control_socket_setup(void);
void agent_control_socket_bind(agentsocket ag);
void agent_control_socket_teardown(void);
void agent_control_socket_connect(void);
void assert_socket_sun_path(struct sockaddr_un *socket_address, char *path);

#endif
