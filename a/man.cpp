//
// Created by mobin on 19/01/24.
//

#include "man.h"

man::man(long long n_for_cli, long long p_for_cli, long long a) {
  this->n_for_cli = n_for_cli;
  this->p_for_cli = p_for_cli;
  this->a = a;
  this->first_key_for_cli =
      std::abs((long long int)pow(n_for_cli, a) % p_for_cli);
}

void man::set_server(addr server_addr) { servers.push_back(server_addr); }

pcpp::RawPacket *man::change_packet(u_char *packet,
                                    pcap_pkthdr *packet_header) {

  pcpp::RawPacket rawPacket(packet, packet_header->len, packet_header->ts,
                            false);
  pcpp::Packet packet1(&rawPacket);

  change_packet(&packet1);

  pcpp::RawPacket *changed = packet1.getRawPacket();
  return changed;
}

void man::change_packet(pcpp::Packet *packet) {

  if (packet->isPacketOfType(pcpp::UDP)) {
    from_to clientToServer = this->client_to_server(packet);
    std::string message = message_of_udp(packet);

    if (message.substr(0, 2) == "df") {
      df(packet);
    } else if (server_key.find(clientToServer) != server_key.end()) {
      encryption(packet);
    }
  }

  packet->computeCalculateFields();
}

void man::df(pcpp::Packet *packet) {

  from_to clientToServer = this->client_to_server(packet);
  std::string message = message_of_udp(packet);

  // fix message
  message = df(message, clientToServer);
  // make data form string
  const uint8_t *temp = reinterpret_cast<const uint8_t *>(message.c_str());
  // replace payload (data)
  pcpp::PayloadLayer *payloadLayer =
      packet->getLayerOfType<pcpp::PayloadLayer>();

  payloadLayer->setPayload(temp, message.size());
  payloadLayer->computeCalculateFields();
  packet->computeCalculateFields();
}

std::string man::df(std::string message, from_to clientToServer) {
  if (message.substr(0, 16) == "dfc> my key is: ") {
    std::string clientKeyString = message.substr(16);
    long long int clientKey = std::stoll(clientKeyString, nullptr, 10);
    client_key[clientToServer] = std::abs((long long int)pow(clientKey, a) % p_for_cli);

    long long int n = server_n[clientToServer];
    long long int p = server_p[clientToServer];
    long long int my_key = (long long int)pow(n, a) % p;
    message = message.substr(0, 16) + std::to_string(my_key);

  } else if (message.substr(0, 16) == "dfs> my key is: ") {
    std::string serverKeyString = message.substr(16);
    long long int serverKey = std::stoll(serverKeyString, nullptr, 10);
    long long int p = server_p[clientToServer];
    server_key[clientToServer] = abs((long long int)pow(serverKey, a) % p);

    message = message.substr(0, 16) + std::to_string(first_key_for_cli);

  } else if (message.substr(0, 8) == "dfs> p: ") {
    std::string serverPString = message.substr(8);
    long long int serverP = std::stoll(serverPString, nullptr, 10);
    server_p[clientToServer] = abs(serverP);

    message = message.substr(0, 8) + std::to_string(p_for_cli);

  } else if (message.substr(0, 8) == "dfs> n: ") {
    std::string serverNString = message.substr(8);
    long long int serverN = std::stoll(serverNString, nullptr, 10);
    server_n[clientToServer] = abs(serverN);

    message = message.substr(0, 8) + std::to_string(n_for_cli);

  }
  return message;
}

void man::encryption(pcpp::Packet *packet) {
  // find layer
  // find src/des addr

  from_to clientToServer = this->client_to_server(packet);
  std::string message = message_of_udp(packet);

  long long dec_key, enc_key;

  if (to_server(packet)) {
    dec_key = client_key[clientToServer];
    enc_key = server_key[clientToServer];
  } else {
    dec_key = server_key[clientToServer];
    enc_key = client_key[clientToServer];
  }

  std::cout << "----------------------------------- " << std::endl;
  message = decryption(dec_key, message);
  std::cout << clientToServer.first.toString() << " to "<<clientToServer.second.toString()<<" : " << message << std::endl;
  message = encryption(enc_key, message);

  // make data form string
  const uint8_t *temp = reinterpret_cast<const uint8_t *>(message.c_str());

  // replace payload (data)
  pcpp::PayloadLayer *payloadLayer =
      packet->getLayerOfType<pcpp::PayloadLayer>();
  payloadLayer->setPayload(temp, message.size());
  payloadLayer->computeCalculateFields();
  packet->computeCalculateFields();
}

std::string man::encryption(key enc_key, std::string message) {
  // find key size
  int temp = enc_key;
  int key_size = 0;
  while (temp > 0) {
    temp /= 10;
    key_size++;
  }
  // encryption ( key + string )
  for (int i = 0; i < message.size(); i++) {
    int j = i % key_size;
    int a = (enc_key / pow(10, j));
    int b = enc_key / pow(10, j + 1);
    message[i] += a - b * 10;
  }

  return message;
}

std::string man::decryption(key dec_key, std::string message) {
  // find key size
  int temp = dec_key;
  int key_size = 0;
  while (temp > 0) {
    temp /= 10;
    key_size++;
  }
  // decryption ( key - string )
  for (int i = 0; i < message.size(); i++) {
    int j = i % key_size;
    int a = (dec_key / pow(10, j));
    int b = dec_key / pow(10, j + 1);
    message[i] -= a - b * 10;
  }

  return message;
}

from_to man::client_to_server(pcpp::Packet *packet) {
  from_to cliToServer;

  pcpp::IPv4Layer *iPv4Layer = packet->getLayerOfType<pcpp::IPv4Layer>();
  pcpp::IPv4Address desIpv4 = iPv4Layer->getDstIPv4Address();
  pcpp::IPv4Address srcIpv4 = iPv4Layer->getSrcIPv4Address();

  if (std::find(servers.begin(), servers.end(), desIpv4) != servers.end()) {
    cliToServer = {srcIpv4, desIpv4};
  } else {
    cliToServer = {desIpv4, srcIpv4};
  }
  return cliToServer;
}

bool man::to_server(pcpp::Packet *packet) {
  bool toServer;

  pcpp::IPv4Layer *iPv4Layer = packet->getLayerOfType<pcpp::IPv4Layer>();
  pcpp::IPv4Address desIpv4 = iPv4Layer->getDstIPv4Address();
  pcpp::IPv4Address srcIpv4 = iPv4Layer->getSrcIPv4Address();

  if (std::find(servers.begin(), servers.end(), desIpv4) != servers.end()) {
    toServer = true;
  } else {
    toServer = false;
  }
  return toServer;
}

std::string man::message_of_udp(pcpp::Packet *packet) {
  if (packet->isPacketOfType(pcpp::UDP)) {
    pcpp::PayloadLayer *payloadLayer =
        packet->getLayerOfType<pcpp::PayloadLayer>();
    // payload to string
    uint8_t *tttt = payloadLayer->getPayload();
    int message_size = payloadLayer->getPayloadLen();

    char *tt = (char *)tttt;
    std::string message = tt;

    message = message.substr(0, message_size);

    return message;
  } else {
    std::cout << "dobsh in udp nist aslan";
    return "";
  }
}