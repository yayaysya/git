/***********************************
Version:2.0
Program: client.h
Content: 1.one server to offer many 
         clients communicate server.
		 2.add the function of sign 
		 and login.
Records: shaw song  10/08 afternoon
         shaw song  10/10 morning
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MyPort 8888

/********************************
Function Name:split_string
Function:split the string buffer
         buffer:xxxxx#xxxx#xxxxx@
Parameter: char* p is the buffer
           char* Dest Send Msg is
		   the DestName SenderName
		   and the Message Sended
*********************************/
void split_string(char* p, char* Dest, char* Send, char* Msg)
{
	while(*p != '#')
		*Dest++ = *p++;
		p++;
	while(*p != '#')
		*Send++ = *p++;
		p++;
	while(*p != '@')
		*Msg++ = *p++;
}

/*********************************
Function Name:Client_Login
Function:Send the UserName and Passwd
         to Login
Parameter: ClientFd is the fd of Client
Return: type:int
        1 means Login Success
		0 means Login Fail
***********************************/
int Client_Login(int ClientFd, char *myname)
{
	char UserName[20];
	char Passwd[20];
	memset(UserName, '\0', 20);
	memset(Passwd, '\0', 20);
	
	printf("Please input your name : ");
	scanf("%s", UserName);
	memcpy(myname, UserName, 20);
	printf("Please input your password : ");
	scanf("%s", Passwd);
	
	//Send
	char SendBuf[50];
	memcpy(SendBuf, UserName, 20);
	strcat(SendBuf,"#");
	strcat(SendBuf, Passwd);
	strcat(SendBuf, "@");
	
	send(ClientFd, SendBuf, 50, 0);
	
	//recv
	char ans[5];
	int ret;
	ret = recv(ClientFd, ans, 5, 0);
	if (ret <= 0)
		return 0;
	if (memcmp(ans, "LOGI", 4) == 0)
	{
		printf("Login in Success!\n");
		return 1;
	}
	else
	{
		printf("Login Fail!!\nWrong username or passwd\n");
		return 0;
	}
}

/****************************************
Function Name: Client_Sign
Function:sign a new user
Parameter: ClientFd
*****************************************/
void Client_Sign(int ClientFd)
{
	char UserName[20];
	char PassWord[20];
	char SendBuf[50];
	memset(UserName, '\0', 20);
	memset(PassWord, '\0', 20);
	memset(SendBuf, '\0', 50);
	
	printf("*****Sign a new user*****\n");
	printf("Please input new Username: ");
	scanf("%s", UserName);
	printf("Please input the password: ");
	scanf("%s", PassWord);
	printf("**************************\n");
	
	memcpy(SendBuf, "SIGN#", 6);
	strcat(SendBuf, UserName);
	strcat(SendBuf, "#");
	strcat(SendBuf, PassWord);
	strcat(SendBuf, "@");
	
	send(ClientFd, SendBuf, 50, 0);
	
}
/****************************************
Function Name: Client_Talk
Function:talk to other Client
Parameter: ClientFd
*****************************************/
void Client_Talk(int ClientFd, char *MyName)
{
		char SendName[20];
		char DestName[20];
		char SendMsg[100];
		char buffer[140];
		memset(SendName, '\0', 20);
		memset(DestName, '\0', 20);
		memset(SendMsg, '\0', 100);
		memset(buffer, '\0', 140); char ans='y';
		printf("Send Message\n");
		if(ans == 'y')
		{
			printf("Please input the Dest Client Name:");
			scanf("%s", DestName);
			printf("Please input the Message:");
		        scanf("%s", SendMsg);
			
			strcpy(buffer,DestName);
			strcat(buffer, "#");
			strcat(buffer,MyName);
			strcat(buffer, "#");
			strcat(buffer,SendMsg);
			strcat(buffer, "@");
			
			int ret = send(ClientFd, buffer, sizeof(buffer), 0);
			if (ret > 0)
			{
				printf("Message is sended!\n");
			}
			else
				printf("fail to send message!\n");
		}
}

/*****************************************
Function Name:ListenKeyboard
Function:listen teh keyboard ,if there is a 
         input then return
*****************************************/
int ListenKeyboard()
{
	fd_set key;
	FD_ZERO(&key);
	FD_SET(0, &key);
	int KeyRet;
	int buf;
	struct timeval KeyTv;
	KeyTv.tv_sec =15;
	KeyTv.tv_usec = 0;
	KeyRet = select(1, &key, NULL, NULL, &KeyTv);
	if(KeyRet == 0)
	{
		system("clear");
		return 0;
	}
	else if(KeyRet > 0);
	{
		scanf("%d",&buf);
		return buf;
	}
	printf("wrong input!\n");
}













