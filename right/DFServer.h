//
// Created by mobin on 25/01/24.
//

#ifndef RIGHT_DFSERVER_H
#define RIGHT_DFSERVER_H
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <map>
#include <netinet/in.h>
#include <sys/socket.h>
class DFServer {
public:
  DFServer(long long int p, long long int n, long long int a);
  void df_req_handler(std::string message, int socket_number,
                      sockaddr_in cli_addr);
  std::string encryption(sockaddr_in cli_addr, std::string message);
  std::string decryption(sockaddr_in cli_addr, std::string message);

private:
  long long int p, n, a, first_key;
  std::map<in_addr_t, long long int> key;
};

#endif // RIGHT_DFSERVER_H
