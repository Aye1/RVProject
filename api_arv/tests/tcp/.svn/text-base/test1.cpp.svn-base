/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/TCPSocket.h>
#include <iostream>
#include <math.h>

using namespace std;
using namespace arv;

struct Msg {
    enum Type { SQRT, SIN, EXP, RESULT, STOP, OK, ERR };    
    
    friend ostream& operator <<(ostream &out, const Msg &obj) {
        switch (obj.type) {
            case SQRT   : return out << "SQRT "   << obj.arg;
            case SIN    : return out << "SIN "    << obj.arg;
            case EXP    : return out << "EXP "    << obj.arg;
            case RESULT : return out << "RESULT " << obj.arg;
            case STOP : return out << "STOP";
            case OK   : return out << "OK";
            case ERR  : return out << "ERR";
            default : return out;
            }
        }
    
    Msg(Type type=OK, float arg=0.0f) : type(type), arg(arg) {}
    
    Type type;
    float arg;
    };

void printHelp() {
    cout << "Command options :" << endl;
    cout <<" -s : to create a server." << endl;
    cout <<" -c : to create a client." << endl;
    }

int main(int argc, char *argv[]) {
    bool client = false;
    bool server = false;

    for (int i=1; i<argc; i++) {
        if (strcmp("-c", argv[i]) == 0)
            client = true;
        else if (strcmp("-s", argv[i]) == 0)
            server = true;
        else if (strcmp("-h", argv[i]) == 0)
            printHelp();
        else
            fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
        }

    if (server == client) {
        cout << "- error : you must choose if you want a client (-c) or a server (-s)." << endl;
        printHelp();
        return 1;
        }

    if (client) {
        cout << "- test TCP socket interface : client ..." << endl;

        TCPClient *client = TCPClient::create("localhost", 4568);
        cout << "> client created." << endl;

        cout << "- waiting for server to accept connection ..." << endl;
        client->connect();
        cout << "> server accepted connection." << endl;

        
        Msg request[5];
        request[0] = Msg(Msg::SIN, M_PI / 4.0f);
        request[1] = Msg(Msg::SQRT, 25.0f);
        request[2] = Msg(Msg::RESULT);
        request[3] = Msg(Msg::EXP, 1.0f);
        request[4] = Msg(Msg::STOP);
        
        for (int i=0; i<5; i++) {
            Msg data = request[i];
            client->send((void*)(&data), sizeof(data));
            cout << "- question sent to server : " << data << endl;

            Msg answer;
            int length = sizeof(answer);
            
            cout << "- waiting for answer from server ..." << endl;
            client->receive((void*)&answer, length);
            cout << "> answer received from server --> " << answer << endl;
            }

        client->close();
        }
    else {
        cout << "- test TCP socket interface : server ..." << endl;

        TCPServer *server = TCPServer::create(4568, 1);
        cout << "> server created." << endl;

        cout << "- accepting connection with clients ..." << endl;
        TCPClient *client = server->accept();
        cout << "> connection established with : " << client->address() << ":" << client->port() << endl;

        while (true) {
            Msg question;
            int length = sizeof(question);
            cout << "- waiting for questions from client ..." << endl;
            client->receive((void*)&question, length);
            cout << "> received question from client : " << question << endl;

            Msg answer;
            switch (question.type) {
                case Msg::SQRT : answer = Msg(Msg::RESULT, sqrtf(question.arg)); break;
                case Msg::SIN  : answer = Msg(Msg::RESULT, sinf(question.arg)); break;
                case Msg::EXP  : answer = Msg(Msg::RESULT, expf(question.arg)); break;
                case Msg::STOP : answer = Msg(Msg::OK); break;
                default :
                    cout << "> unexpected request from client !" << endl;
                    answer = Msg(Msg::ERR);
                }
            
            client->send((void*)&answer, sizeof(answer));
            cout << "- answer sent to client --> " << answer << endl;
            
            // stop server because client asked for it :
            if (question.type == Msg::STOP) break;
            }
        
        delete client;
        server->close();
        delete server;
        }
    }
