#ifndef TELECOM_FORTH_TASK__STRUCT_H_
#define TELECOM_FORTH_TASK__STRUCT_H_

struct RRCConnectionRequest {
  uint8_t protocolVersion;
  uint8_t spare1;
  uint16_t length;
  uint8_t messageType;
  uint8_t transactionID;
  uint16_t ueID;
  uint8_t nasKeySetIdentifier;
  uint8_t spare2;
  uint16_t rrcEstablishmentCause;
};

struct RRCConnectionComplete {
  uint8_t protocolVersion;
  uint8_t spare1;
  uint16_t length;
  uint8_t messageType;
  uint8_t transactionID;
  uint16_t ueID;
  uint8_t rrcStatus;
  uint8_t spare2;
};

#endif
