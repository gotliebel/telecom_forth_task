#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "struct.h"

void handle(int socket) {
  char buffer[1024];
  struct sockaddr_in client_addr;
  int len = sizeof(client_addr);

  int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &client_addr,
                   (socklen_t * ) & len);
  if (n == -1) {
    printf("Error when receiving");
    exit(1)
  }

  struct RRCConnectionRequest *request =
      (struct RRCConnectionRequest *) buffer;

  struct RRCConnectionComplete complete;
  complete.protocolVersion = request->protocolVersion;
  complete.spare1 = request->spare1;
  complete.length = htons(sizeof(complete));
  complete.messageType = 2;
  complete.transactionID = request->transactionID;
  complete.ueID = request->ueID;
  complete.rrcStatus = 0;
  complete.spare2 = 0;

  if (sendto(sockfd, &complete, sizeof(complete), 0, (struct sockaddr *) &client_addr,
             (socklen_t) len) == -1) {
    printf("Error when sending");
    exit(1)
  }
}

volatile sig_atomic_t must_exit = 0;
volatile sig_atomic_t num = 0;

void handle_sigterm(int signum) {
  must_exit = 1;
  if (num == 0) {
    exit(0);
  }
}

int main() {
  struct sigaction action_term;
  memset(&action_term, 0, sizeof(action_term));
  action_term.sa_handler = handle_sigterm;
  action_term.sa_flags = SA_RESTART;
  sigaction(SIGTERM, &action_term, NULL);
  sigaction(SIGINT, &action_term, NULL);

  struct sockaddr_in server_addr = {.sin_family = AF_INET, .sin_port = htons(8888), .sin_addr.s_addr = INADDR_ANY};

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    printf("Error when opening socket: %d\n", errno);
    exit(1);
  }

  if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
    close(socket_fd);
    printf("Error when binding\n");
    exit(1);
  }
  if (listen(socket_fd, SOMAXCONN) == -1) {
    close(socket_fd);
    printf("Error when listening\n");
    exit(1);
  }

  while (true) {
    if (must_exit == 1) {
      break;
    }
    struct sockaddr_in client_addr;
    socklen_t len_client;
    int client_socket =
        accept(socket_fd, (struct sockaddr *) &client_addr, &len_client);
    if (client_socket == -1) {
      printf("Error when opening client socket\n");
      exit(1);
    }
    handle(client_socket);
  }

  close(socket_fd);
  close(client_socket);
  return 0;
}