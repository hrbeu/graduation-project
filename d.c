#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#define SERVER_PORT 5555

//Ŀ�Ľڵ� 
main() 
{
	//����recv�����������������ݣ� 

    int i=0,n;
    int serverSocket;
    struct sockaddr_in server_addr;
    struct sockaddr_in clientAddr;
    int addr_len=sizeof(clientAddr);
    int client;
    char buffer[10];
    int iDataNum;
     /*
     TCP/IPЭ���壬���ò���AF_INET
     ���׽�������ΪSOCK_STREAM
    */
    if((serverSocket=socket(AF_INET,SOCK_STREAM,0))<0)
      {
         perror("socket");
         exit(1);
       }
     
    bzero(&server_addr,sizeof(server_addr));  //�����ַ��� 
    server_addr.sin_family=AF_INET;             //Э��� 
    server_addr.sin_port=htons(SERVER_PORT);     //�˿ں� 
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);  //�������е�ַ 
    if(bind(serverSocket,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
      {
        perror("connect");
        exit(1);
       }
    if(listen(serverSocket,5)<0)
       {
         perror("listen");
         exit(1);
        }
     printf("\nstatus��waiting for link request\n");
   while(1){ client=accept(serverSocket,(struct sockaddr*)&clientAddr,(socklen_t*)&addr_len);
            if(client<0)
              {
                 perror("accept");
               }
             
             iDataNum=recv(client,buffer,10,0);
             if(iDataNum<0)
               {
                 perror("Recv");
                }
             printf("\nstatus��Recv client data...n\n");
             n=atoi(buffer);
             i=n;
             printf("Migrating process from  IP address�� %s\n",inet_ntoa(clientAddr.sin_addr));
             printf("Migrating process from  Port is %d\n",htons(clientAddr.sin_port));
      switch(i==0){
          case (1):	
	                
	               while(i<n){
		                  printf("%d\n",i);
                              sleep(1);
                              i=i+1;
                              }
                              break;
           case (0):
                   while(i<=20){
                            printf("%d\n",i);
                            sleep(1);
                            i=i+1;
                            }
                            }            
                            }
} 
