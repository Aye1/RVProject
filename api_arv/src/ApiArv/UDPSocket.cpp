/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "UDPSocket.h"
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <stdio.h>
//#include <unistd.h>
//#include <string.h>

namespace arv {

/*********************************************************************************************/

#define ERROR(message) std::cerr << "- error in UDPSocket : " << message << "." << std::endl;
#define ERROR_RETURN(message) { ERROR(message) return false; }

UDPSocket::UDPSocket(int port) : port(port) {
    socket = 0;
    }

bool UDPSocket::init() {
    struct sockaddr_in address;
    address.sin_family      = AF_INET;             // AF_INET = Internet domain sockets for use with IPv4 addresses.
    address.sin_port        = htons(port);         // convert values between host and network byte order (<netinet/in.h>)
    address.sin_addr.s_addr = htonl(INADDR_ANY);   // INADDR_ANY = Address to accept any incoming messages.

    // protocol : PF_INET (IPV4 protocol socket), type : full duplex UDP.
    socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socket != -1  && bind(socket, (struct sockaddr*) &(address), sizeof(address)) != -1)
        return true;
    
    ERROR("socket binding failed")
    return false;
    }

bool UDPSocket::close() {
    return ::shutdown(socket, SHUT_RDWR) != -1;     //  (SHUT_WR) , SHUT_RD
    }

bool UDPSocket::send(const void* data, int length, const char *server_name, int port) {
    in_addr_t remoteAddress = inet_addr(server_name);

    if (remoteAddress == INADDR_NONE) {
        // <server_name> is not a dotted IP, so resolve it through DNS :
        hostent* server_description = gethostbyname(server_name);
        if (server_description != NULL)
            remoteAddress = *((u_long*)server_description->h_addr_list[0]);
        if (remoteAddress == INADDR_NONE)
            ERROR_RETURN("cannot find server")
        }

    struct sockaddr_in distAddr;
    distAddr.sin_family       = AF_INET;
    distAddr.sin_port         = htons(port);
    distAddr.sin_addr.s_addr  = remoteAddress;

    if (sendto(socket, data, length, 0, (struct sockaddr*) &distAddr, sizeof(distAddr)) == -1)
        ERROR_RETURN("cannot send data")
    
    return true;
    }

bool UDPSocket::receive(void *data, int& length, char *server_name, int& port) {
    struct sockaddr_in distAddr;
    socklen_t distAddrLen = sizeof(distAddr);
    if (recvfrom(socket, data, length, 0, (struct sockaddr*) &distAddr, &distAddrLen) == -1)
        ERROR_RETURN("cannot receive data")

    strcpy(server_name, inet_ntoa(distAddr.sin_addr));
    port = ntohs(distAddr.sin_port);
    return true;
    }

}

