//
// Created by mobin on 25/01/24.
//

#ifndef LEFT_DF_H
#define LEFT_DF_H
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>

class DF {
public:
  DF(long int a);
  long long int set_key(int socket_number, sockaddr_in *server_addr);
  static std::string encryption(long long int key, std::string message);
  static std::string decryption(long long int key, std::string message);

private:
  long int a;
  long long int ask_p(int socket_number, sockaddr_in *server_addr);
  long long int ask_n(int socket_number, sockaddr_in *server_addr);
};

#endif // LEFT_DF_H
