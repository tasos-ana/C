/*
    microtcp, a lightweight implementation of TCP for teaching,
    academic purpose

    Copyright (C) 2015  Manolis Surligas <surligas@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * microtcp.h
 *
 *  Created on: Oct 25, 2015
 *      Author: surligas
 */

#ifndef LIB_MICROTCP_H_
#define LIB_MICROTCP_H_

#include <stdint.h>
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>

/*
 * Several useful constants
 */
#define MICROTCP_ACK_TIMEOUT_US 200000
#define MICROTCP_MSS 1400
#define MICROTCP_RECVBUF_LEN 8192
#define MICROTCP_WIN_SIZE MICROTCP_RECVBUF_LEN
#define MICROTCP_INIT_CWND (3 * MICROTCP_MSS)
#define MICROTCP_INIT_SSTHRESH MICROTCP_WIN_SIZE

/* our constants */
#define SIZE_H sizeof(microtcp_header_t)
#define MAX_PACKET_SIZE (MICROTCP_MSS + SIZE_H)
#define FIN 1
#define SYN 2
#define RST 4
#define ACK 8
#define FIN_ACK 9
#define SYN_ACK 10
#define CLIENT 'C'
#define SERVER 'S'

/**
 * Possible states of the microTCP socket
 *
 * NOTE: You can insert any other possible state
 * for your own convenience
 */
typedef enum {
    LISTEN,
    ESTABLISHED,
    CLOSING_BY_PEER,
    CLOSING_BY_HOST,
    CLOSED,
    INVALID,
    UNKNOWN
} microtcp_state_t;

/**
 * This is the microTCP socket structure. It holds all the necessary
 * information of each microTCP socket.
 *
 * NOTE: Fill free to insert additional fields.
 */
typedef struct {
    int sd; /**< The underline UDP socket descriptor */
    microtcp_state_t state; /**< The state of the microTCP socket */
    size_t init_win_size; /**< The window size negotiated at the 3-way handshake */
    size_t curr_win_size; /**< The current window size */

    uint8_t *recvbuf; /**< The *receive* buffer of the TCP
	 connection. It is allocated during the connection establishment and
	 is freed at the shutdown of the connection. This buffer is used
	 to retrieve the data from the network. */
    size_t buf_fill_level; /**< Amount of data in the buffer */

    size_t cwnd;
    size_t ssthresh;

    size_t seq_number; /**< Keep the state of the sequence number */
    size_t ack_number; /**< Keep the state of the ack number */

    const struct sockaddr *address;
    socklen_t address_len;

    char ID; // we are client or server?
    int dACK_counter; // counts duplicate acks
    int atSlowStart; // 1 if we are at slow start, 0 if not
    size_t recv_win_size;
} microtcp_sock_t;

/**
 * microTCP header structure
 */
typedef struct {
    uint32_t seq_number; /**< Sequence number */
    uint32_t ack_number; /**< ACK number */
    uint16_t control; /**< Control bits (e.g. SYN, ACK, FIN) */
    uint16_t window; /**< Window size in bytes */
    uint32_t data_len; /**< Data legth in bytes (EXCLUDING header) */
    uint32_t future_use0; /**< 32-bits for future use */
    uint32_t future_use1; /**< 32-bits for future use */
    uint32_t future_use2; /**< 32-bits for future use */
    uint32_t checksum; /**< CRC-32 checksum, see crc32() in utils folder */
} microtcp_header_t;


microtcp_sock_t
microtcp_socket(int domain, int type, int protocol);

int
microtcp_bind(microtcp_sock_t socket, const struct sockaddr *address,
        socklen_t address_len);

microtcp_sock_t
microtcp_connect(microtcp_sock_t socket, const struct sockaddr *address,
        socklen_t address_len);

microtcp_sock_t
microtcp_accept(microtcp_sock_t socket, struct sockaddr *address,
        socklen_t address_len);

microtcp_sock_t
microtcp_shutdown(microtcp_sock_t socket, int how);

ssize_t microtcp_recv(microtcp_sock_t *socket, void *buf, size_t len, int flags);

ssize_t microtcp_send(microtcp_sock_t *socket, const void *buffer, size_t length, int flags);

#endif /* LIB_MICROTCP_H_ */