#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>

void main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("Usage :: ./myser {Port_Number} \n");
		return;
	}
	int ls,s,percent,count1=1,count2=1;
	char fileN[4096],fileS[1024];
	char recvW[1];
	socklen_t clilen;
	struct sockaddr_in ser,cli;
	ls = socket(AF_INET,SOCK_STREAM,0);
	if(ls<0)
		printf("Error in creating socket\n");
	ser.sin_addr.s_addr = inet_addr("127.0.0.1");
	//ser.sin_addr.s_addr = inet_addr("192.168.43.250");
	ser.sin_family = AF_INET;
	ser.sin_port = htons(atoi(argv[1]));
	// Socket created and server defined

	int b = bind(ls,(struct sockaddr*)&ser,sizeof(ser));
	if(b<0)
	{	
		printf("Error in binding\n");
		close(ls);
		return;
	}
	// Socket Binded

	int l = listen(ls,5);
	// Listen queue defined

	if(l<0)
	{
		printf("Error in listening\n");
		return;
	}

	clilen = sizeof(cli);
	s = accept(ls,(struct sockaddr*)&cli,&clilen);
	// Connection Accepted

	if(s<0)
	{
		printf("Error in accepting connection\n");
		close(ls);
		close(s);
	}
	recv(s,fileN,sizeof(fileN),0);
	printf("File Name :: %s\n",fileN);

	recv(s,fileS,sizeof(fileS),0);
	if(atoi(fileS)>(1024*1024))
	{
		printf("File Size :: %.3f MB\n",(atof(fileS)/1024)/1024);
	}
	else if(atoi(fileS)>1024)
	{
		printf("File Size :: %.3f kB\n",(atof(fileS)/1024));
	}
	else
	{
		printf("File Size :: %d Bytes\n",atoi(fileS));
	}
	percent = atoi(fileS)/100;
	FILE *fp = fopen(fileN,"w");
	while(1)
	{
		if(recv(s,recvW,1,0)!=0)
		{
			fwrite(recvW,sizeof(recvW[0]),1,fp);
			if(count1 == count2)
			{
				printf("Percent  :: %d %%\r",count1/percent);
				fflush(stdout);
				count1+=percent;
			}
			count2++;
			recvW[0] = 0;
		}
		else
		{
			printf("\n");
			if(atoi(fileS)>(1024*1024))
			{
				printf("Received File name :: %s . With File Size :: %.3f MB\n",fileN,(atof(fileS)/1024)/1024);
			}
			else if(atoi(fileS)>1024)
			{
				printf("Received File name :: %s . With File Size :: %.3f kB\n",fileN,atof(fileS)/1024);
			}
			else
			{
				printf("Received File name :: %s . With File Size :: %d B\n",fileN,atoi(fileS));
			}
			close(ls);
			return;
		}
	}
}
