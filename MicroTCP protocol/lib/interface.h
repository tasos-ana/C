#ifndef LIB_INTERFACE_H_
#define LIB_INTERFACE_H_

#include "microtcp.h"
#include <stdint.h>

void convert_to_Network_order(microtcp_header_t * original);
void convert_to_Local_order(microtcp_header_t * original);

void init_header_packet(microtcp_header_t *header, uint16_t control, uint32_t seq_number, uint32_t ack_number);

int errorChecking(void *buf, size_t buf_size);

void init_socket(microtcp_sock_t *socket, char ID,
        const struct sockaddr *address, socklen_t address_len);

size_t min(int flow_control_win, int cwnd, int remaining);

int wait_packet(microtcp_sock_t *socket);

void set_timeout(int sock, suseconds_t tv_usec);

int send_segment(microtcp_sock_t *socket,const void *buffer, size_t data_len, size_t index);

#endif /* LIB_INTERFACE_H_ */