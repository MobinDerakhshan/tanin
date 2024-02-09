#include <iostream>
#include <pcap.h>
#include <cstring>
#include "pcapplusplus/RawPacket.h"
#include "pcapplusplus/Packet.h"
#include "man.h"
int main(int argc, char *argv[]) {
    char errbuff[PCAP_ERRBUF_SIZE];
    char *dev1_name = argv[1], *dev2_name = argv[2];

    pcap_t *dev1 = pcap_open_live(dev1_name, BUFSIZ, 1, -1, errbuff);
    pcap_t *dev2 = pcap_open_live(dev2_name, BUFSIZ, 1, -1, errbuff);
    if (dev1 == NULL) {
        std::cout << errbuff << std::endl;
    }
    if (dev2 == NULL) {
        std::cout << errbuff << std::endl;
    }

    man man(31,43,11);
    man.set_server(pcpp::IPv4Address("10.0.0.3"));

    while (true) {
        const u_char *packet;
        struct pcap_pkthdr *packet_header;

        if (pcap_next_ex(dev1, &packet_header, &packet) == 1) {
            pcpp::RawPacket *replyPacket = man.change_packet((u_char *) packet, packet_header);

            pcap_sendpacket(dev2, replyPacket->getRawData(), replyPacket->getRawDataLen());
        }
        if (pcap_next_ex(dev2, &packet_header, &packet) == 1) {
            pcpp::RawPacket *replyPacket = man.change_packet((u_char *) packet, packet_header);

            pcap_sendpacket(dev1, replyPacket->getRawData(), replyPacket->getRawDataLen());
        }
    }

}