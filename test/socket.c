#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>

#include <sys/select.h>
#include <sys/stat.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CLIENTS 5
#define PORT 8080

volatile sig_atomic_t flag = 1;

void handler(int) {
	flag = 0;
}

int main() {
	int server_socket;
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("server: socket: ");
		return 1;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);


	int opt = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
		perror("server: bind: ");
		return 1;
	}

	if (listen(server_socket, MAX_CLIENTS) == -1) {
		perror("server: listen: ");
		return 1;
	}

	signal(SIGINT, handler);

	while (flag) {

		struct sockaddr_in client_addr;
		socklen_t client_addr_len;
		int client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &client_addr_len);
		if (client_socket == -1) {
			perror("server: accept: ");
			return 1;
		}

		char request[1024] = "";
		recv(client_socket, request, sizeof(request), 0);

		printf("%s\n", request);

		//processing

		int fd = open("index.html", O_RDONLY);
		struct stat filestats;
		stat("index.html", &filestats);
		char *body = (char*) calloc(filestats.st_size, sizeof(char));
		read(fd, body, filestats.st_size);

		char response[] = "HTTP/1.1 200 OK\nServer: Custom HTTP server\nContent-Type: text/html\nContent-length: 72\n\n";

		size_t size = strlen(response) + strlen(body);
		char *final = (char*) calloc(size, sizeof(char));
		strcat(final, response);
		strcat(final, body);

		write(client_socket, final, size);

		shutdown(client_socket, SHUT_RDWR);
		close(client_socket);
	}

	fprintf(stdout, "\nServer is shutting down\n");
	close(server_socket);
	return 0;
}
