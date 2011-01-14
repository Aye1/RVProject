/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_TCP_SOCKET__
#define __ARV_TCP_SOCKET__

namespace arv {

    class TCPSocket;    // association of a socket and an address
    
    /** @brief Implementation of a TCP client.
     * This class implements a TCP client able to connect to a server instantiated with class \c TCPServer.
     * A typical usage for this class is:
     @code
     *  TCPClient *client = TCPClient::create("localhost", 4567);
     *  if (client == NULL) error();
     *  // try to connect to server :
     *  if (!client->connect()) error();
     *  // connection is established.
     *
     *  while (communicating) {
     *      // send request to server :
     *      DataA request = ... ;
     *      client->send((void*)&request, sizeof(request));
     *  
     *      // receive answer from server :
     *      DataB answer;
     *      int length = sizeof(answer);
     *      client->receive((void*)&answer, length);
     *      // here <answer> contains <length> bytes of received data.
     *      }
     *  client->close();
     @endcode
     */
    class TCPClient {
        public:
            static TCPClient* create(char *serverName="localhost", int serverPort=6666);
                /**< Creates a TCP client for connection on a specified server.
                 @param serverName name of the server to connect to, \c "localhost" for local machine.
                 @param serverPort port on the server to connect to.
                 @return the newly created client on success or \c NULL if some error occured.
                 */
            ~TCPClient();
                /**< Destroys the object.
                 * The \c close() method needs to be called before.
                 */
            
            bool connect();
                /**< Tries to connect to the server.
                 @return \c true on success, \c false if some error occured.
                 */
            bool send(const void* data, int length);
                /**< Sends data to server.
                 @param data a pointer to the data that needs to be sent
                 @param length the size (in bytes) of the data
                 @return \c true on success, \c false if some error occured.
                 */
            bool receive(void* data, int& length);
                /**< Receives data from server.
                 @param data a pointer to a place in memory where received data will be put.
                 @param length will be set to the size (in bytes) of received data.
                 @return \c true on success, \c false if some error occured.
                 */
            bool close();
                /**< Closes the client.
                 * Must be called before destroying this object.
                 @return \c true on success, \c false if some error occured.
                 */
    
            char* address() const;
                /**< Gets the name of the server machine.
                 @return the name of the server machine.
                 */
            int   port()    const;
                /**< Gets the port number of the server.
                 @return the port number of the server.
                 */
            
        private:
            TCPClient(TCPSocket *socket) : socket(socket) {}
            TCPSocket *socket;
            friend class TCPServer;
        };
    
    /** @brief Implementation of a TCP server.
     * This class implements a TCP server able to receive connections from clients instantiated with class \c TCPClient.
     * A typical usage for this class is:
     @code
     *  TCPServer *server = TCPServer::create(4567, 1);
     *  if (server == NULL) error();
     *  TCPClient *client = server->accept();
     *  if (client == NULL) error();
     *  // connection is established.
     *  
     *  while (communicating) {
     *      // receive request from client :
     *      DataB request;
     *      int length = sizeof(request);
     *      client->receive((void*)&request, length);
     *      // here <request> contains <length> bytes of received data.
     *  
     *      // send answer to client :
     *      DataA answer = ... ;    // process request to create answer
     *      client->send((void*)&answer, sizeof(answer));
     *      }
     *  server->close();
     @endcode
     */
    class TCPServer {
        public:
            static TCPServer* create(int port=6666, int maxConnections=1);
                /**< Creates a TCP server receiving connections on the specified port.
                 @param port the port receiving connections to this server.
                 @param maxConnections the maximum number of connections on this server.
                 @return the newly created server or \c NULL if some error occured.
                 */
            ~TCPServer();
                /**< Destroys the object.
                 * The \c close() method needs to be called before.
                 */
            
            TCPClient* accept();
                /**< Waits for connection with a client.
                 @return the first connecting client of \c NULL if some error occured.
                 */
            bool close();
                /**< Closes the server.
                 * Must be called before destroying this object.
                 @return \c true on success, \c false if some error occured.
                 */
            
        private:
            TCPServer(TCPSocket *socket) : socket(socket) {}
            TCPSocket *socket;
        };
    
    }

#endif

