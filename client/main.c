#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../socketgc.h"

/**
 * main - entry point of the client application.
 * This function is responsible for instatiating
 * and maintaining a connection with the server,
 * takes in user input and communicates with the server.
 *
 * @argc: Represents the number of arguments passed
 * to the programs from the command line
 * @argv: Hold the argumnents passed to the programs
 * via the command line.
 *
 * Return: 0 on successful execution, otherwise it returns 1.
 *
*/
int main(int argc, char **argv)
{
	int fd, conn_result;
	struct sockaddr_in *address;
	char *line = NULL;
	size_t linesize = 0;
	ssize_t char_count, amount_sent;

	if (argc != 2) {
		printf("Usage: %s <username>\n", argv[0]);
		return EXIT_FAILURE;
	}

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("Unable to create socket");
		return (EXIT_FAILURE);
	}

	address = createipv4address(2000, "127.0.0.1");
	conn_result = connect(fd, (struct sockaddr *)address, sizeof(*address));
	if (conn_result != 0) {
		perror("Unable to connect to the server");
		close(fd);
		free(address);
		return (EXIT_FAILURE);
	}

	printf("Connected! Type a message and press enter to send, or type 'exit' to exit.\r\n");
	while (1) {
		char_count = getline(&line, &linesize, stdin);

		if (char_count < 0 || strcmp(line, "exit\n") == 0) {
			break;
		}

		line[char_count - 1] =  0;
		amount_sent = send(fd, line, char_count, 0);
	}

	close(fd);
	free(address);
	free(line);
	return (EXIT_SUCCESS);
}
