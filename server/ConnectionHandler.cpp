#include "ConnectionHandler.h"
#include "../ZKP/DLP_ZKP.h"
#include "../ZKP/ECC_ZKP.h"
#include "../ECC/ecc.h"
#include "../common/common.h"
#include <cassert>
#include <sys/socket.h>
#include <errno.h>
#include <iostream>
#include <pthread.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <sys/types.h>   // For pid_t if it's not in unistd.h
#include <cerrno>        // For error numbers
#include <unistd.h>      // For close() and pid_t
#include <sys/syscall.h> // For syscall() and SYS_gettid


ConnectionHandler::ConnectionHandler(int fd) : fd(fd) {}


string ConnectionHandler::readMessage() {
    string msg(2048, '\0');    // buffer with 1024 length which is filled with NULL character
    
    int readBytes = recv(this->fd, msg.data(), msg.size(), 0);
    if (readBytes < 1) {
        cout << "Error in readMessage, readBytes: " << readBytes << endl;
        return "";
    }

    return msg;
}


void ConnectionHandler::sendMessage(const string& msg) {
    int n = send(this->fd, msg.c_str(), msg.size(), 0);
    if (n != static_cast<int>(msg.size())) {
        cout << "Error while sending message, message size: " << msg.size() << " bytes sent: " << endl;
    }
}

// Test for dlp
// void ConnectionHandler::handleConnection_dlp() {
//     pid_t tid = gettid();
//     cout << "Connection handled at thread ID: " << tid << endl;
//     this_thread::sleep_for(chrono::milliseconds(1));
    
//     Verifier_dlp verifier = Verifier_dlp();
//     verifier.gen_public_key();

//     json pub_key;
//     pub_key["g"] = serialize_ZZ(verifier.g);
//     pub_key["p"] = serialize_ZZ(verifier.p);
//     this->sendMessage(pub_key.dump());
//     cout << "Sent public key: " << pub_key << endl;
//     // receive a, y
//     auto msg = this->readMessage();
//     json recv_data = json::parse(msg);
//     verifier.a = unserialize_ZZ(recv_data["a"]);
//     verifier.y = unserialize_ZZ(recv_data["y"]);
//     verifier.e = generate_random_number(verifier.g, verifier.order);
//     json challenge;
//     challenge["e"] = serialize_ZZ(verifier.e);

//     //send e
//     this->sendMessage(challenge.dump());
//     //recv prove 
//     ZZ z = unserialize_ZZ(json::parse(this->readMessage())["prove"]);
//     //check prove
//     if(verifier.verify(z))
//         this->sendMessage("Authorized");
//     else{
//         this->sendMessage("Malicious");
//     }
//     shutdown(this->fd, SHUT_RDWR);
//     close(this->fd);
//     cout << "Terminated thread ID: "<< tid << endl;
// }


void ConnectionHandler::handleConnection_ecc() {
    pid_t tid = gettid();
    cout << "Connection handled at thread ID: " << tid << endl;
    this_thread::sleep_for(chrono::milliseconds(1));

    Verifier_ecc verifier = Verifier_ecc();
    json pub_parameter = serialize_EC(verifier.curve, verifier.generator, true);
    cout << pub_parameter << endl;
    this->sendMessage(pub_parameter.dump());
    auto msg = this->readMessage();
    json recv_data = json::parse(msg);
    verifier.A = ECPoint(verifier.curve, unserialize_ZZ(recv_data["A"]["x"]), unserialize_ZZ(recv_data["A"]["y"]));
    verifier.Y = ECPoint(verifier.curve, unserialize_ZZ(recv_data["Y"]["x"]), unserialize_ZZ(recv_data["Y"]["y"]));
    json challenge;

    challenge["e"] = serialize_ZZ(verifier.gen_challenge());
    this->sendMessage(challenge.dump());
    ZZ z = unserialize_ZZ(json::parse(this->readMessage())["prove"]);
    //check prove
    if(verifier.verify(z))
        this->sendMessage("Authorized");
    else{
        this->sendMessage("Malicious");
    }
    shutdown(this->fd, SHUT_RDWR);
    close(this->fd);
    cout << "Terminated thread ID: "<< tid << endl;
}
