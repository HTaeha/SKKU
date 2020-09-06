//2014313303, Taeha Hong

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
char* itos();
void stat();
void collect()
{
	int fd,fd2,fd3,fd4;
	char* string;
	char* input;  //store path or command input
	int input_size;
	char buf[4096]={0,};
	int i;
	char* outfile;  //make file , file name is seq
	int seq=1;   // processing sequence
	int count_path=0;   // counting path input
	int not_path=0;
	int bs_count=0;
	char* inputfile = "InputFile";
	char* path;
	int print_flag=0;
	int child_status;
	char* err_string;

	fd2 = open(inputfile,  O_WRONLY| O_CREAT | O_TRUNC, 0666);
	while(1){
		signal(SIGINT, SIG_IGN);
		signal(SIGUSR1,stat);
	//path or command input
		input_size=read(0,buf,sizeof(buf)/sizeof(char))-1;
		input = (char *)malloc(sizeof(char)*(input_size));
		
		for(i=0;i<input_size;i++){
			input[i]=buf[i];
		}
		
		
	//	
		outfile = itos(seq);
	
	//district command
		print_flag=0;
		
		if(input[0]=='p' && input[1]=='r' && input[2]=='i' && input[3]=='n' && input[4]=='t' && input[5]==' '){
			path = (char*)malloc(sizeof(char)*(input_size-6));
			for(i=0;i<input_size-6;i++){
				path[i] = input[i+6];
			}
			print_flag=1;
		}

		if(input[0]=='s'&&input[1]=='t'&&input[2]=='a'&&input[3]=='t' && input_size==4){
			stat(count_path, seq);
			seq++;
			print_flag=1;
			continue;
		}else if(input[0]=='l'&&input[1]=='o'&&input[2]=='a'&&input[3]=='d' && input[4]==' '){
			load();
			seq++;
			print_flag=1;
			continue;
		}else if(input[0]=='q'&&input[1]=='u'&&input[2]=='i'&&input[3]=='t' && input_size==4){
			print_flag=1;
			close(fd2);
			quit();
		}else{
			if(!is_path(input)){
				continue;
			}else{
				if(print_flag==0){
					write(fd2, input, sizeof(char)*input_size);
					write(fd2, ":", 1);
					write(fd2, outfile, sizeof(char)*strlength(outfile));
					write(fd2, "\n", 1);

				}
			}
		}

		if(print_flag==1){
			fd3 = open(inputfile, O_RDONLY);
			op_print(path, fd3,seq);
			seq++;
			print_flag=0;
			continue;
		}
		if(fork() == 0)
		{
			fd = open(outfile, O_WRONLY| O_CREAT | O_TRUNC, 0666);
			dup2(fd, STDOUT_FILENO);
			fd4 = open("/dev/null", O_APPEND);
			dup2(fd4, STDERR_FILENO);
			close(fd);
			close(fd4);
			execl("/bin/ls","ls",input,"-l", NULL);
			
		}
		else
		{
			wait(&child_status);
			if(child_status==0){
				print(outfile, input, seq);	
				seq++;
				count_path++;
			}else{
				if(fork() == 0){
					execl("/bin/rm","rm",outfile,NULL);
				}else{
					wait(&child_status);
					write(1,itos(seq), sizeof(char)*intlength(seq));
					write(1, ">", 1);
					write(1, "Error occurred!", 15);
					write(1, "\n", 1);
					seq++;
				}
			}
		}

	}
	close(fd2);

}
void op_print(char* path, int in_fd, int real_seq){
	char print_buf[1]={0,};
	int i=0;
	int flag=0;
	int not_contain=0;
	char* temp;
	int seq_flag=0;
	int seq=0;
	char* real_seq_string;
	char* not_message="Not Available";
	char* seq_string;
	temp = (char*)malloc(sizeof(char)*strlength(path));
	while(read(in_fd, print_buf, 1)>0){
		if(print_buf[0]=='\n'){
			flag=0;
			not_contain=0;
			seq_flag=0;
			seq=0;
			i=0;
			continue;
		}
		if(not_contain==1){
			continue;
		}	

		if(not_contain==0 && print_buf[0]==':'){
			
			if(i==strlength(path)){
				seq_flag=1;
				continue;
			}else{
				not_contain=1;
				continue;
			}
			
		}
		if(not_contain==0 && seq_flag==0){
			if(path[i]!=print_buf[0]){
				not_contain=1;
				
				continue;
			}else{
				temp[i] = print_buf[0];
				i++;
				continue;
			}
		}
	
		if(seq_flag==1){
			seq = seq*10 + (int)(print_buf[0]-'0');
		}
		if(not_contain==0){
			seq_string = itos(seq);
			print(seq_string,temp,real_seq);
			return;
		}
		
	
	}
	real_seq_string=itos(real_seq);
	write(1,real_seq_string ,sizeof(char)*intlength(real_seq));
	write(1, ">", 1);
	write(1, not_message, strlength(not_message));
	write(1, "\n", 2);

}
int is_path(char* input){
	int i;
	int bs_count=0;

		for(i=0;i<strlength(input);i++){
			if(input[i]=='<' || input[i]=='>' ||input[i]=='|' ||input[i]==92 ||input[i]==':' ||input[i]=='(' ||input[i]==')' ||input[i]=='&' ||input[i]==';' ||input[i]=='{' ||input[i]=='}' ||input[i]=='*' ||input[i]=='?'){

				if(i!=0){
					if(input[i-1]==92){
						if(input[i]!=92){
							continue;
						}else{
							continue;
						}
					}else{
						if(input[i]!=92){
							return 0;
						}else{
							return 0;
						}
					}	
				}else{
					if(input[i]!=92){
						return 0;
					}else{
						return 0;
					}
				}

			}
			if(bs_count==1){
				return 0;
			}
			bs_count=0;
		}

	return 1;
}
void stat(int count_p, int stat_seq){
	char* stat_string;
	int stat_size;
	char* pcount_string;
	char* seq_string;
	int pcount_len;
	int seq_len;
	int i;
	
	pcount_string = itos(count_p);
	seq_string = itos(stat_seq);

	pcount_len=intlength(count_p);
	seq_len=intlength(stat_seq);

	stat_size=pcount_len+seq_len+8;
	stat_string = (char *)malloc(sizeof(char)*stat_size);

	for(i=0;i<seq_len;i++){
		stat_string[i] = seq_string[i];
	}
	stat_string[seq_len] = '>';
	for(i=0;i<pcount_len;i++){
		stat_string[seq_len+1+i] = pcount_string[i];
	}
	stat_string[seq_len+pcount_len+1] = ' ';
	stat_string[seq_len+pcount_len+2] = 'p';
	stat_string[seq_len+pcount_len+3] = 'a';
	stat_string[seq_len+pcount_len+4] = 't';
	stat_string[seq_len+pcount_len+5] = 'h';
	stat_string[seq_len+pcount_len+6] = 's';
	stat_string[seq_len+pcount_len+7] = '\n';

	write(1,stat_string,sizeof(char)*(stat_size));
}
void quit(){
	kill(0,9);
}
void load(){}
void print(char* outfile, char* input,int seq){
	int fd2;
	int pnum=0;
	int psize=0;
	char* pnum_string;
	char* psize_string;
	char* seq_string;
	char print_buf[1]={0,};
	int d_flag=0;
	int n_count=0;
	int n_flag=0;
	int s_count=0;
	int psize_string_length=0;
	int end_flag=0;
	int i=0;
	int psize_flag=0;
	int flag=0;
	int d_count=0;
	char* output;
	int output_size;
	int temp_psize=0;

	fd2 = open(outfile, O_RDONLY);

	while(read(fd2, print_buf, 1)>0){
		if(print_buf[0]=='\n'){
			n_count++;
			s_count=0;
			psize_flag=0;
			d_flag=0;
		}
		if(n_count>=1){
			if(print_buf[0]=='\n'){
				n_flag=1;
				continue;
			}
			if(n_flag==1 && print_buf[0]=='d'){
				d_flag=1;
				d_count++;
			}
		}
		if(d_flag==1){
			n_flag=0;
			continue;
		}
		if(n_flag==1){
			psize_flag=1;
		}
		if(psize_flag==1){
			if(print_buf[0]==' '){
				s_count++;
			}
			if(s_count>=4){
				if(print_buf[0]==' '){
					if(flag==1){
						psize_flag=0;
						flag=0;
						psize = psize + temp_psize;
						temp_psize = 0;
					}
					continue;
				}	
				temp_psize = temp_psize*10 + (int)(print_buf[0]-'0');
				flag=1;
			}
		}
		n_flag=0;
	}

	pnum=n_count-1;
	seq_string = itos(seq);
	pnum_string = itos(pnum);
	psize_string = itos(psize);

	output_size=intlength(seq)+strlength(input)+intlength(pnum)+intlength(psize)+4;
	output = (char*)malloc(sizeof(char)*output_size);

	for(i=0;i<intlength(seq);i++){
		output[i]=seq_string[i];
	}
	output[intlength(seq)]='>';
	for(i=0;i<strlength(input);i++){
		output[i+intlength(seq)+1]=input[i];
	}
	output[intlength(seq)+1+strlength(input)]=':';
	for(i=0;i<intlength(pnum);i++){
		output[i+intlength(seq)+1+strlength(input)+1] = pnum_string[i];
	}
	output[intlength(seq)+1+strlength(input)+1+intlength(pnum)]=':';
	for(i=0;i<intlength(psize);i++){
		output[i+intlength(seq)+1+strlength(input)+1+intlength(pnum)+1]=psize_string[i];
	}
	output[intlength(seq)+1+strlength(input)+1+intlength(pnum)+1+intlength(psize)]='\n';

	write(1, output, sizeof(char)*output_size);

	close(fd2);
}
int strlength(char *string){
	int length=1;
	
	while(string[length]!='\0'){
		length++;
	}

	return length;
}
int intlength(int num){
	int length=0;

	if(num==0){
		length++;
	}
	while(num>0){
		num = num/10;
		length++;
	}
	return length;
}
char* itos(int s){
	int s_length=0;
	int i;
	int result=s;
	char* string;
	char* string_zero[1];

	if(s==0){
		string_zero[0]='0';
		return string_zero;
	}
	while(s>0){
		s = s/10;
		s_length++;
	}
	string = (char *)malloc(sizeof(char)*s_length);
	for(i=s_length-1;i>=0;i--){
		string[i]=result%10 + '0';
		result = result/10;
	}
	return string;
}
