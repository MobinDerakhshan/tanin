#include "DFServer.h"
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main() {
  int socket_number = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_number == -1) {
    std::cout << "in cheshe?" << std::endl;
  }

  struct sockaddr_in cli_addr;
  memset(&cli_addr, 0, sizeof(cli_addr));
  unsigned int cli_addr_len = sizeof(cli_addr);

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  server_addr.sin_addr.s_addr = inet_addr("10.0.0.3");
  // bind SOCKET with sockaddr_in
  if (bind(socket_number, (const struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    std::cout << "-bind-" << std::endl;
    exit(EXIT_FAILURE);
  }
  // DF
  DFServer dfServer(101, 11, 23);

  std::string message;
  char recmessage[1024];
  while (true) {
    int sizeof_recvmessage =
        recvfrom(socket_number, recmessage, 1024, MSG_WAITALL,
                 (struct sockaddr *)&cli_addr, &cli_addr_len);
    recmessage[sizeof_recvmessage] = '\0';
    message = recmessage;

    if (message.substr(0, 5) == "dfc> ") {
      dfServer.df_req_handler(message, socket_number, cli_addr);
    } else {
      message = dfServer.decryption(cli_addr, message);
      std::cout << "|||||||||||||||||||||||||||||||||" << std::endl;
      std::cout << " : " << message << std::endl;

      message = message + " ke chi?";
      std::cout << "->" << message << std::endl;
      message = dfServer.encryption(cli_addr, message);
      sendto(socket_number, message.c_str(), message.size(), MSG_CONFIRM,
             (const struct sockaddr *)&cli_addr, cli_addr_len);
    }
  }
}
