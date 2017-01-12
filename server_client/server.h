/************************************
Version:2.0
Author:Shaw Song
Program: server.h
Content: 1.one server to offer many 
         clients communicate server
		 2.add the function of sign
		 3.add the function of login
Records: shaw song  10/08 morning
         shaw song  10/09 afternoon
************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAX_FD_NUM  100
#define MyPort 8888
#define USERFILE "/tmp/UserList"
#define CHARTRECORD "/tmp/ChartRecord"


/**************************
Name:FdName
Function: relate the fd and 
          Client name
***************************/
typedef struct
{
	int fd;
	char name[20];
} FdName;

/********************************
Function Name:split_string
Function:split the string buffer
         buffer:xxxxx#xxxx#xxxxx@
Parameter: char* p is the buffer
           char* Dest Send Msg is
		   the DestName SenderName
		   and the Message Sended
*********************************/

void split_string(char* p, char* Dest, char *Send, char* Msg)
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

/********************************
Function Name:Server_Login
Function:confirm the UserName and 
         Passwd
Parameter: ClientFd is the fd of 
           the new Client
		   username is the name of 
		   user
Return: type:int
             1 means Login Success
		     0 means Login Fail
*********************************/
int Server_Login(int ClientFd, char* username)
{
	int ret;
	char RecvBuf[50];
	memset(RecvBuf, '\0', 50);
	ret = recv(ClientFd, RecvBuf, 50, 0);
	if (ret <= 0)
	{
		printf("Recv UserName and PassWord Fail!\n");
		return 0;
	}
	//init Name
	char* p = NULL;
	p = RecvBuf;
	while(*p != '#')
		*username++ = *p++;
	//confirm
	FILE* LoginFile;
	LoginFile = fopen (USERFILE, "a+");
  	if (fgetc(LoginFile) == EOF)
	{
        	char *buff="admin#admin@";
        	fputs(buff, LoginFile);
	}
        fseek(LoginFile, 0, SEEK_SET);
	char temp[50];
	while (fscanf(LoginFile, "%s@", temp) != EOF)
	{
		strcat(temp,"\0");
		//printf("%s\n",temp);
		//printf("%s\n",RecvBuf);
		if (strcmp (temp, RecvBuf) == 0)
		{
			printf("Log in success!\n");
            send(ClientFd, "LOGI", 5, 0);
			fclose(LoginFile);
			return 1 ;
		}          
    }
    send(ClientFd, "FAIL", 5, 0);
	fclose(LoginFile);
	return 0;
}

/***********************************************
Function Name: Server_Sign
Function: sign a new user in the Userlist
Parameter:UserName, Password
************************************************/
void Server_Sign(char* username, char* passwd)
{
	FILE* SignFile;
	SignFile = fopen(USERFILE, "a+");
	fseek(SignFile, 0, SEEK_END);
	fputs("\n", SignFile);
	fputs(username, SignFile);
	fputs("#",SignFile);
	fputs(passwd, SignFile);
	fputs("@",SignFile);
	fclose(SignFile);
}
