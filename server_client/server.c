/************************************
Version:2.1
Author:Shaw Song
Program: server.c
Content: 1.one server to offer many 
         clients communicate server
		 2.add the function of sign
		 3.add the function of login
		 4.change the Userface 
Records: shaw song  10/08 morning
         shaw song  10/09 afternoon
		 shaw song  10/10 afternoon
************************************/
#include "server.h"


int main()
{
	int MaxFd=0;
	int ConnectAmount = 0;
	int ListenFd;
	int ClientFd;
	char Name[20];
	struct timeval tv;
	struct sockaddr_in ServerAddr, ClientAddr;
	fd_set FdSet;
	FdName fd_name_table[MAX_FD_NUM];

	
	//create ListenFd
	ListenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == ListenFd)
	{
		perror("socket create error :");
		exit(1);
	}
	
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(MyPort);
	ServerAddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(ListenFd, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) == -1)
	{
		perror("bind error:");
		exit(1);
	}
	
	if (listen(ListenFd, 10) == -1)
	{
		perror("listen error");
		exit(1);
	}
	
	
	//recv and send message
	//init 
	MaxFd = ListenFd;
	int i;

	for (i = 0; i < MAX_FD_NUM; i++)
	{
		fd_name_table[i].fd = 0;
		memset(fd_name_table[i].name, '\0', 20);
	}	
	
	while(1)
	{
	    FILE* file;
	    file = fopen(CHARTRECORD, "a+");
	    if (file < 0)
	    {
		    perror("open CharRecord fail!");
		    exit(1);
	    }
	    fseek(file, 0, SEEK_END);
		//select
		FD_ZERO(&FdSet);
		FD_SET(ListenFd, &FdSet);
		printf("--------------------------\n");
		printf("now online %d  clients:\n",ConnectAmount);
		for(i = 0; i < MAX_FD_NUM; i++)
		{
			if(fd_name_table[i].fd != 0)
			{
				printf("%s\n",fd_name_table[i].name);
				FD_SET(fd_name_table[i].fd, &FdSet);
				if (fd_name_table[i].fd > MaxFd)
				{
					MaxFd = fd_name_table[i].fd;
				}
			}
		}

		tv.tv_sec = 10;
		tv.tv_usec = 0;
		int ret;
		ret=select(MaxFd+1, &FdSet, NULL, NULL, &tv);
		if (ret < 0)
		{
			perror("select error");
			exit (1);
		}
		if (ret == 0)
		{
		//	printf("time out!\n");
			continue;
		}

		//connect to new client
		if(FD_ISSET(ListenFd, &FdSet))
		{

		    int sin_size=sizeof(ServerAddr);
			memset(Name, '\0', 20);
			ClientFd = accept(ListenFd, (struct sockaddr *)&ClientAddr, &sin_size);
		    printf("ClientFd:%d\n",ClientFd);
		    if (ClientFd < 0)
			{
				printf("can't accept a new ClientFd!\n");
				continue;
			}
			if (Server_Login(ClientFd, Name) == 0)
				close(ClientFd);
			else
			{
				for (i = 0; i < MAX_FD_NUM; i++)
			    {
					if(fd_name_table[i].fd == 0)
				    {
					fd_name_table[i].fd = ClientFd;
					memcpy(fd_name_table[i].name, Name,20);
					break;
				    }
			    }
			    ConnectAmount++;
			}
		}
		

		//recv and send
		for (i = 0; i < MAX_FD_NUM; i++)
		{
			if (FD_ISSET(fd_name_table[i].fd, &FdSet))
			{

				char buffer[140];
				memset(buffer, '\0', 140);
				int recv_ret;
				recv_ret = recv(fd_name_table[i].fd, buffer, sizeof(buffer), 0);
				if (recv_ret <= 0)
				{
					printf("%s offline\n",fd_name_table[i].name);
					close(fd_name_table[i].fd);
					fd_name_table[i].fd = 0;
					memset(fd_name_table[i].name, '\0',20);
					ConnectAmount--;
					FD_CLR(fd_name_table[i].fd, &FdSet);
					break;
				}
				//fputs(buffer,file);
				//fputs("\n",file);
				char SendName[20];
				char DestName[20];
				char SendMsg[100];
				memset(SendName,'\0',20);
				memset(DestName,'\0',20);
				memset(SendMsg,'\0',100);
				split_string(buffer, DestName, SendName, SendMsg);

			    printf("%s %s %s\n",DestName,SendName, SendMsg);	
				int flag = 0;
				int j = 0;
				for (j = 0;j < MAX_FD_NUM; j++)
				{
					if (memcmp(DestName,fd_name_table[j].name,20) == 0)
					{
				        	int DestTemp = fd_name_table[j].fd;
			          		send(DestTemp, buffer, sizeof(buffer),0);
							fputs(buffer,file);
							fputs("\n",file);
						    flag=1;
						    break;
					}
					else if (memcmp(DestName, "SIGN", 4) == 0)
					{
						Server_Sign(SendName, SendMsg);
						flag = 1;
						break;
					}
				}
				if (!flag)
				{
					printf("can't find the %s!\n",DestName);
				}
				else 
				{
				    printf("send to the %s success!\n",DestName);	
				}		
			}
		}
		fclose(file);
	}
	
}
