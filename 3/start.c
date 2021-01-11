#include<sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* close() */
#include <sys/socket.h>
#include <netdb.h>
 
void checkipnport(const char *host ,const char *port)
{
    int sock;
    //printf("hsakdj\n");
	//char host[] = "www.bits-pilani.ac.in";
    //char port[] = "80";
    struct addrinfo hints, *res;
	//char message[] = "GET / HTTP/1.1\nHost: www.bits-pilani.ac.in/\n\n";
    char buf[1024];
    int status;
 
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(host, port, &hints, &res);
    if (status != 0) {
        //perror("getaddrinfo");
        return;
    }
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        //perror("socket");
        return;
    }
    status = connect(sock, res->ai_addr, res->ai_addrlen);
    if (status == -1) {
        //perror("connect");
        return;
    }
    //printf("Yeah\n");
    printf("%s host works on port %s\n", host, port);
    freeaddrinfo(res);
    close(sock);
    return;
}

int main(){
	FILE *f = fopen("website.txt", "r");
	int tot = 100;
	int total = 0;
	char **host = (char**)malloc(tot*sizeof(char*));
	for(int i =0;;i++){
		host[i] = (char*)malloc(5000*sizeof(char));
		if(fscanf(f, "%[^\n]\n", host[i]) == EOF)break;
		if(i== tot-1){
			tot = tot*2;
			host = realloc(host, tot*sizeof(char*));
		}
		//printf("%s\n",host[i]);
		total++;
	}
		//printf("%d\n",total);
	fclose(f);
	int status;
	for(int i = 0;i< total;i++){
		printf("%s\n", host[i]);
		char *tmp = host[i];
		int tr = 1;
		char ht[] = "https://";
		for(int i =0;i<strlen(ht);i++){
			if(ht[i] != tmp[i]){
				tr = 0;
				break;
			}
		}
		if(tr ==1)tmp = tmp + 8;
		char *t = tmp;
		while(*t != '/' && *t!='\0')t++;
		*t = '\0';
		//printf("%s\n", tmp);
		char cnum[10];	
		int num = 80;
		sprintf(cnum, "%d", num);
		//printf("%s\n", cnum);
		if(fork() == 0){
			checkipnport(tmp, cnum);
			exit(0);
		}
		for(num = 1;num<80;num++){
			sprintf(cnum, "%d", num);
			//printf("%s\n", cnum);
			if(fork()==0){
				checkipnport(tmp, cnum);
				exit(0);
			}
		}
		for(num = 81;num<1025;num++){
			sprintf(cnum, "%d", num);
			//printf("%s\n", cnum);
			if(fork()==0){
				checkipnport(tmp, cnum);
				exit(0);
			}
		}
		while(wait(&status)>0);
		printf("\n");
	}
}
