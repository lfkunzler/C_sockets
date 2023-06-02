/*
 *  ./clent IP "send this message"
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void die_with_user_msg(const char* msg, const char* detail)
{
    fprintf(stderr, "%s: %s\n", msg, detail);
    exit(1);
}

void die_with_sys_msg(const char* msg)
{
    perror(msg);
    exit(2);
}

int str2int(const char* const str)
{
    int ret = 0;
    const char* aux = str;
    while (*aux != '\0') {
        if (*aux < '0' || *aux > '9') return 0;
        ret = ret*10 + *aux-'0';
        aux++;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int sock, ret_val;
    char *serverIP, *msg;
    unsigned short port;
    struct sockaddr_in serv_addr;

    if (argc < 3 || argc > 4) {
        die_with_user_msg("Parameters", "<server addr> <message> [<server port>]");
    }

    serverIP = argv[1];
    msg = argv[2];

    if (argc != 4 || !(port = str2int(argv[3]))) {
        port = 7; /* default port to echo server */
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        die_with_sys_msg("socket() failed");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; /* ipv4 address family */

    ret_val = inet_pton(AF_INET, serverIP, &serv_addr.sin_addr.s_addr);
    if (0 == ret_val) {
        die_with_user_msg("inet_pton() failed", "invalid_address string");
    }
    if (ret_val < 0) {
        die_with_sys_msg("inet_pton() failed");
    }

    serv_addr.sin_port = htons(port);

    return 0;
}
