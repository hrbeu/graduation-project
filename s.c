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
#include<time.h>
//源节点 
main()
{
	int n,i=0,flag;
    srand((unsigned)time(NULL));
    flag=rand()%2;
	if(flag==0)
       n=rand()%5;
    else
      n=rand()%5+5; 	
switch(i==0){
          case (1):	
	                
	               while(i<n){
		                  printf("%d\n",i);
                              sleep(1);
                              i=i+1;
                              }
                              break;
           case (0):
                   while(1){
                            printf("%d\n",i);
                            sleep(1);
                            i=i+1;
                            }
                            }                                  
   struct sockaddr_in serverAddr;
   int clientSocket;
   char sendbuf[10];
  /*
     TCP/IP协议族，设置参数AF_INET
     流套接字类型为SOCK_STREAM
  */
   if((clientSocket=socket(AF_INET,SOCK_STREAM,0))<0)         
     {                                               
       perror("socket error");
       return -1;
      }
   serverAddr.sin_family=AF_INET;                                                //协议簇 
   serverAddr.sin_port=htons(5555);                                              //端口号 
   serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");                            //服务器地址 
   if(connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0)  
      {
        perror("connect");
        exit(1);
       }
   printf("connect with destination host...\n");
   //将格式化的数据写入字符串中   
   sprintf(sendbuf,"%d",n);       
    //调用sent函数,将 n的值传送给recv函数 
    send(clientSocket,sendbuf,sizeof(sendbuf),0);
    close(clientSocket);	 
}
