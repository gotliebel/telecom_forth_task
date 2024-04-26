#include "send.h"

void tx_send(uint8_t **buffer, ssize_t *len) {
  struct sockaddr_in servaddr = {
      .sin_family = AF_INET,
      .sin_port = htons(8080),
      .sin_addr.s_addr = inet_addr("192.168.0.1"),
  };

  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
  if (sockfd == -1) {
    printf("Error when opening socket\n");
    exit(1);
  }

  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1) {
    printf("Error when connecting socket\n");
    exit(1);
  }

  struct RRCConnectionRequest request;
  request.protocolVersion = 0;
  request.spare1 = 0;
  request.length = htons(sizeof(request));
  request.messageType = 1;
  request.transactionID = 1;
  request.ueID = 1;
  request.nasKeySetIdentifier = 1;
  request.spare2 = 0;
  request.rrcEstablishmentCause = 1;

  if (sendto(sockfd, &request, sizeof(request), 0, (struct sockaddr *) &servaddr,
             sizeof(servaddr)) == -1) {
    printf("Error when sending from client");
    exit(1);
  }

  close(sockfd);
}