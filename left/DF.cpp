//
// Created by mobin on 25/01/24.
//

#include "DF.h"

DF::DF(long int a) { this->a = a; }

long long DF::set_key(int socket_number, sockaddr_in *server_addr) {
  long long int key;
  // find my key
  long long int p = ask_p(socket_number, server_addr);
  long long int n = ask_n(socket_number, server_addr);
  long long int my_key = (long long int)pow(n, a) % p;
  my_key = abs(my_key);

  std::string message = "dfc> my key is: " + std::to_string(my_key);
  char recvmessage[1024];
  unsigned int sizeof_server_addr = sizeof(server_addr);
  // send
  sendto(socket_number, message.c_str(), message.size(), MSG_CONFIRM,
         (const struct sockaddr *)server_addr, sizeof(*server_addr));
  // receive
  int sizeof_recvmessage =
      recvfrom(socket_number, recvmessage, 1024, MSG_WAITALL,
               (struct sockaddr *)&server_addr, &sizeof_server_addr);
  recvmessage[sizeof_recvmessage] = '\0';
  message = recvmessage;
  // check recvmessage
  while (message.substr(0, 16) != "dfs> my key is: ") {
    std::cout << "SH" << std::endl;
    key = set_key(socket_number, server_addr);
    return key;
  }
  // find server key
  message = message.substr(16);
  long long int server_key = std::stoll(message, nullptr, 10);
  // find key
  key = abs((long long int)pow(server_key, a) % p);
  return key;
}

long long int DF::ask_p(int socket_number, sockaddr_in *server_addr) {
  long long int p;

  std::string message = "dfc> give me p";
  char recvmessage[1024];
  unsigned int sizeof_server_addr = sizeof(server_addr);
  // send
  sendto(socket_number, message.c_str(), message.size(), MSG_CONFIRM,
         (const struct sockaddr *)server_addr, sizeof(*server_addr));
  // receive
  int sizeof_recvmessage =
      recvfrom(socket_number, recvmessage, 1024, MSG_WAITALL,
               (struct sockaddr *)&server_addr, &sizeof_server_addr);
  recvmessage[sizeof_recvmessage] = '\0';
  message = recvmessage;
  // check recvmessage
  while (message.substr(0, 8) != "dfs> p: ") {
    std::cout << "SH" << std::endl;
    p = ask_p(socket_number, server_addr);
    return p;
  }
  // find p
  message = message.substr(8);
  p = std::stoll(message, nullptr, 10);
  return p;
}

long long int DF::ask_n(int socket_number, sockaddr_in *server_addr) {
  long long int n;

  std::string message = "dfc> give me n";
  char recvmessage[1024];
  unsigned int sizeof_server_addr = sizeof(server_addr);
  // send
  sendto(socket_number, message.c_str(), message.size(), MSG_CONFIRM,
         (const struct sockaddr *)server_addr, sizeof(*server_addr));
  // receive
  int sizeof_recvmessage =
      recvfrom(socket_number, recvmessage, 1024, MSG_WAITALL,
               (struct sockaddr *)&server_addr, &sizeof_server_addr);
  recvmessage[sizeof_recvmessage] = '\0';
  message = recvmessage;
  // check recvmessage
  while (message.substr(0, 8) != "dfs> n: ") {
    std::cout << "SH" << std::endl;
    n = ask_p(socket_number, server_addr);
    return n;
  }
  // find n
  message = message.substr(8);
  n = std::stoll(message, nullptr, 10);
  return n;
}

std::string DF::decryption(long long key, std::string message) {
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

std::string DF::encryption(long long key, std::string message) {
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
