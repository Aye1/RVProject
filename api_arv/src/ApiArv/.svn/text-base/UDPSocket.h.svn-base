/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_UDP_SOCKET__
#define __ARV_UDP_SOCKET__

namespace arv {
    
    /** UDP socket implementation.
     * A typical implementation of a UDP client using this class is:
     @code
     *  UDPSocket socket(2345);
     *  if (!socket.init()) error();
     *  while (communicating) {
     *      // send question :
     *      //----------------
     *          DataA data = ... ;
     *          socket.send((void*)(&data), sizeof(data), "localhost", 3456);
     *          cout << "- question sent to server : " << data << endl;
     *  
     *      // receive answer :
     *      //-----------------
     *          DataB answer;
     *          int length = sizeof(answer);
     *          char serverName[255];
     *          int port;
     *          cout << "- waiting for answer from server ..." << endl;
     *          socket.receive((void*)&answer, length, serverName, port);
     *          cout << "> answer received from : " << serverName << ":" << port << " --> " << answer << endl;
     *      }
     *  socket.close();
     @endcode
     * A typical implementation of a UDP server using this class is:
     @code
     *  UDPSocket socket(3456);
     *  if (!socket.init()) error();
     *  while (communicating) {
     *      // receive question :
     *      //-------------------
     *          DataB question;
     *          int length = sizeof(question);
     *          char clientName[255];
     *          int port;
     *          cout << "- waiting for questions from client ..." << endl;
     *          socket.receive((void*)&question, length, clientName, port);
     *          cout << "> received question from : " << clientName << ":" << port << " --> " << question << endl;
     *  
     *      // send answer :
     *      //--------------
     *          DataA answer = ... ;    // process question to make answer
     *          socket.send((void*)&answer, sizeof(answer), clientName, port);
     *          cout << "- answer sent to client : " << answer << endl;
     *      }
     *  socket.close();
     @endcode
     */
    class UDPSocket {
        public:
            UDPSocket(int port);
                /**< Creates a new UDP socket associated on the specified port.
                 @param port the port used by this UDP socket.
                 */
                
            bool init();
                /**< Needs to be called before any transmission.
                 @return \c true on success, \c false if some error occured.
                 */
                
            bool send(const void* data, int length, const char *serverName="localhost", int port=6666);
                /**< Sends data to another UDP socket.
                 @param data the data that needs to be send.
                 @param length number of bytes of \p data to send.
                 @param serverName name of the distant server, \c "localhost" for the local machine.
                 @param port the port of the socket on the distant server.
                 @return \c true on success, \c false if some error occured.
                 */
                
            bool receive(void *data, int& length, char *serverName, int& port);
                /**< Receives data from another UDP socket.
                 @param data pointer to memory where received data will be copied.
                 @param length to put the length (in bytes) of received data
                 @param serverName name of the distant server, \c "localhost" for the local machine.
                 @param port the port of the socket on the distant server.
                 @return \c true on success, \c false if some error occured.
                 */
                
            bool close();
                /**< Closes this UDP socket. */
            
        private:
            int port;
            int socket;
        };
    
    }

#endif

