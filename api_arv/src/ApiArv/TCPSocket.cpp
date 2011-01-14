/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "TCPSocket.h"
#include <iostream>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

namespace arv {

/*********************************************************************************************/

#define ERROR(message) std::cerr << "- error in TCPSocket : " << message << "." << std::endl;
#define ERROR_RETURN(message, ret) { ERROR(message) return ret; }
#define ERROR_MSG(message) ERROR(message << " : msg = " << strerror(errno))
#define ERROR_MSG_RETURN(message, ret) { ERROR(message << " errno = " << strerror(errno)) return ret; }

/*********************************************************************************************/

class TCPSocket {
    public:
        // is server_name == NULL, local host assumed ( = "localhost")
        static TCPSocket* create(char *server_name, int port) {
            u_long addr = (server_name == NULL) ? htonl(INADDR_ANY) : inet_addr(server_name);       // INADDR_ANY = Address to accept any incoming messages.
        
            if (addr == INADDR_NONE) {
                // <server_name> is not a dotted IP, so resolve it through DNS :
                hostent* serverDescription = gethostbyname(server_name);
                if (serverDescription != NULL) addr = *((u_long*)serverDescription->h_addr_list[0]);
                }
            if (addr == INADDR_NONE)
                ERROR_RETURN("cannot find server", NULL)
        
            struct sockaddr_in address;
            address.sin_family      = AF_INET;           // AF_INET = Internet domain sockets for use with IPv4 addresses.
            address.sin_port        = htons(port);       // convert values between host and network byte order (<netinet/in.h>)
            address.sin_addr.s_addr = addr;
        
            // protocol : PF_INET (IPV4 protocol socket), type : full duplex TCP :
            int socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (socket == -1)
                ERROR_MSG_RETURN("cannot initialize TCP socket", NULL)
            
            return new TCPSocket(socket, address);
            }
        
        bool bind(bool reuse = true) const {
            int arg = reuse ? 1 : 0;
            // if arg == 1, allow socket to be reused if previously used :
            ::setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof arg);


            if (::bind(socket, (struct sockaddr *)&(address), sizeof(address)) == -1)
                ERROR_MSG_RETURN("cannot bind socket", false)
            return true;
            }
    
        char* addressName() const { return inet_ntoa(address.sin_addr); }
        int   port()        const { return ntohs(address.sin_port); }
        
        // to retrieve the adress froman existing socket :
        bool getAddress() {
            socklen_t addressLen = sizeof(address);
            if (getsockname(socket, (struct sockaddr *)&(address), &addressLen) == -1)
                ERROR_MSG_RETURN("cannot get socket address", false)
            return true;
            }
        
        bool close() {
            //::shutdown(socket, SHUT_RDWR);     //  (SHUT_WR) , SHUT_RD
            if (::close(socket) == -1) ERROR_MSG_RETURN("on socket close", false)
            return true;
            }
        
        bool listen(int maxConnections) const {
            if (::listen(socket, maxConnections) == -1)
                ERROR_MSG_RETURN("cannot listen", false)
            return true;
            }

        bool connect() const {
            if (::connect(socket, (struct sockaddr*)&(address), sizeof(address)) == -1)
                ERROR_MSG_RETURN("cannot connect", false)
            return true;
            }
        
        TCPSocket* accept() const {
            struct sockaddr_in clientAddress;
            socklen_t addressLen = sizeof(clientAddress);
            int clientSocket = ::accept(this->socket, (struct sockaddr*) &clientAddress, &addressLen);
            if (clientSocket == -1)
                ERROR_MSG_RETURN("cannot accept", NULL)
            return new TCPSocket(clientSocket, clientAddress);
            }
    
        bool send(const void *data, int length) {
            if (::send(socket, (const char *)data, length, 0) == -1)
                ERROR_RETURN("cannot send data", false)
            return true;
            }
        
        bool receive(void *data, int& length) {
            if (::recv(socket, (char *)data, length, 0) == -1)
                ERROR_RETURN("cannot receive data", false)
            return true;
            }
    
    private:
        TCPSocket(int socket, struct sockaddr_in address) : socket(socket), address(address) {}
        
    public:
        int socket;
        struct sockaddr_in address;
    };

/*********************************************************************************************/
// TCPClient implementation :

TCPClient* TCPClient::create(char *serverName, int serverPort) {
    TCPSocket *socket = TCPSocket::create(serverName, serverPort);
    if (socket == NULL) return NULL;
    return new TCPClient(socket);
    }

TCPClient::~TCPClient() { delete socket; }

bool TCPClient::connect()                           { return socket->connect(); }
bool TCPClient::send(const void* data, int length)  { return socket->send(data, length); }
bool TCPClient::receive(void* data, int& length)    { return socket->receive(data, length); }
bool TCPClient::close()                             { return socket->close(); }

char* TCPClient::address() const { return socket->addressName(); }
int   TCPClient::port()    const { return socket->port(); }


/*********************************************************************************************/
// TCPServer implementation :

TCPServer* TCPServer::create(int port, int maxConnections) {
    TCPSocket *socket = TCPSocket::create(NULL, port);
    if (socket == NULL) return NULL;
    // bind and allow server to resuse a previously used socket :
    if (!socket->bind(true)) return NULL;
    if (!socket->listen(maxConnections)) return NULL;
    return new TCPServer(socket);
    }

TCPServer::~TCPServer() {
    delete socket;
    }

bool TCPServer::close() {
    return socket->close();
    }

TCPClient* TCPServer::accept() {
    TCPSocket *client = socket->accept();
    return (client == NULL) ? NULL : new TCPClient(client);
    }

}

