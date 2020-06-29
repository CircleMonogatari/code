#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
 



char* data = "GET /transactionlist HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/78.0.3904.97 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nSec-Fetch-Site: none\r\nSec-Fetch-Mode: navigate\r\nAccept-Encoding: gzip, deflate, br\r\nAccept-Language: zh-CN,zh;q=0.9\r\nCookie: Goland-d21d92b4=0befd0f4-0bb9-4cd5-93f4-78e962eb6be9\r\n\r\n";


int main(){
  int clientSocket;
  char buffer[4096];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
 
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(80801);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
 
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
 
  send(clientSocket,data,strlen(data),0);

  recv(clientSocket, buffer, 4096, 0);
 
  printf("Data received: -------------------------------\n%s\n----------------------------------\n",buffer);   
 
  return 0;
}
