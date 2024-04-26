#include <stdio.h>
#include <sys/types.h>


int main() {
  uint8_t *buffer;
  ssize_t len;
  tx_send(&buffer, &len);
  return 0;
}
