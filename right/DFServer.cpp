//
// Created by mobin on 25/01/24.
//

#include "DFServer.h"

DFServer::DFServer(long long p, long long n, long long a) {
  this->p = p;
  this->n = n;
  this->a = a;
  first_key = (long long int)pow(n, a) % p;
  first_key = abs(first_key);
}

void DFServer::df_req_handler(std::string message, int socket_number,
                              sockaddr_in cli_addr) {
  std::string answer;
  if (message == "dfc> give me p") {
    answer = "dfs> p: " + std::to_string(p);
  } else if (message == "dfc> give me n") {
    answer = "dfs> n: " + std::to_string(n);
  } else if (message.substr(0, 16) == "dfc> my key is: ") {
    answer = "dfs> my key is: " + std::to_string(first_key);
    message = message.substr(16);
    long long int clientKey = std::stoll(message, nullptr, 10);
    key[cli_addr.sin_addr.s_addr] = abs((long long int)pow(clientKey, a) % p);
  }

  sendto(socket_number, answer.c_str(), answer.size(), MSG_CONFIRM,
         (const struct sockaddr *)&cli_addr, sizeof(cli_addr));
}

std::string DFServer::encryption(sockaddr_in cli_addr, std::string message) {
  // key
  int key = this->key[cli_addr.sin_addr.s_addr];
  // find key size
  int temp = key;
  int key_size = 0;
  while (temp > 0) {
    temp /= 10;
    key_size++;
  }
  // encryption ( key + string )
  for (int i = 0; i < message.size(); i++) {
    int j = i % key_size;
    int a = (key / pow(10, j));
    int b = key / pow(10, j + 1);
    message[i] += a - b * 10;
  }
  return message;
}

std::string DFServer::decryption(sockaddr_in cli_addr, std::string message) {
  // key
  int key = this->key[cli_addr.sin_addr.s_addr];
  // find key size
  int temp = key;
  int key_size = 0;
  while (temp > 0) {
    temp /= 10;
    key_size++;
  }
  // decryption ( key - string )
  for (int i = 0; i < message.size(); i++) {
    int j = i % key_size;
    int a = (key / pow(10, j));
    int b = key / pow(10, j + 1);
    message[i] -= a - b * 10;
  }
  return message;
}