/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/UDPSocket.h>
#include <iostream>

using namespace std;
using namespace arv;

void printHelp() {
    cout << "Command options :" << endl;
    cout <<" -s : to create a server." << endl;
    cout <<" -c : to create a client." << endl;
    }

// data format used for requests from client to server :
struct DataQuestion {
    enum Op { ADD, MULT, STOP };
    DataQuestion() {}
    DataQuestion(Op op, int arg1, int arg2) : op(op), arg1(arg1), arg2(arg2) {}
    
    friend ostream& operator <<(ostream &out, const DataQuestion &obj) {
        switch (obj.op) {
            case ADD  : return out << "ADD "  << obj.arg1 << " " << obj.arg2;
            case MULT : return out << "MULT " << obj.arg1 << " " << obj.arg2;
            case STOP : return out << "STOP";
            default : return out;
            }
        }
    
    Op op;
    int arg1;
    int arg2;
    };

// data format used for answers from server to client :
struct DataAnswer {
    enum Type { RESULT, OK };

    friend ostream& operator <<(ostream &out, const DataAnswer &obj) {
        switch (obj.type) {
            case RESULT : return out << "RESULT = "  << obj.value;
            case OK     : return out << "OK";
            default : return out;
            }
        }
    
    Type type;
    int value;
    };

int main(int argc, char* argv[]) {

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
        cout << "- test UDP socket interface : client ..." << endl;

        UDPSocket *socket = new UDPSocket(2345);
        socket->init();

        DataQuestion listData[3];
        listData[0] = DataQuestion(DataQuestion::ADD,  2, 2);
        listData[1] = DataQuestion(DataQuestion::MULT, 3, 7);
        listData[2] = DataQuestion(DataQuestion::STOP, 0, 0);
        
        for (int i=0; i<3; i++) {
            DataQuestion data = listData[i];
            socket->send((void*)(&data), sizeof(data), "localhost", 3456);
            cout << "- question sent to server : " << data << endl;

            DataAnswer answer;
            int length = sizeof(answer);
            char serverName[255];
            int port;
            
            cout << "- waiting for answer from server ..." << endl;
            socket->receive((void*)&answer, length, serverName, port);
            cout << "> answer received from : " << serverName << ":" << port << " --> " << answer << endl;
            }

        socket->close();
        } 
    else {
        cout << "- test UDP socket interface : server ..." << endl;
        
        UDPSocket *socket = new UDPSocket(3456);
        socket->init();
        
        while (true) {
            DataQuestion question;
            int length = sizeof(question);
            char clientName[255];
            int port;
            cout << "- waiting for questions from client ..." << endl;
            socket->receive((void*)&question, length, clientName, port);
            cout << "> received question from : " << clientName << ":" << port << " --> " << question << endl;

            DataAnswer answer;
            switch (question.op) {
                case DataQuestion::ADD  : answer.type = DataAnswer::RESULT; answer.value = question.arg1 + question.arg2; break;
                case DataQuestion::MULT : answer.type = DataAnswer::RESULT; answer.value = question.arg1 * question.arg2; break;
                case DataQuestion::STOP : answer.type = DataAnswer::OK;     answer.value = -1;
                }
            
            socket->send((void*)&answer, sizeof(answer), clientName, port);
            cout << "- answer sent to client : " << answer << endl;
            
            // stop server because client asked for it :
            if (question.op == DataQuestion::STOP) break;
            }
        
        socket->close();
        }
    }

