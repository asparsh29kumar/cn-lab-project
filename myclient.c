///This is the client code.
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
 
int fileSEND(char *server, int PORT, const char *lfile,const char *rfile){
    int socketDESC;
    struct sockaddr_in serverADDRESS;
    FILE * file_to_send;
    int ch;
    char toSEND[1];
    //char remoteFILE[4096];
    int count1=1,count2=1, percent;

    serverADDRESS.sin_addr.s_addr = inet_addr("127.0.0.1");
    //serverADDRESS.sin_addr.s_addr = inet_addr("192.168.43.76");
    serverADDRESS.sin_port = htons(PORT);
    serverADDRESS.sin_family = AF_INET;

    socketDESC = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDESC < 0) {
        printf("Cannot create socket\n");
        return 1;
    }
                 
    if (connect(socketDESC, (struct sockaddr *) &serverADDRESS, sizeof(serverADDRESS)) < 0)
    {
        printf("Cannot connect\n");
        return 1;
    }
 
 
    file_to_send = fopen (lfile,"r");
    if(!file_to_send) 
    {
        perror("Error opening file\n");
        close(socketDESC);
        return 0;
    }
    else 
    {
        long fileSIZE;
        fseek (file_to_send, 0, SEEK_END);
        fileSIZE =ftell (file_to_send);
        rewind(file_to_send);

        send(socketDESC, rfile, strlen(rfile), 0);
        // File size sent
        char rem[1024];
        sprintf(rem,"%ld",fileSIZE);
        printf("Remote file size :: %s\n",rem);
        send(socketDESC,rem,sizeof(rem),0);
        // File name sent

        percent = fileSIZE / 100;
        while((ch=getc(file_to_send))!=EOF)
        {
            toSEND[0] = ch;
            send(socketDESC, toSEND, 1, 0);
            if( count1 == count2 ) 
            {
                printf("Percent  :: %d %%\r",count1/percent);
                fflush(stdout);
                count1+=percent;
            }
            count2++;
        }
        printf("\n");
    }
    fclose(file_to_send);
    close(socketDESC);
 
    return 0;
}
 
void main(int argc, char const *argv[])
{
    if(argc < 3)
    {
        printf("Usage :: ./mycli {port_no} {file_to_send} {file_name_at_server}\n");
        return;
    }
    fileSEND("localhost", atoi(argv[1]), argv[2], argv[3]);
    return;
}
