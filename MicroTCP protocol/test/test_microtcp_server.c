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
 * test_microtcp_server.c
 *
 *  Created on: Oct 31, 2015
 *      Author: surligas
 */

/*
 * You can use this file to write a test microTCP server.
 * This file is already inserted at the build system.
 */

/* ----------------------------- */
/* Anastasas Anastasios  AM:3166 *
 *                               *
 * Giakoumhs Giwrgos     AM:3157 *
/* ----------------------------- */
#include "../lib/microtcp.h"

int
main(int argc, char **argv) {
    microtcp_sock_t sock;

    struct sockaddr_in sin;

    struct sockaddr client_addr;
    socklen_t client_addr_len;

    if ((sock = microtcp_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)).sd == -1) {
        perror("opening TCP socket");
        exit(EXIT_FAILURE);
    }

    memset(&sin, 0, sizeof (struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(6886);
    /* Bind to all available network interfaces */
    sin.sin_addr.s_addr = INADDR_ANY;

    if (microtcp_bind(sock, (struct sockaddr *) &sin, sizeof (struct sockaddr_in)) == -1) {
        perror("TCP bind");
        exit(EXIT_FAILURE);
    }

    /* Ok, a tricky part here. See man accept() for details */

    client_addr_len = sizeof (struct sockaddr);
    if ((sock = microtcp_accept(sock, &client_addr, client_addr_len)).state == ESTABLISHED) {
        printf("New connection accepted!\n");
    }

    char buf[512];
    int received = 0;
    while (received != -1) {
        received = microtcp_recv(&sock, buf, 19, 0);
        if (received > 0) {
            buf[received] = 0;
            printf("\n");
            printf("%s\n", buf);
        }
    }
    sock = microtcp_shutdown(sock, 0);

    return 0;
}