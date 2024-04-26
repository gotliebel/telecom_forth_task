#ifndef TELECOM_FORTH_TASK__SEND_H_
#define TELECOM_FORTH_TASK__SEND_H_
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "struct.h"

void tx_send(uint8_t **buffer, ssize_t *len);

#endif