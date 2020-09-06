//2014313303, Taeha Hong
#include <sys/types.h>
#include <sys/socket.h> 
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h> 
#include <stdlib.h>
#include <strings.h>
#define MAXLINE 200

int stoi();
int strcmp();
char * strcpy();
size_t strlen();

int main (int argc, char *argv[]) { 
	int n, cfd,fd;
	struct hostent *h; 
	struct sockaddr_in saddr;
	char buf[MAXLINE]; 
	char *host; 
	int port;
	int input_len,i;
	int col_flag=0;
	int dot_count=0;
	int port_start;
	int err_flag=0;
	char* pathname;
	int s_flag = 0;
	char* local = "127.0.0.1";
	char* file;
	char buf2[200];
	int result[1];
	int file_len;
	char* temp;

	while(1){
		input_len = read(STDIN_FILENO,buf,MAXLINE);
		buf[input_len] = '\0';
		if(!strcmp(buf,"exit\n")){
			exit(EXIT_SUCCESS);
		}
		for(i=0;i<input_len;i++){
			if(buf[i] == '.'){
				dot_count++;
			}
			if(buf[i] == ':'){
				buf[i] = '\0';
				if(!strcmp(buf,"localhost")){
					host = local;
				}else if(dot_count == 3){
					host = buf;
				}else{
					err_flag=1;
					break;
				}
				port_start = i+1;
				col_flag=1;
			}
			if(buf[i] == '/' && col_flag == 1){
				buf[i] = '\0';
				if(i == port_start){
					port = 10001;
				}else{
					port = stoi(buf+port_start);
				}
				pathname = buf+i+1;
				s_flag = 1;
				buf[i] = '/';
				break;
			}else if(buf[i] == '/' && col_flag == 0){
				buf[i] = '\0';
				if(!strcmp(buf,"localhost")){
					host = local;
				}else if(dot_count == 3){
					host = buf;
				}else{
					err_flag = 1;
					break;
				}
				port = 10001;
				pathname = buf+i+1;
				s_flag = 1;
				buf[i] = '/';
				break;
			}
		}
		dot_count = 0;
		col_flag = 0;	
		port_start = 0;
		if(err_flag==1 || s_flag == 0){
			err_flag = 0;
			s_flag = 0;
			continue;
		}
		pathname[strlen(pathname)]='\0';

		if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
			continue;
		}
		if ((h = gethostbyname(host)) == NULL) { 
			continue;
		}
		bzero((char *)&saddr, sizeof(saddr)); 
		saddr.sin_family = AF_INET; 
		bcopy((char *)h->h_addr, (char *)&saddr.sin_addr.s_addr, h->h_length);
		saddr.sin_port = htons(port); 
		if (connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr)) < 0) {
			write(STDOUT_FILENO,"E_CONN, ",8);
			write(STDOUT_FILENO,pathname,strlen(pathname));
			continue;
		}

		write(cfd, pathname,MAXLINE);

		for(i=input_len-2;i>=0;i--){
			if(buf[i]=='/'){
				file = buf+i+1;
				break;
			}
		}

		read(cfd,result,4);
		if(result[0]==0){
			write(STDOUT_FILENO,"OK, ",4);
			write(STDOUT_FILENO,pathname,strlen(pathname));
		}else if(result[0]==1){
			write(STDOUT_FILENO,"E_FILE, ",8);
			write(STDOUT_FILENO,pathname,strlen(pathname));
			continue;
		}else if(result[0]==2){
			write(STDOUT_FILENO,"E_PERM, ",8);
			write(STDOUT_FILENO,pathname,strlen(pathname));
			continue;
		}

		file[strlen(file)-1] = '\0';
		fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0666);

		read(cfd,&file_len,4);
		while((n = read(cfd,buf2,file_len))>0){
			write(fd, buf2,n);
			read(cfd,&file_len,4);
		}
			close(fd);
		close(cfd);
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
int strcmp(const char *lhs, const char *rhs)
{
	do {
		if (*lhs < *rhs)
			return -1;
		else if (*lhs > *rhs)
			return 1;
	} while (*lhs++ && *rhs++);
	return 0;
}
char *strcpy (char *dst, const char *src)
{
	char *t = dst;
	while(*src)
		*t++ = *src++;
	*t = '\0';
	return dst;
}
size_t strlen (const char *str)
{
	size_t r = -1;
	while(str[++r]);
	return r;
}
