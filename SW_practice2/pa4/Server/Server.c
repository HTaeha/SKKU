//2014313303, Taeha Hong
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <strings.h>
#define MAXLINE 200

int stoi();
size_t strlen();

int main (int argc, char *argv[]) {
	int n=0;
	int fd, listenfd, connfd, caddrlen;
	struct hostent *h;
	struct sockaddr_in saddr, caddr;
	char buf[200];
	int port;
	char pathname[MAXLINE];
	int result=0;
	int child_status;
	int length=0;

	if(argv[1] == '\0'){
		port = 10001;
	}else{
		port = stoi(argv[1]);
	}
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		return -1;
	}
	bzero((char *)&saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
		return -1;
	}
	if (listen(listenfd, 5) < 0) {
		return -1;
	}
	while (1) { 
		caddrlen = sizeof(caddr);
		if ((connfd = accept(listenfd, (struct sockaddr *)&caddr, &caddrlen)) < 0) {
			return -1;
		}
		h = gethostbyaddr((const char *)&caddr.sin_addr.s_addr,	sizeof(caddr.sin_addr.s_addr), AF_INET);

		if(fork()==0){
			close(listenfd);
			read(connfd, pathname, MAXLINE);
			pathname[strlen(pathname)-1]='\0';

			fd = open(pathname, O_RDONLY);
			if(fd == -1){
				if(errno == ENOENT){
					result = 1;
					write(connfd,&result,4);
					close(connfd);
					exit(0);
				}else if(errno == EPERM){
					result = 2;
					write(connfd,&result,4);
					close(connfd);
					exit(0);
				}
			}else{
				result = 0;
				write(connfd,&result,4);
			}

			while((n=read(fd,buf,MAXLINE))>0){
				write(connfd,&n,4);
				write(connfd,buf,n);
			}

			close(fd);
			close(connfd);
			exit(0);
		}else{
			wait(&child_status);
		}
		close(connfd);
		
	}
}
int stoi(char* s){
	int result=0;
	int i;

	for(i=0;i<strlen(s);i++){
		result = 10*result +(int)(s[i]-'0');
	}
	return result;
}
size_t strlen (const char *str)
{
	size_t r = -1;
	while(str[++r]);
	return r;
}
