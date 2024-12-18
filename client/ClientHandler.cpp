#include "ClientHandler.h"
#include "../ZKP/ECC_ZKP.h"
#include "../ECC/ecc.h"
#include "../common/common.h"
#include <cassert>
#include <sys/socket.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>  
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ClientHandler::ClientHandler(int fd, chrono::time_point<chrono::steady_clock> start_time, fstream& waiting_time) : 
        fd(fd), start_time(start_time), process_id(process_id), waiting_time(waiting_time){}


// Test for dlp
// void ClientHandler::handleConnection_dlp(int num_workers) {
//     string msg = this->readMessage();
//     auto wait_duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - this->start_time).count();
//     this->waiting_time << num_workers << " " << wait_duration << endl;
//     json pub_key = json::parse(msg);
//     //recv pub_key
//     ZZ g = unserialize_ZZ(pub_key["g"]);
//     ZZ p = unserialize_ZZ(pub_key["p"]);
//     ZZ w = generate_random_number(g, p - 1); // rand between g and order
//     Prover prover = Prover(w, p ,g);

//     //send data
//     json send_data;
//     send_data["a"] = serialize_ZZ(prover.gen_param());
//     send_data["y"] = serialize_ZZ(prover.y);
//     this->sendMessage(send_data.dump());
//     ZZ e = unserialize_ZZ(json::parse(this->readMessage())["e"]);

//     //gererate prove
//     ZZ z = prover.gen_prove(e);
//     json prove;
//     prove["prove"] = serialize_ZZ(z);
//     this->sendMessage(prove.dump());
//     // cout << this->readMessage() << endl; //Check for output
//     fflush(stdout);
//     shutdown(this->fd, SHUT_RDWR);
//     close(this->fd);
//     exit(0);
// }

void ClientHandler::handleConnection_ecc(int num_workers) {
    string msg = this->readMessage();
    auto wait_duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - this->start_time).count();
    this->waiting_time << num_workers << " " << wait_duration << endl;
    vector<ZZ> pub_key = unserialize_EC(msg, true);
    ZZ p = pub_key[0];
    ZZ a = pub_key[1];
    ZZ b = pub_key[2];
    ZZ order = pub_key[3];
    EllipticCurve curve = EllipticCurve(p,a,b,order);
    ECPoint generator = ECPoint(curve, pub_key[4], pub_key[5]);
    cout << msg << endl;
    ZZ w = generate_random_number(ZZ(2), order);
    Prover_ecc prover = Prover_ecc(w, curve, order, generator);
    json send_data;
    send_data["A"] = serialize_ECPoint(prover.gen_param());
    send_data["Y"] = serialize_ECPoint(prover.Y);
    cout << send_data << endl;
    this->sendMessage(send_data.dump());
    ZZ e = unserialize_ZZ(json::parse(this->readMessage())["e"]);
    ZZ z = prover.gen_prove(e);
    json prove;
    prove["prove"] = serialize_ZZ(z);
    this->sendMessage(prove.dump());
    cout << this->readMessage() << endl; //Check for output
    fflush(stdout);
    shutdown(this->fd, SHUT_RDWR);
    close(this->fd);
    exit(0);
}

string ClientHandler::readMessage() {
    string msg(2048, '\0');    // buffer with 1024 length which is filled with NULL character      
    int readBytes = recv(this->fd, msg.data(), msg.size(), 0);
    if (readBytes < 1) {
        cout << "Error in readMessage, readBytes: " << readBytes << endl;
        return "";
    }

    return msg;
}


void ClientHandler::sendMessage(const string& msg) {
    int n = send(this->fd, msg.c_str(), msg.size(), 0);
    if (n != static_cast<int>(msg.size())) {
        cout << "Error while sending message, message size: " << msg.size() << " bytes sent: " << endl;
    }
}


