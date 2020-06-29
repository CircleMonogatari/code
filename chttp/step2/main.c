#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
 

char* textFileRead(char* filename)
{
  char* text;
  FILE *pf = fopen(filename,"r");
  fseek(pf,0,SEEK_END);
  long lSize = ftell(pf);
  // 用完后需要将内存free掉
  text=(char*)malloc(lSize+1);
  rewind(pf); 
  fread(text,sizeof(char),lSize,pf);
  text[lSize] = '\0';
  return text;
}



int main(){
  int clientSocket;
  char buffer[4096];
  char *data;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
 
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = inet_addr("121.37.236.234");
 
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
 
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
 

  data = textFileRead("./request.txt");
  printf("---%s---\n", data);
  send(clientSocket,data,strlen(data),0);
  free(data);
  recv(clientSocket, buffer, 4096, 0);
 
  printf("Data received: -------------------------------\n%s\n----------------------------------\n",buffer);   
 
  return 0;
}
