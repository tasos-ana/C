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
 * microtcp.c
 *
 *  Created on: Oct 25, 2015
 *      Author: surligas
 */

#include "microtcp.h"
#include "interface.h"
#include "../utils/crc32.h"
#include <assert.h>

microtcp_sock_t microtcp_socket(int domain, int type, int protocol) {
    microtcp_sock_t tcp_sock;
    if (type == SOCK_STREAM && protocol == IPPROTO_TCP) {
        if ((tcp_sock.sd = socket(domain, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
            tcp_sock.state = INVALID;
            return tcp_sock;
        }
        tcp_sock.state = UNKNOWN;
        return tcp_sock;
    }
    tcp_sock.state = INVALID;
    tcp_sock.sd = -1;
    return tcp_sock;
}

int microtcp_bind(microtcp_sock_t socket, const struct sockaddr *address, socklen_t address_len) {
    return bind(socket.sd, address, address_len);
}

microtcp_sock_t microtcp_accept(microtcp_sock_t socket, struct sockaddr *address, socklen_t address_len) {
    srand(time(NULL));
    microtcp_header_t *header = malloc(SIZE_H);
    microtcp_header_t *received_header = malloc(SIZE_H);

    assert(socket.state == UNKNOWN);

    int sock = socket.sd;
    int received, data_len;
    int sended;

    memset(header, 0, SIZE_H);
    memset(received_header, 0, SIZE_H);

    //perimenoume SYN
    if ((received = recvfrom(sock, received_header, SIZE_H, 0, address, &address_len)) < 0) {
        socket.state = INVALID;
        return socket;
    }

    set_timeout(socket.sd, MICROTCP_ACK_TIMEOUT_US * 2);

    init_socket(&socket, SERVER, address, address_len);
    if (errorChecking(received_header, SIZE_H) == 0) {
        socket.state = INVALID;
        return socket;
    }

    int N = received_header->seq_number;
    socket.ack_number = N + received_header->data_len + 1; // ack_number increase

    if (received_header->control == SYN) {
        init_header_packet(header, SYN_ACK, rand() % 100000, socket.ack_number);
        header->window = MICROTCP_WIN_SIZE;
        socket.seq_number = header->seq_number;
        socket.recv_win_size = received_header->window;
    } else {
        free(header);
        free(received_header);
        socket.state = INVALID;
        return socket;
    }

    data_len = header->data_len;

    header->checksum = crc32((uint8_t *) header, SIZE_H);
    convert_to_Network_order(header);

    sended = sendto(sock, header, SIZE_H, 0, address, address_len);
    socket.seq_number += data_len + 1;

    //exoume steilei SYN+ACK
    //perimenoume teleftaio ACK
    memset(received_header, 0, SIZE_H);
    if ((received = recvfrom(sock, received_header, SIZE_H, 0, address, &address_len)) == -1) {
        socket.state = INVALID;
        return socket;
    }
    if (errorChecking(received_header, SIZE_H) == 0) {
        socket.state = INVALID;
        return socket;
    }

    set_timeout(socket.sd, MICROTCP_ACK_TIMEOUT_US);

    N = received_header->seq_number;
    socket.ack_number = N + received_header->data_len + 1; // ack_number increase

    int M = received_header->ack_number;
    if (received_header->control == ACK) {
        if (socket.seq_number == M) {
            socket.state = ESTABLISHED;
            socket.recvbuf = malloc(MICROTCP_RECVBUF_LEN * sizeof (uint8_t));
            socket.recv_win_size = received_header->window;

            free(header);
            free(received_header);
            return socket;
        } else {
            free(header);
            free(received_header);
            socket.state = INVALID;
            return socket;
        }
    }
    return socket; //to socket pou pirame me state UNKNOWN
}

microtcp_sock_t microtcp_connect(microtcp_sock_t socket, const struct sockaddr *address, socklen_t address_len) {
    srand(time(NULL));
    int data_len;

    if (socket.state == ESTABLISHED) {
        return socket;
    }
    microtcp_header_t *header = malloc(SIZE_H);
    microtcp_header_t *received_header = malloc(SIZE_H);

    set_timeout(socket.sd, MICROTCP_ACK_TIMEOUT_US);

    //fill the fields
    init_header_packet(header, SYN, rand() % 1111111, 0);
    header->window = MICROTCP_WIN_SIZE;

    init_socket(&socket, CLIENT, address, address_len);
    socket.seq_number = header->seq_number;

    int sock = socket.sd;

    data_len = header->data_len;
    header->checksum = crc32((uint8_t *) header, SIZE_H);
    convert_to_Network_order(header);

    sendto(sock, header, SIZE_H, 0, socket.address, socket.address_len);
    socket.seq_number += data_len + 1; // seq_number increase
    //exoume steilei SYN

    struct sockaddr client_addr;
    socklen_t client_addr_len = sizeof (struct sockaddr);
    int received;

    memset(received_header, 0, SIZE_H);
    //perimenoume ACK+SYN
    if ((received = recvfrom(sock, received_header, SIZE_H, 0, &client_addr, &client_addr_len)) == -1) {
        socket.state = INVALID;
        return socket;
    }

    if (!errorChecking(received_header, SIZE_H)) {
        socket.state = INVALID;
        return socket;
    }

    int M = received_header->seq_number; //na to valoume kai sto struct sock
    socket.ack_number = M + received_header->data_len + 1; // ack_number increase
    int received_ack = received_header->ack_number;
    if (received_header->control == SYN_ACK) {
        if (received_ack == socket.seq_number) {
            socket.recv_win_size = received_header->window;
            init_header_packet(header, ACK, socket.seq_number, socket.ack_number);

            data_len = header->data_len;
            header->checksum = crc32((uint8_t *) header, SIZE_H);
            convert_to_Network_order(header);
            sendto(sock, header, SIZE_H, 0, address, address_len);
            socket.seq_number += data_len + 1; // seq_number increase
            //exoume steilei ACK
            socket.state = ESTABLISHED;
            socket.recvbuf = malloc(MICROTCP_RECVBUF_LEN * sizeof (uint8_t));
            socket.recv_win_size = received_header->window;

            free(header);
            free(received_header);
            return socket;
        }
    }
    socket.state = INVALID;
    free(header);
    free(received_header);
    return socket; //to socket pou pirame me state UNKNOWN
}

microtcp_sock_t microtcp_shutdown(microtcp_sock_t socket, int how) {
    int received, sended, data_len;
    microtcp_header_t *header = malloc(SIZE_H);
    int sock = socket.sd;

    memset(header, 0, SIZE_H);

    if ((socket.ID == SERVER && socket.state == ESTABLISHED) || socket.state == CLOSING_BY_HOST) {
        received = SYN;
        while (received != FIN_ACK) {
            received = wait_packet(&socket);
        } // pirame FIN_ACK

        init_header_packet(header, ACK, socket.seq_number, socket.ack_number);
        header->window = socket.curr_win_size; // TODO bale auto sto init_header

        data_len = header->data_len;
        header->checksum = crc32((uint8_t *) header, SIZE_H);
        convert_to_Network_order(header);

        // stelnoume ACK
        sended = sendto(sock, header, SIZE_H, 0, socket.address, socket.address_len);
        socket.seq_number += data_len + 1; // seq_number increase

        if (socket.ID == CLIENT) {
            socket.state = CLOSED;
            free(socket.recvbuf);
            close(socket.sd);
            free(header);
            return socket;
        }
        socket.state = CLOSING_BY_PEER;
    }
    memset(header, 0, SIZE_H);

    if ((socket.state == ESTABLISHED && socket.ID == CLIENT) || socket.state == CLOSING_BY_PEER) {
        //fill the fields
        init_header_packet(header, FIN_ACK, socket.seq_number, socket.ack_number);
        header->window = socket.curr_win_size;

        data_len = header->data_len;
        header->checksum = crc32((uint8_t *) header, SIZE_H);
        convert_to_Network_order(header);

        sended = sendto(sock, header, SIZE_H, 0, socket.address, socket.address_len);
        socket.seq_number += data_len + 1; // seq_number increase
        //steilame FIN_ACK

        received = SYN;
        while (received != ACK) {
            received = wait_packet(&socket);
        } // pirame ACK

        if (socket.ID == SERVER) {
            socket.state = CLOSED;
            free(socket.recvbuf);
            close(socket.sd);
            free(header);
            return socket;
        }
        socket.state = CLOSING_BY_HOST;
        free(header);
        socket = microtcp_shutdown(socket, how);
    }

    return socket;
}

ssize_t microtcp_recv(microtcp_sock_t *socket, void *buf, size_t len, int flags) {
    assert(socket->state == CLOSING_BY_PEER || socket->state == ESTABLISHED);
    int received, sended;
    int M, data_len;
    struct sockaddr address;
    socklen_t address_len = sizeof (struct sockaddr);
    microtcp_header_t *header = malloc(SIZE_H);
    microtcp_header_t *received_header = malloc(SIZE_H);
    uint8_t *tmp_buf = malloc(MAX_PACKET_SIZE);

    int sock = socket->sd;

    if (socket->state == CLOSING_BY_PEER && socket->buf_fill_level == 0) {
        free(header);
        free(received_header);
        return -1;
    }

    set_timeout(socket->sd, 80);

    if (socket->state == ESTABLISHED) {
        while (socket->buf_fill_level < len) { // prepei na laboume kai alla
            memset(tmp_buf, 0, MAX_PACKET_SIZE);
            memset(header, 0, SIZE_H);
            memset(received_header, 0, SIZE_H);
            // lambanoume paketo
            if ((received = recvfrom(sock, tmp_buf, MAX_PACKET_SIZE, 0, &address, &address_len)) < 0) {
                // egine timeout ara prowthoume sthn
                // efarmogh oti exoume mexri twra
                break;
            }
            assert(received >= SIZE_H);

            if (errorChecking(tmp_buf, received) == 0) {
                ;
                continue;
            }// einai swsto
            memmove(received_header, tmp_buf, SIZE_H);

            M = received_header->seq_number;

            if (M == socket->ack_number) { // an einai auto pou perimename
                socket->ack_number = M + received_header->data_len + 1; // ack_number increase

                // den prokeite pote na upairboume me ta data to window
                memmove(socket->recvbuf + socket->buf_fill_level, tmp_buf + SIZE_H, received_header->data_len);
                socket->curr_win_size -= received_header->data_len;
                socket->buf_fill_level += received_header->data_len;
            }

            init_header_packet(header, ACK, socket->seq_number, socket->ack_number);
            header->window = socket->curr_win_size;

            data_len = header->data_len;
            header->checksum = crc32((uint8_t *) header, SIZE_H);
            convert_to_Network_order(header);

            // stelnoume ACK
            sended = sendto(sock, header, SIZE_H, 0, &address, address_len);
            socket->seq_number += data_len + 1; // seq_number increase

            // an htan FIN-ACK allazoume to state
            // kai epistrefoume -1
            if (received_header->control == FIN_ACK) {
                assert(socket->ID == SERVER && received_header->data_len == 0);
                socket->state = CLOSING_BY_PEER;
                break;
            }
        }
        free(tmp_buf);
    }

    set_timeout(socket->sd, MICROTCP_ACK_TIMEOUT_US);

    if (socket->buf_fill_level <= len) { // den pirame alla paketa se mikro diasthma
        memmove(buf, socket->recvbuf, socket->buf_fill_level);
        memset(socket->recvbuf, 0, socket->buf_fill_level); // ton adeiazoume
        socket->curr_win_size += socket->buf_fill_level;
        sended = socket->buf_fill_level;
        socket->buf_fill_level = 0;
        return sended;
    } else {
        memmove(buf, socket->recvbuf, len);
        // afairoume auta pou dwsame sthn efarmogh
        memmove(socket->recvbuf, socket->recvbuf + len, socket->buf_fill_level - len);
        socket->curr_win_size += len;
        socket->buf_fill_level -= len;
        return len;
    }
}

ssize_t microtcp_send(microtcp_sock_t *socket, const void *buffer, size_t length, int flags) {
    uint32_t i, chunks, firstSeq_number, min_ack, last_seq, recv_ack, confirmed_segments = 0;
    size_t remaining, data_sent = 0, bytes_to_send, index, sended, received;
    firstSeq_number = socket->seq_number;
    //min_ack = 0;
    uint32_t data_len;
    struct sockaddr address;
    socklen_t address_len = sizeof (struct sockaddr);

    microtcp_header_t *header = malloc(SIZE_H);
    microtcp_header_t *received_header = malloc(SIZE_H);

    memset(header, 0, SIZE_H);
    memset(received_header, 0, SIZE_H);

    remaining = length;
    while (data_sent < length) {
        if (socket->cwnd <= socket->ssthresh) {
            socket->atSlowStart = 1;
        } else {
            socket->atSlowStart = 0;
        }
        last_seq = 0;
        index = data_sent;
        bytes_to_send = min(socket->recv_win_size, socket->cwnd, remaining);
        chunks = bytes_to_send / MICROTCP_MSS;

        min_ack = 0;
        /* Send chunks */
        for (i = 0; i < chunks; i++) { // tha stelei tosa paketa osa einai ta chunks
            data_len = MICROTCP_MSS;
            if ((sended = send_segment(socket, buffer, data_len, index)) < 0) {
                return sended;
            }

            if (min_ack == 0) {
                min_ack = socket->seq_number;
            }
            index += data_len;
        }
        /* Check if there is a semi-filled chunk */
        if (bytes_to_send % MICROTCP_MSS != 0) {
            chunks++;
            last_seq = socket->seq_number;

            data_len = bytes_to_send % MICROTCP_MSS;
            if ((sended = send_segment(socket, buffer, data_len, index)) < 0) {
                return sended;
            }

            if (min_ack == 0) {
                min_ack = socket->seq_number;
            }
            index += data_len;
        }

        while (socket->recv_win_size == 0) {
            assert(chunks == 0);
            // stelnoume ena adeio paketo
            if ((sended = send_segment(socket, NULL, 0, 0)) < 0) {
                return sended;
            }

            set_timeout(socket->sd, rand() % (MICROTCP_ACK_TIMEOUT_US - 1) + 1);
            if ((received = recvfrom(socket->sd, received_header, SIZE_H, 0, &address, &address_len)) == -1) {
                continue;
            }
            if (errorChecking(received_header, SIZE_H) == 0) {
                continue;
            }

            socket->ack_number = received_header->seq_number + received_header->data_len + 1;
            if (received_header->control == ACK) {
                if (received_header->ack_number == socket->seq_number) {
                    socket->recv_win_size = received_header->window;
                }
            }
        }
        set_timeout(socket->sd, MICROTCP_ACK_TIMEOUT_US);

        /* Get the ACKs */
        i = 0;
        while (i < chunks) {
            memset(received_header, 0, SIZE_H);

            if ((received = recvfrom(socket->sd, received_header, SIZE_H, 0, &address, &address_len)) < 0) {
                // timeout retransmit kai pame se slow start
                socket->ssthresh = socket->cwnd / 2;
                socket->cwnd = (MICROTCP_MSS < socket->ssthresh ? MICROTCP_MSS : socket->ssthresh);
                socket->atSlowStart = 1;
                break;
            }
            if (received != SIZE_H) continue;
            if (errorChecking(received_header, SIZE_H) == 0) {
                continue;
            }

            socket->ack_number = received_header->seq_number + received_header->data_len + 1;
            if (received_header->control != ACK) { // 0x01000
                assert(0); // TODO remove assert
                continue;
            }
            recv_ack = received_header->ack_number;
            int dACK = 0;

            socket->recv_win_size = received_header->window;
            if (min_ack == recv_ack) {
                dACK = 0;
                socket->dACK_counter = 0;

                if (min_ack == last_seq) { // REF se mia function me apo katw
                    min_ack = last_seq + bytes_to_send % MICROTCP_MSS + 1;
                } else {
                    min_ack += MICROTCP_MSS + 1;
                }
                if (socket->atSlowStart == 1) {
                    socket->cwnd += MICROTCP_MSS;
                }
                i++;
            } else if (min_ack < recv_ack) {
                dACK = 0;
                socket->dACK_counter = 0;
                assert(!(last_seq != 0) || (min_ack != last_seq + bytes_to_send % MICROTCP_MSS + 1));

                if (recv_ack == last_seq + bytes_to_send % MICROTCP_MSS + 1) {
                    assert(last_seq != 0);
                    min_ack = recv_ack;
                    break; // ta pire ola ta chunks
                }

                i = i + (recv_ack - min_ack) % MICROTCP_MSS;

                min_ack = recv_ack + MICROTCP_MSS + 1;
            } else {
                if (socket->dACK_counter == 0) {
                    dACK = recv_ack;
                } else if (socket->dACK_counter == 3) {
                    socket->dACK_counter == 0;
                    //retransmit, benoume se congestion avoidance?
                    socket->ssthresh = socket->cwnd / 2;
                    socket->cwnd = socket->cwnd / 2 + 1;

                    break;
                }
                socket->dACK_counter++;
                //i++;
            }
        }
        confirmed_segments += i;
        if (socket->atSlowStart == 0 && i == chunks) { // an pirame ola ta ACK
            socket->cwnd += MICROTCP_MSS;
        }
        if (i != chunks) { // kati pige straba (timeout h dup acks)
            remaining -= ((recv_ack - firstSeq_number - confirmed_segments) - data_sent);
            data_sent = recv_ack - firstSeq_number - confirmed_segments;
            socket->seq_number = recv_ack;
            continue;
        }

        remaining -= bytes_to_send;
        data_sent += bytes_to_send;
    }
    return data_sent;
}
