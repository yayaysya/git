/***********************************
Version:2.0
Program: client.c
Content: 1.one server to offer many 
         clients communicate server.
		 2.add the function of sign 
		 and login.
Records: shaw song  10/08 afternoon
         shaw song  10/10 morning
***********************************/

#include "client.h"


int main(int argc, char ** argv)
{
	//init var
	int ClientFd;
	struct sockaddr_in ClientAddr;
	
	char MyName[20];
	memset(MyName, '\0', 20);
	char buffer[140];
	memset(buffer, '\0',140);
	char DestName[20];
	memset(DestName,'\0',20);
	char SendName[20];
	memset(SendName,'\0',20);
	char SendMsg[100];
	memset(SendName,'\0',100);
	
	//input client name
	//printf("Please input the Client Name\n");
	//scanf("%s", MyName);
	
	ClientFd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == ClientFd)
	{
		perror("Socket Create fail");
		exit(1);
	}
	
	//connect
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_port = htons(MyPort);
	ClientAddr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(ClientAddr.sin_zero, 0, 8);
	
	if (connect(ClientFd, (struct sockaddr*)&ClientAddr, sizeof(ClientAddr)) < 0)
	{
		perror("connect  error");
		return 0;
	}
        printf("connect success!\n");
	
	//Client Login
	if (Client_Login(ClientFd, MyName) == 0)
	{
		close(ClientFd);
		printf("Press any key to exit\n");
		char t ;
		scanf("%c",&t);
		exit(0);
	}
	
	while(1)
	{
		//confirm if a message can receive
	    fd_set FdSet;
	    FD_ZERO(&FdSet);
	    FD_SET(ClientFd, &FdSet);
	    int ret;
	    struct timeval tv;
	    tv.tv_sec=1;
	    tv.tv_usec=0;
            printf("\n-------%s Message Receive------\n", MyName);
	    while (1)
	    {
	        int tmp = select(ClientFd+1, &FdSet, NULL, NULL, &tv);
		    if(!FD_ISSET(ClientFd, &FdSet) || tmp == 0)
		    {
			    break;
		    }

		    ret = recv(ClientFd, buffer, sizeof(buffer), 0);
			if (ret <= 0)
			{
				perror("recover fail");
				close(ClientFd);
				exit(1);
			}
			
			split_string(buffer,DestName,SendName,SendMsg);
			printf(" %s:%s\n", SendName, SendMsg);
	    }	
            printf("-------------END---------------\n\n");
		
		
		/* function */
		int chs;
		printf(">>>>Please choose the number<<<<\n");
		printf("1.Sign a new user\n");
		printf("2.Talk to other Client\n");
		printf("3.Quit and exit\n");
		printf(">>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<\n");
		//scanf("%d",&chs);
		chs = ListenKeyboard();
		if (chs == 0)
			continue;
		if (chs == 1)
			Client_Sign(ClientFd);
		else if (chs == 2)
			Client_Talk(ClientFd, MyName);
		else if (chs == 3)
		{
			printf("Client Quit!\n");
			close(ClientFd);
			return 0;
		}
		
   }
	
}
