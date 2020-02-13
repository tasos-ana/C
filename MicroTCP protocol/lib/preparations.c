#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "interface.h"
#include "microtcp.h"
#include "../utils/crc32.h"

void convert_to_Network_order(microtcp_header_t * original) {
    original->seq_number = htonl(original->seq_number);
    original->ack_number = htonl(original->ack_number);
    original->control = htons(original->control);
    original->window = htons(original->window);
    original->data_len = htonl(original->data_len);
    original->future_use0 = htonl(original->future_use0);
    original->future_use1 = htonl(original->future_use2);
    original->future_use2 = htonl(original->future_use1);
    original->checksum = htonl(original->checksum);
}

void convert_to_Local_order(microtcp_header_t * original) {

    original->seq_number = ntohl(original->seq_number);
    original->ack_number = ntohl(original->ack_number);
    original->control = ntohs(original->control);
    original->window = ntohs(original->window);
    original->data_len = ntohl(original->data_len);
    original->future_use0 = ntohl(original->future_use0);
    original->future_use1 = ntohl(original->future_use2);
    original->future_use2 = ntohl(original->future_use1);
    original->checksum = ntohl(original->checksum);

}

void init_header_packet(microtcp_header_t *header, uint16_t control, uint32_t seq_number, uint32_t ack_number) {
    header->seq_number = seq_number;
    header->ack_number = ack_number;
    header->control = control;
    header->window = 0;
    header->data_len = 0;
    header->future_use0 = 0;
    header->future_use1 = 0;
    header->future_use2 = 0;
    header->checksum = 0;

}

void init_socket(microtcp_sock_t *socket, char ID,
        const struct sockaddr *address, socklen_t address_len) {

    socket->ID = ID;
    socket->address = address;
    socket->address_len = address_len;
    socket->init_win_size = MICROTCP_WIN_SIZE;
    socket->curr_win_size = socket->init_win_size;
    socket->dACK_counter = 0;
    socket->buf_fill_level = 0;
    socket->atSlowStart = 1;
    socket->cwnd = MICROTCP_INIT_CWND;
    socket->ssthresh = MICROTCP_INIT_SSTHRESH;
}

int errorChecking(void *buf, size_t buf_size) {
    microtcp_header_t *header = malloc(SIZE_H);
    memset(header, 0, SIZE_H);

    memmove(header, buf, SIZE_H);
    convert_to_Local_order(header);
    int cur_checksum = header->checksum;
    header->checksum = 0;
    memset(buf, 0, SIZE_H);
    memmove(buf, header, SIZE_H);

    int new_checksum = crc32((uint8_t *) buf, buf_size);
    free(header);
    if (cur_checksum != new_checksum) {
        return 0;
    }
    return 1;
}

size_t min(int flow_control_win, int cwnd, int remaining) {
    size_t min = cwnd;

    if (flow_control_win < min) {
        min = flow_control_win;
    }

    if (remaining < min) {
        min = remaining;
    }

    return min;
}

/* epistrefei to control tou prwtou swstou paketou pou pire,
 * an einai kapoio ACK elenxei an auto exei swsto ack number */
int wait_packet(microtcp_sock_t *socket) {
    struct sockaddr address;
    socklen_t address_len = sizeof (struct sockaddr);
    int received, M;
    microtcp_header_t *received_header = malloc(SIZE_H);

    int counter = 0;

    while (1) {
        counter++;
        assert(counter < 20);

        memset(received_header, 0, SIZE_H);
        if ((received = recvfrom(socket->sd, received_header, SIZE_H, 0, &address, &address_len)) == -1) {
            continue;
        }

        if (errorChecking(received_header, SIZE_H) == 0) {
            continue;
        }

        M = received_header->seq_number;
        socket->ack_number = M + received_header->data_len + 1; // ack_number increase

        int control = received_header->control;
        if (control == ACK || control == SYN_ACK || control == FIN_ACK) {
            if (socket->seq_number != received_header->ack_number) { // exei lathos ACK number
                continue; // to agnooume
            }
        }
        free(received_header);

        return control;
    }
}

void set_timeout(int sock, suseconds_t tv_usec) {
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = tv_usec;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof (struct timeval)) < 0) {
        perror("setsockopt");
    }
}

int send_segment(microtcp_sock_t *socket, const void *buffer, size_t data_len, size_t index) {
    assert((!(buffer == NULL) || (data_len == 0)) && (!(data_len == 0) || index == 0));
    /* buffer => data_len  &&  data_len => index */
    int sended;
    microtcp_header_t *header = malloc(SIZE_H);
    memset(header, 0, SIZE_H);

    init_header_packet(header, 0, socket->seq_number, 0);
    header->data_len = data_len;
    uint8_t *tmp_buf = malloc(SIZE_H + data_len);
    memset(tmp_buf, 0, SIZE_H + data_len);
    memmove(tmp_buf, header, SIZE_H);

    if (data_len != 0) {
        memmove(tmp_buf + SIZE_H, buffer + index, data_len);
    }

    header->checksum = crc32(tmp_buf, SIZE_H + data_len);
    convert_to_Network_order(header);
    memmove(tmp_buf, header, SIZE_H);

    do {
        if ((sended = sendto(socket->sd, tmp_buf, SIZE_H + data_len,
                0, socket->address, socket->address_len)) < 0) {
            return sended;
        }
    } while (sended != SIZE_H + data_len);

    socket->seq_number += data_len + 1;

    free(tmp_buf);
    free(header);
    tmp_buf = NULL;
}