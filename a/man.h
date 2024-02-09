//
// Created by mobin on 19/01/24.
//

#ifndef PISH_MAN_IN_THE_MIDDLE_MAN_H
#define PISH_MAN_IN_THE_MIDDLE_MAN_H
#include <bits/stdc++.h>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pcap/pcap.h>
#include <pcapplusplus/Packet.h>
#include "pcapplusplus/UdpLayer.h"
#include "pcapplusplus/IPv4Layer.h"
#include "pcapplusplus/PayloadLayer.h"

#define key long long int
#define addr pcpp::IPv4Address
#define from_to std::pair<pcpp::IPv4Address, pcpp::IPv4Address>


class man {
public:
    man(long long int n_for_cli, long long int p_for_cli, long long int a);
    void set_server(addr server_addr);
    pcpp::RawPacket *change_packet(u_char *packet, pcap_pkthdr *packet_header);
    void df(pcpp::Packet *packet);
    void encryption(pcpp::Packet *packet);

private:
    long long int n_for_cli, p_for_cli, first_key_for_cli, a;
    std::map<std::pair<addr,addr> , key> server_p;
    std::map<std::pair<addr,addr> , key> server_n;
    std::map<std::pair<addr,addr> , key> server_key;
    std::map<std::pair<addr,addr> , key> client_key;
    std::vector<addr> servers;

    std::string encryption(key enc_key, std::string message);
    std::string decryption(key dec_key, std::string message);
    std::string df(std::string message, from_to clientToServer);
    void change_packet(pcpp::Packet *packet);
    from_to client_to_server(pcpp::Packet *packet);
    bool to_server(pcpp::Packet *packet);
    static std::string message_of_udp(pcpp::Packet *packet);

};


#endif //PISH_MAN_IN_THE_MIDDLE_MAN_H
