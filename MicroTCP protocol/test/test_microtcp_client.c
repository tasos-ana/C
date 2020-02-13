/*
 * test_microtcp_client.c
 *
 *  Created on: Oct 31, 2015
 *      Author: surligas
 */


/*
 * You can use this file to write a test microTCP client.
 * This file is already inserted at the build system.
 */

/* ----------------------------- */
/* Anastasas Anastasios  AM:3166 *
 *                               *
 * Giakoumhs Giwrgos     AM:3157 *a
/* ----------------------------- */
#include "../lib/microtcp.h"

int
main(int argc, char **argv) {
    microtcp_sock_t sock;

    if ((sock = microtcp_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)).sd == -1) {
        perror("opening TCP socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof (struct sockaddr_in));
    sin.sin_family = AF_INET;
    /*Port that server listens at */
    sin.sin_port = htons(6886);
    /* The server's IP*/
    sin.sin_addr.s_addr = inet_addr("147.52.19.43");

    if ((sock = microtcp_connect(sock, (struct sockaddr *) &sin, sizeof (struct sockaddr_in))).state != ESTABLISHED) {
        perror("microtcp connect");
        exit(EXIT_FAILURE);
    }
    sleep(1);
    printf("Connected to server!!\n");

    char buf[] = "Hello World";
    printf("%d\n", sizeof (buf));
    microtcp_send(&sock, buf, 10, 0);
    sock = microtcp_shutdown(sock, 0);

    return 0;
}