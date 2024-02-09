#include "DF.h"
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main() {
  int socket_number = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_number == -1) {
    std::cout << "in cheshe?" << std::endl;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  server_addr.sin_addr.s_addr = inet_addr("10.0.0.3");
  unsigned int server_addr_len = sizeof(server_addr);

  int DF_a = 43;
  DF df(DF_a);
  long long int key = df.set_key(socket_number, &server_addr);

  char recvmessage[1024];
  std::string message = "salam";
  std::cout << "->" << message << std::endl;

  message = DF::encryption(key, message);
  sendto(socket_number, message.c_str(), message.size(), 0,
         (const struct sockaddr *)&server_addr, sizeof(server_addr));

  int sizeof_recvmessage =
      recvfrom(socket_number, recvmessage, 1024, MSG_WAITALL,
               (struct sockaddr *)&server_addr, &server_addr_len);
  recvmessage[sizeof_recvmessage] = '\0';
  message = recvmessage;

  message = DF::decryption(key, message);
  std::cout << " : " << message << std::endl;
}
