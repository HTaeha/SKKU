//-----------------------------------------------------------
// 
// SWE2007: Software Experiment II (Fall 2014)
//
// Skeleton code for PA #3
// November 11, 2015
//
// Taeha Hong, 2014313303
//-----------------------------------------------------------
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <wait.h>
#include "string_sw.h"

#define MAXLINE 200

void s_pipe();
void redirection_in_out();
void redirection_in();
void redirection_out();
void redirection_out2();
void pipe_head();
void pipe_tail();
void pipe_cat();
void s_head();
void s_tail();
void s_cat();
void s_cp();
void s_mv();
void s_rm();
void s_cd();
void input_check();
void s_pwd();
void s_exit();
char* my_strchr();
int stoi();
char* itos();
int strncmp();

char *pwd (int full)
{
	static char buffer[1024];
	char *ptr;
	if (getcwd(buffer, 1024) != buffer)
		return NULL;

	if (full)
		return buffer;

	if (strcmp("/", buffer) == 0)
		return buffer;

	ptr = strrchr(buffer, '/');
	return ptr+1;
}

void print_prompt()
{
	int ret;
	char *ptr = pwd(0);
	char head[200];

	if (!ptr)
		exit(1);
	if (strlen(ptr) > 190)
		exit(1);

	strcpy(head, "swsh:");
	strcat(head, ptr);
	strcat(head, "> ");

	ret = write(2, head, strlen(head));
	if (ret <= 0)
		exit(1);
}
int main(void)
{
	int ret,i;
	char input[240];
	char buf[240];
	while (1) {
		for(i=0;i<240;i++){
			input[i] = '\0';
			buf[i] = '\0';
		}

		signal(SIGINT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		print_prompt();

		ret = read(0, input, 200);
		if (ret < 0)
			exit(1);
		else if (ret == 0) {
			write(STDERR_FILENO, "exit\n", 5);
			break;
		}
		strcpy(buf, input);
		input_check(input, buf);
	}

	return 0;
}
void input_check(char* input, char* buf){
	int i,j;
	char *argv[MAXLINE];
	int argc=0;
	char *delim = input;
	int child_status;
	int cmd_count=1;
	int cmd_index[MAXLINE];
	int red_in_flag=0;
	int red_out_flag=0;
	int cmd_size[MAXLINE];
	int size = 0;
	char **cmd[MAXLINE];
	char **temp;

	buf[strlen(buf)-1] = ' ';
	argc = 0 ;
	for(i=0;i<MAXLINE;i++){
		argv[i] = '\0';
		cmd_index[i] = '\0';
		cmd_size[i] = '\0';
		cmd[i] = '\0';
	}
	while(*buf && (*buf == ' ')){
		buf++;
	}
	while(delim = my_strchr(buf, ' ')){
			argv[argc++] = buf;
			*delim = '\0';
			buf = delim + 1;
			while(*buf && (*buf == ' ')){
				buf++;
			}
	}
	argv[argc] = '\0';

	cmd_index[0] = 0;
	i=0;
	j=1;
	cmd_size[0] = argc;
	while(1){
		if(strcmp(argv[i],"|")==0){
			cmd_count++;
			cmd_index[j]=i+1;
			cmd_size[j]=size;
			size=0;
			j++;
		}
		if(i == argc-1){
			break;
		}
		i++;
		size++;
	}
	cmd_index[cmd_count] = '\0';

	if(argc == 0){
		write(STDERR_FILENO, "swsh: Command not found\n", 24);
	}

	for(i=0;i<argc;i++){
		if(!strcmp(argv[i], "<")){
			red_in_flag=1;
		}
		if(!strcmp(argv[i], ">") || !strcmp(argv[i], ">>")){
			red_out_flag=1;
		}

	}
	
	if(cmd_count>1){
		s_pipe(argv, argc,cmd_index,cmd_size,red_in_flag,red_out_flag,cmd_count);
	}else{
			if(red_in_flag && red_out_flag && argc>=3){
				redirection_in_out(argv, argc,cmd_index, 0, cmd_size, red_out_flag,cmd_count);
			}else if(argc>=2 && red_in_flag){
//				if(cmd_count>=2){
//					for(j=0;j<cmd_index[1]-2;j++){
//						if(strcmp(argv[j], "<")==0){
//							redirection_in(argv, argc,cmd_index, 0, j,cmd_size, red_out_flag);
//						}
//					}
//				}else{
					for(j=0;j<argc;j++){
						if(strcmp(argv[j], "<")==0){
							redirection_in(argv, argc, cmd_index, 0,j,cmd_size, red_out_flag);
						}
					}
//				}
			}else if(argc>=3 && strcmp(argv[argc-2],">")==0){
				redirection_out(argv, argc, cmd_index,0, cmd_count,cmd_size, red_out_flag);
			}else if(argc>=3 && strcmp(argv[argc-2],">>")==0){
				redirection_out2(argv,argc, cmd_index,0, cmd_count,cmd_size, red_out_flag);
			}else if(!strcmp(argv[0], "ls") ||!strcmp(argv[0], "man") ||!strcmp(argv[0], "grep") ||!strcmp(argv[0], "bc") ||!strcmp(argv[0], "awk") ||!strcmp(argv[0], "sort") || (argv[0][0] == '.' && argv[0][1] == '/')){
				if(fork()==0){
					execvp(argv[0], argv);
					exit(EXIT_FAILURE);
				}else{
					wait(&child_status);
				}
			}else if(!strcmp("head", argv[0])){
				s_head(argv, argc, cmd_index, 0,cmd_size, red_out_flag);
			}else if(!strcmp("tail", argv[0])){
				s_tail(argv, argc, cmd_index, 0,cmd_size, red_out_flag);
			}else if(!strcmp("cat", argv[0])){
				s_cat(argv, cmd_index, 0);
			}else if(!strcmp("cp", argv[0])){
				s_cp(argv, cmd_index, 0);
			}else if(!strcmp("mv",argv[0])){
				s_mv(argv, cmd_index, 0);
			}else if(!strcmp("rm", argv[0])){
				s_rm(argv, cmd_index, 0);
			}else if(!strcmp("cd", argv[0])){
				s_cd(argv, cmd_index, 0);
			}else if(!strcmp("pwd",argv[0])){
				s_pwd();
			}else if(!strcmp("exit", argv[0])){
				s_exit(argv, cmd_index, 0);
		}else{
			write(STDERR_FILENO, "swsh: Command not found\n", 24);
		}
	}
}

void s_pipe(char ** argv, int argc, int * cmd_index, int * cmd_size, int red_in_flag,int red_out_flag, int cmd_count){
	int   p[2];
	pid_t pid;
	int   fd_in = 0;
	int i,j;

	for(i=0;i<argc;i++){
		if(!strcmp(argv[i],"|"))
			argv[i] = '\0';
	}
	argv[argc] = NULL;
	for(i=0;i<cmd_count;i++)
	{
		pipe(p);
		if ((pid = fork()) == -1)
		{
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{

	//		printf("cmd_count %d  argc %d  i %d  red_in_flag %d\n",cmd_count,argc,i,red_in_flag);
			dup2(fd_in, STDIN_FILENO); //change the input according to the old one 
			if (i != cmd_count-1)
				dup2(p[1], STDOUT_FILENO);
			close(p[0]);

			if(red_in_flag && i==0){
				redirection_in(argv,argc,cmd_index,0,cmd_index[1]-3,cmd_size,red_out_flag);
			}else if(i==cmd_count-1&&red_out_flag && argc>=3 && !strcmp(argv[argc-2],">")){
				redirection_out(argv, argc, cmd_index,cmd_count-1, cmd_count,cmd_size, red_out_flag);
			}else if(i==cmd_count-1&&red_out_flag && argc>=3 && !strcmp(argv[argc-2],">>")){
				redirection_out2(argv,argc, cmd_index,i, cmd_count,cmd_size, red_out_flag);
			}else if(!strcmp("head", argv[cmd_index[i]])){
	//			printf("head\n");
				s_head(argv, argc, cmd_index, i,cmd_size, red_out_flag);
			}else if(!strcmp("tail", argv[cmd_index[i]])){
				s_tail(argv, argc, cmd_index, i,cmd_size, red_out_flag);
			}else if(!strcmp("cat", argv[cmd_index[i]])){
				s_cat(argv, cmd_index, i);
			}else if(!strcmp("cp", argv[cmd_index[i]])){
				s_cp(argv, cmd_index, i);
			}else if(!strcmp("mv",argv[cmd_index[i]])){
				s_mv(argv, cmd_index, i);
			}else if(!strcmp("rm", argv[cmd_index[i]])){
				s_rm(argv, cmd_index, i);
			}else if(!strcmp("cd", argv[cmd_index[i]])){
				s_cd(argv, cmd_index, i);
			}else if(!strcmp("pwd",argv[cmd_index[i]])){
				s_pwd();
			}else if(!strcmp("exit", argv[cmd_index[i]])){
				s_exit(argv, cmd_index, i);
			}else{
	//			printf("exe\n");
				execvp(argv[cmd_index[i]], argv+cmd_index[i]);
				exit(EXIT_FAILURE);
			}
			exit(0);
		}
		else
		{
			wait(NULL);
			close(p[1]);
			fd_in = p[0]; //save the input for the next command
		}
	}
	return ;
}
void redirection_in_out(char ** argv , int argc, int * cmd_index, int i, int * cmd_size,int red_out_flag, int cmd_count){
	int fd, fd2;
	int child_status;
//printf("red in out\n");
	argv[argc-4] = '\0';
	argv[argc-2] = '\0';
	if(fork()==0){
		fd = open(argv[argc-3], O_RDONLY);
		if(fd == -1){
			if(errno == ENOENT){
				write(STDERR_FILENO,"swsh: No such file\n",19);
				exit(EXIT_FAILURE);
			}		
		}
		fd2 = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDIN_FILENO);
		dup2(fd2, STDOUT_FILENO);
		close(fd2);
		close(fd);
		if(!strcmp("head", argv[0])){
			s_head(argv, argc, cmd_index, i,cmd_size, red_out_flag);
		}else if(!strcmp("tail", argv[0])){
			s_tail(argv, argc, cmd_index, i,cmd_size, red_out_flag);
		}else if(!strcmp("cat", argv[0])){
			s_cat(argv, cmd_index, i);
		}else if(!strcmp("cp", argv[0])){
			s_cp(argv, cmd_index, i);
		}else if(!strcmp("mv",argv[0])){
			s_mv(argv, cmd_index, i);
		}else if(!strcmp("rm", argv[0])){
			s_rm(argv, cmd_index, i);
		}else if(!strcmp("cd", argv[0])){
			s_cd(argv, cmd_index, i);
		}else if(!strcmp("pwd",argv[0])){
			s_pwd();
		}else if(!strcmp("exit", argv[0])){
			s_exit(argv, cmd_index, i);
		}else{
			execvp(argv[0], argv);
		}
		exit(EXIT_FAILURE);
	}else{
		wait(&child_status);
	}


}
void redirection_in(char ** argv , int argc, int * cmd_index, int i,int red_index, int * cmd_size,int red_out_flag){
	int fd;
	int child_status;

	i=0;
	argv[red_index] = '\0';
	if(fork()==0){
		fd = open(argv[red_index+1], O_RDONLY);
		if(fd == -1){
			if(errno == ENOENT){
				write(STDERR_FILENO,"swsh: No such file\n",19);
				exit(EXIT_FAILURE);
			}		
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
		if(!strcmp("head", argv[0])){
			s_head(argv, argc, cmd_index, 0,cmd_size, red_out_flag);
		}else if(!strcmp("tail", argv[0])){
			s_tail(argv, argc, cmd_index, 0,cmd_size, red_out_flag);
		}else if(!strcmp("cat", argv[0])){
			if(argc==3){
				pipe_cat(argc,cmd_index,0);
			}else{
				s_cat(argv, cmd_index, 0);
			}
		}else if(!strcmp("cp", argv[0])){
			s_cp(argv, cmd_index, 0);
		}else if(!strcmp("mv",argv[0])){
			s_mv(argv, cmd_index, 0);
		}else if(!strcmp("rm", argv[0])){
			s_rm(argv, cmd_index, 0);
		}else if(!strcmp("cd", argv[0])){
			s_cd(argv, cmd_index, 0);
		}else if(!strcmp("pwd",argv[0])){
			s_pwd();
		}else if(!strcmp("exit", argv[0])){
			s_exit(argv, cmd_index, 0);
		}else{
			execvp(argv[0], argv);
		}
		exit(EXIT_FAILURE);
	}else{
		wait(&child_status);
	}

}
void redirection_out(char ** argv, int argc, int * cmd_index,int i, int cmd_count,int * cmd_size,int red_out_flag){
	int fd;
	int child_status;

	argv[argc-2] = '\0';
	if(fork()==0){
		fd = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		if(!strcmp("head", argv[cmd_index[cmd_count-1]])){
			s_head(argv, argc, cmd_index, cmd_count-1,cmd_size, red_out_flag);
		}else if(!strcmp("tail", argv[cmd_index[cmd_count-1]])){
			s_tail(argv, argc, cmd_index, cmd_count-1,cmd_size, red_out_flag);
		}else if(!strcmp("cat", argv[cmd_index[cmd_count-1]])){
			s_cat(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("cp", argv[cmd_index[cmd_count-1]])){
			s_cp(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("mv",argv[cmd_index[cmd_count-1]])){
			s_mv(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("rm", argv[cmd_index[cmd_count-1]])){
			s_rm(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("cd", argv[cmd_index[cmd_count-1]])){
			s_cd(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("pwd",argv[cmd_index[cmd_count-1]])){
			s_pwd();
		}else if(!strcmp("exit", argv[cmd_index[cmd_count-1]])){
			s_exit(argv, cmd_index, cmd_count-1);
		}else{
			execvp(argv[cmd_index[cmd_count-1]], argv+cmd_index[cmd_count-1]);
		}
		exit(EXIT_FAILURE);
	}else{
		wait(&child_status);
	}
}
void redirection_out2(char ** argv, int argc, int * cmd_index,int i, int cmd_count, int * cmd_size,int red_out_flag){
	int fd;
	int child_status;

	argv[argc-2] = '\0';
	if(fork()==0){
		fd = open(argv[argc-1], O_WRONLY | O_CREAT | O_APPEND, 0666);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		if(!strcmp("head", argv[cmd_index[cmd_count-1]])){
			s_head(argv, argc, cmd_index, cmd_count-1,cmd_size, red_out_flag);
		}else if(!strcmp("tail", argv[cmd_index[cmd_count-1]])){
			s_tail(argv, argc, cmd_index, cmd_count-1,cmd_size, red_out_flag);
		}else if(!strcmp("cat", argv[cmd_index[cmd_count-1]])){
			s_cat(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("cp", argv[cmd_index[cmd_count-1]])){
			s_cp(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("mv",argv[cmd_index[cmd_count-1]])){
			s_mv(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("rm", argv[cmd_index[cmd_count-1]])){
			s_rm(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("cd", argv[cmd_index[cmd_count-1]])){
			s_cd(argv, cmd_index, cmd_count-1);
		}else if(!strcmp("pwd",argv[cmd_index[cmd_count-1]])){
			s_pwd();
		}else if(!strcmp("exit", argv[cmd_index[cmd_count-1]])){
			s_exit(argv, cmd_index, cmd_count-1);
		}else{
			execvp(argv[cmd_index[cmd_count-1]], argv+cmd_index[cmd_count-1]);
		}
		exit(EXIT_FAILURE);
	}else{
		wait(&child_status);
	}
}
void pipe_head(char ** argv, int argc, int * cmd_index, int a, int * cmd_size,int red_out_flag){
	int fd;
	char buf[1] = {0,};
	int n_count=0;
	int i;
	int num;
	int file_index = cmd_index[a]+1;
	int op_flag=0;


	fd = open("asdfasdf", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	while(read(STDIN_FILENO,buf,1)>0){
		write(fd, buf, 1);
	}

	if(red_out_flag==0){
		for(i=0;i<cmd_size[a];i++){
			if(!strcmp("-n", argv[cmd_index[a]+i])){
				op_flag = 1;
				num = stoi(argv[cmd_index[a]+i+1]);
				if(i==1){
					file_index = cmd_index[a]+3;
				}else if(i==2){
					file_index = cmd_index[a]+1;
				}
			}
		}
	}else{
		for(i=0;i<cmd_size[a]-2;i++){
			if(!strcmp("-n", argv[cmd_index[a]+i])){
				op_flag = 1;
				num = stoi(argv[cmd_index[a]+i+1]);
				if(i==1){
					file_index = cmd_index[a]+3;
				}else if(i==2){
					file_index = cmd_index[a]+1;
				}
			}
		}
	}
	fd = open("asdfasdf", O_RDONLY);


	while(read(fd, buf, 1)>0){
		if(buf[0] == '\n'){
			n_count++;
		}
		if(n_count == num && op_flag == 1){
			write(STDOUT_FILENO,"\n",1);
			break;
		}
		if(n_count == 10){
			write(STDOUT_FILENO, "\n",1);
			break;
		}
		write(STDOUT_FILENO, buf, 1);
	}
	close(fd);
	unlink("asdfasdf");
}
void pipe_tail(char ** argv, int argc, int * cmd_index, int a,int * cmd_size, int red_out_flag){
	int fd;
	char buf[1] = {0,};
	int n_count=0;
	int i;
	int num;
	int file_index = 1;
	int op_flag=0;
	int total_n=0;

	fd = open("asdfasdf1", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	while(read(STDIN_FILENO,buf,1)>0){
		write(fd, buf, 1);
	}

	if(red_out_flag==0){
		for(i=0;i<cmd_size[a];i++){
			if(!strcmp("-n", argv[cmd_index[a]+i])){
				op_flag = 1;
				num = stoi(argv[cmd_index[a]+i+1]);
				if(i==1){
					file_index = cmd_index[a]+3;
				}else if(i==2){
					file_index = cmd_index[a]+1;
				}
			}
		}
	}else{
		for(i=0;i<cmd_size[a]-2;i++){
			if(!strcmp("-n", argv[cmd_index[a]+i])){
				op_flag = 1;
				num = stoi(argv[cmd_index[a]+i+1]);
				if(i==1){
					file_index = cmd_index[a]+3;
				}else if(i==2){
					file_index = cmd_index[a]+1;
				}
			}
		}
	}

	fd = open("asdfasdf1", O_RDONLY);

	while(read(fd, buf, 1)>0){
		if(buf[0] == '\n'){
			total_n++;
		}
	}
	lseek(fd, 0, SEEK_SET);
	while(read(fd, buf, 1)>0){
		if(op_flag ==1){
			if(n_count >= total_n-num){
				write(STDOUT_FILENO,buf,1);
			}
		}else{
			if(total_n<=10){
				write(STDOUT_FILENO,buf, 1);
			}else if(n_count >= total_n-10){
				write(STDOUT_FILENO, buf,1);
			}
		}
		if(buf[0] == '\n'){
			n_count++;
		}
	}
	close(fd);
	unlink("asdfasdf1");
}
void pipe_cat(char ** argv, int* cmd_index, int i){
	int fd;
	char buf[1]={0,};

	while(read(fd,buf,1)>0){
		write(STDOUT_FILENO,buf,1);
	}
	close(fd);
}

void s_head(char ** argv, int argc, int * cmd_index, int a, int * cmd_size,int red_out_flag){
	int fd;
	char buf[1] = {0,};
	int n_count=0;
	int i;
	int num;
	int file_index = cmd_index[a]+1;
	int op_flag=0;
//printf("hh\n");
//printf("a : %d, red_out_flag : %d",a,red_out_flag);
	if(red_out_flag==0){
		for(i=0;i<cmd_size[a];i++){
			if(!strcmp("-n", argv[cmd_index[a]+i])){
				op_flag = 1;
				num = stoi(argv[cmd_index[a]+i+1]);
				if(i==1){
					file_index = cmd_index[a]+3;
				}else if(i==2){
					file_index = cmd_index[a]+1;
				}
			}
		}
	}else{
		for(i=0;i<cmd_size[a]-2;i++){
			if(!strcmp("-n", argv[cmd_index[a]+i])){
				op_flag = 1;
				num = stoi(argv[cmd_index[a]+i+1]);
				if(i==1){
					file_index = cmd_index[a]+3;
				}else if(i==2){
					file_index = cmd_index[a]+1;
				}
			}
		}
	}
//printf("open\n");
	fd = open(argv[file_index], O_RDONLY);


	while(read(fd, buf, 1)>0){
		if(buf[0] == '\n'){
			n_count++;
		}
		if(n_count == num && op_flag == 1){
			write(STDOUT_FILENO,"\n",1);
			break;
		}
		if(n_count == 10 && op_flag == 0){
			write(STDOUT_FILENO, "\n",1);
			break;
		}
		write(STDOUT_FILENO, buf, 1);
	}
	close(fd);
}
void s_tail(char ** argv, int argc, int * cmd_index, int a,int * cmd_size, int red_out_flag){
	int fd;
	char buf[1] = {0,};
	int n_count=0;
	int i;
	int num;
	int file_index = 1;
	int op_flag=0;
	int total_n=0;

	if(red_out_flag==0){
		for(i=0;i<cmd_size[a];i++){
			if(!strcmp("-n", argv[cmd_index[a]+i])){
				op_flag = 1;
				num = stoi(argv[cmd_index[a]+i+1]);
				if(i==1){
					file_index = cmd_index[a]+3;
				}else if(i==2){
					file_index = cmd_index[a]+1;
				}
			}
		}
	}else{
		for(i=0;i<cmd_size[a]-2;i++){
			if(!strcmp("-n", argv[cmd_index[a]+i])){
				op_flag = 1;
				num = stoi(argv[cmd_index[a]+i+1]);
				if(i==1){
					file_index = cmd_index[a]+3;
				}else if(i==2){
					file_index = cmd_index[a]+1;
				}
			}
		}
	}

	fd = open(argv[file_index], O_RDONLY);

	while(read(fd, buf, 1)>0){
		if(buf[0] == '\n'){
			total_n++;
		}
	}
	lseek(fd, 0, SEEK_SET);
	while(read(fd, buf, 1)>0){
		if(op_flag ==1){
			if(n_count >= total_n-num){
				write(STDOUT_FILENO,buf,1);
			}
		}else{
			if(total_n<=10){
				write(STDOUT_FILENO,buf, 1);
			}else if(n_count >= total_n-10){
				write(STDOUT_FILENO, buf,1);
			}
		}
		if(buf[0] == '\n'){
			n_count++;
		}
	}
	close(fd);

}
void s_cat(char ** argv, int* cmd_index, int i){
	int fd;
	char buf[1]={0,};

	fd = open(argv[cmd_index[i]+1], O_RDONLY);

	while(read(fd,buf,1)>0){
		write(STDOUT_FILENO,buf,1);
	}
	close(fd);
}
void s_cp(char ** argv, int* cmd_index, int i){
	int fd,fd2;
	char buf[1]={0,};

	fd = open(argv[cmd_index[i]+1], O_RDONLY);
	fd2 = open(argv[cmd_index[i]+2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	while(read(fd,buf,1)>0){
		write(fd2,buf,1);
	}
	close(fd);
	close(fd2);
}
void s_mv(char ** argv, int * cmd_index, int i){
	if(rename(argv[cmd_index[i]+1], argv[cmd_index[i]+2])==-1){
		if(errno == EACCES){
			write(STDERR_FILENO, "mv: Permission denied\n", 22);
		}else if(errno == EISDIR){
			write(STDERR_FILENO, "mv: Is a directory\n",19);
		}else if(errno == ENOENT){
			write(STDERR_FILENO, "mv: No such file or directory\n",30);
		}else if(errno == ENOTDIR){
			write(STDERR_FILENO, "mv: Not a directory\n",20);
		}else if(errno == EPERM){
			write(STDERR_FILENO, "mv: Permission denied\n",22);
		}else{
			write(STDERR_FILENO, "Error occurred: ",16);
			write(STDERR_FILENO, itos(errno), strlen(itos(errno)));
			write(STDERR_FILENO, "\n", 1);
		}
	}
}
void s_rm(char ** argv, int * cmd_index, int i){
	if(unlink(argv[cmd_index[i]+1])==-1){
		if(errno == EACCES){
			write(STDERR_FILENO, "rm: Permission denied\n", 22);
		}else if(errno == EISDIR){
			write(STDERR_FILENO, "rm: Is a directory\n",19);
		}else if(errno == ENOENT){
			write(STDERR_FILENO, "rm: No such file or directory\n",30);
		}else if(errno == ENOTDIR){
			write(STDERR_FILENO, "rm: Not a directory\n",20);
		}else if(errno == EPERM){
			write(STDERR_FILENO, "rm: Permission denied\n",22);
		}else{
			write(STDERR_FILENO, "Error occurred: ",16);
			write(STDERR_FILENO, itos(errno), strlen(itos(errno)));
			write(STDERR_FILENO, "\n", 1);
		}
	}
}
void s_cd(char ** argv, int * cmd_index, int i){
	if(chdir(argv[cmd_index[i]+1])==-1){
		if(errno == EACCES){
			write(STDERR_FILENO, "cd: Permission denied\n", 22);
		}else if(errno == EISDIR){
			write(STDERR_FILENO, "cd: Is a directory\n",19);
		}else if(errno == ENOENT){
			write(STDERR_FILENO, "cd: No such file or directory\n",30);
		}else if(errno == ENOTDIR){
			write(STDERR_FILENO, "cd: Not a directory\n",20);
		}else if(errno == EPERM){
			write(STDERR_FILENO, "cd: Permission denied\n",22);
		}else{
			write(STDERR_FILENO, "Error occurred: ",16);
			write(STDERR_FILENO, itos(errno), strlen(itos(errno)));
			write(STDERR_FILENO, "\n", 1);
		}
	}
}
void s_pwd(){
	char buffer[1024];
	getcwd(buffer, 1024);

	write(STDOUT_FILENO, buffer, sizeof(char)*strlen(buffer));
	write(STDOUT_FILENO, "\n", 1);
}
void s_exit(char **argv, int * cmd_index, int j){
	int ret =0;

	write(2, "exit\n",5);
	if(argv[cmd_index[j]+1]!='\0'){
		ret = stoi(argv[cmd_index[j]+1]);
		exit(ret);
	}else{
		exit(EXIT_SUCCESS);
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
char* itos(int s){
	int s_length=0;
	int i;
	int result=s;
	char string[1000];
	char* string_zero[1];

	if(s==0){
		string_zero[0]='0';
		return string_zero;
	}
	while(s>0){
		s = s/10;
		s_length++;
	}
	for(i=s_length-1;i>=0;i--){
		string[i]=result%10 + '0';
		result = result/10;
	}
	string[s_length]='\0';
	return string;
}
char * my_strchr(const char *str, int c)
{
    char *pstr = str;

    char temp;

    if(c == 0)
    {
        while(*pstr++);

        return pstr - 1;
    }

    while(temp = *pstr++)
    {
        if(temp == c)
        {
            return pstr - 1;
        }
    }
    return 0;
}
int strncmp(const char *s1, const char *s2, size_t n){
	unsigned char *ps1, *ps2;
	int i;
	ps1 = (unsigned char*)s1;
	ps2 = (unsigned char *)s2;

	for(i=0;i<=n;i++){
		if((*ps1)!=(*ps2)){
			if((*ps1)>(*ps2)){
				return 1;
			}else{
				return -1;
			}
		}
		if((*ps1) == 0){
			return 0;
		}
		*ps1++;
		*ps2++;
	}
	return 0;
}

