//-----------------------------------------------------------
// 
// SWE2007: Software Experiment II (Fall 2015)
//
// Skeleton code for PA #1
// September 16, 2015
//
// Woo-Yeong Jeong
// Computer Systems Laboratory
// Sungkyunkwan University
//
//Taeha Hong , 2014313303
//-----------------------------------------------------------

#include "convert.h"
#include <fcntl.h>
#include <errno.h>
int convert(const char *input)

{
	int fd,fd2;
	int i=0;
	char buf[1]={0,};	
	int countU=0;
	int countD=0;
	int nbytes;
	char *name;
	int length;
	int flag=0;
	int size;
	char* error="Error occurred!\n";
	int e_length;
	length = strlength(input);
	name = (char *)malloc(sizeof(char)*(length+4));
	for(i=0;i<length;i++){
		name[i]=input[i];
	}
	name[length]='.';
	name[length+1]='o';
	name[length+2]='u';
	name[length+3]='t';

	e_length = strlength(error);


	fd = open(input, O_RDONLY);
	if(errno==2){
		write(2, error, sizeof(char)*e_length);
		return 1;
	}else if(errno==13){
		write(2, error, sizeof(char)*e_length);
		return 2;
	}
	
	fd2 = open(name, O_WRONLY | O_CREAT | O_EXCL, 0644);

	if(errno==17){
		write(2, error, sizeof(char)*e_length);
		close(fd);
		close(fd2);
		return 3;
	}else if(errno==13){
		write(2, error, sizeof(char)*e_length);
		close(fd);
		close(fd2);
		return 4;
	}

	if(errno>0){
		write(2, error, sizeof(char)*e_length);
		exit(1);
	}

	nbytes = read(fd, buf, sizeof(buf));

	while(read(fd, buf, sizeof(buf))>0){

		if(buf[0]=='\r'){
			flag=1;
			continue;	
		}
		if(flag==1 && buf[0]=='\n'){
			countD++;
		}
		if(flag==0 && buf[0]=='\n'){
			countU++;
		}
		flag=0;
	}
	lseek(fd, 0, SEEK_SET);

	if(countU>countD){
		size = U2D(input, fd, fd2);
	}else{
		size = D2U(input, fd, fd2);
	}
	print(input, name, size);

	close(fd);
	free(name);
	name = '\0';

	return 0;
}
void print(char *input, char *name, int o_size){
	int length, n_length;
	char* input_file;
	char* output_file;
	char* size;
	int i,j;
	int s_length=0;
	int s=o_size;

	length = strlength(input);
	n_length = strlength(name);

	input_file = (char *)malloc(sizeof(char)*(length+7));
	input_file[0]='I';
	input_file[1]='n';
	input_file[2]='p';
	input_file[3]='u';
	input_file[4]='t';
	input_file[5]=':';
	for(i=0;i<length;i++){
		input_file[i+6]=input[i];
	}
	input_file[length+6]=' ';

	output_file = (char *)malloc(sizeof(char)*(n_length+8));
	output_file[0]='O';
	output_file[1]='u';
	output_file[2]='t';
	output_file[3]='p';
	output_file[4]='u';
	output_file[5]='t';
	output_file[6]=':';
	for(i=0;i<n_length;i++){
		output_file[i+7]=name[i];
	}
	output_file[n_length+7]=' ';

	while(s>0){
		s = s/10;
		s_length++;
	}

	size = (char *)malloc(sizeof(char)*(s_length+6));
	size[0]='S';
	size[1]='i';
	size[2]='z';
	size[3]='e';
	size[4]=':';

	for(i=s_length+4;i>4;i--){
		size[i]=o_size%10 + '0';
		o_size = o_size/10;
		
	}
	size[s_length+5]='\n';

	write(1, input_file, sizeof(char) * (length+7));
	write(1, output_file, sizeof(char)*(n_length+8));
	write(1, size, sizeof(char)*(s_length+6));	

	free(input_file);
	free(output_file);
	free(size);
	input_file='\0';
	output_file='\0';
	size='\0';
}
int strlength(char *string){
	int length=1;
	
	while(string[length]!='\0'){
		length++;
	}

	return length;
}
int U2D(const char *input, int fd, int fd2){
	char u2d_buf[1]={0};
	int i;
	int j=0;
	int k;
	int size=0;
	int r_count=0;
	char* trim;
	int r_size;
	char* temp;

	while(read(fd, u2d_buf, 1)>0){

		if(u2d_buf[0]=='\r'){
			r_count++;
			continue;
		}
		if(r_count!=0){
			for(i=0;i<r_count;i++){
				size++;
			}

		}
		if(u2d_buf[0]=='\n' && r_count==0){
			size++;
		}
		
		if(u2d_buf[0]=='\t'){
			for(i=0;i<4;i++){
				size++;
			}
			continue;
		}
		r_count=0;
		size++;
	}
	lseek(fd, 0, SEEK_SET);	
	trim = (char*)malloc(sizeof(char)*size);
	temp = (char*)malloc(sizeof(char)*size);
	i=0;
	j=0;
	while(read(fd, u2d_buf, 1)>0){
		if(u2d_buf[0]=='\r'){
			r_count++;
			temp[j]='\r';
			j++;
			continue;
		}
		if(r_count!=0){
			for(k=0;k<j;k++){
				trim[i]=temp[k];
				i++;
			}
		
		}
		if(u2d_buf[0]=='\n' && r_count==0){
			trim[i]='\r';
			i++;
		}
		
		if(u2d_buf[0]=='\t'){
			//for(j=0;j<4;j++){
			for(k=0;k<4;k++){
				trim[i]=' ';
				i++;
			}
			continue;
		}

		j=0;
		r_count=0;
		trim[i]=u2d_buf[0];
		i++;
	}
	r_size = U2D_trimming(trim, fd2, size);

	close(fd2);
	free(temp);
	free(trim);
	trim='\0';
	temp='\0';

	return r_size;
}
int  D2U(const char *input, int fd, int fd2){
	char d2u_buf[1]={0,};
	int i;
	int s_count=0;
	int r_count=0;
	int n=1;
	int len;
	int size=0;
	char* trim;
	int r_size;

	puts("D2U");
	len = strlength(input);

	while(read(fd, d2u_buf, 1)>0){
		if(d2u_buf[0]=='\r'){
			r_count++;
			continue;
		}
		if(r_count==1){
			if(d2u_buf[0]!='\n'){
				size++;
			}
		}

		if(((input[len-1]=='c' || input[len-1]=='h') && input[len-2]=='.')  ||
		(input[len-1]=='a' && input[len-2]=='v' && input[len-3]=='a'  && input[len-4]=='j' && input[len-5]=='.')){

			if(d2u_buf[0]==' '){
				s_count++;
				if(s_count==(8*n)){
					size++;
				}
				if(s_count>(8*n)){
					n++;
				}
				continue;
			}
			if(s_count!=0){
				if(s_count<8){
					for(i=0;i<s_count;i++){
						size++;
					}
				} else if(s_count>(8*(n-1)) && n>1 && s_count!=(8*n)){
					for(i=0;i<s_count-(8*(n-1));i++){
						size++;
					}
				}
			}
			n=1;
			s_count=0;
		}
		r_count=0;
		size++;
	}
	lseek(fd, 0, SEEK_SET);	
	trim = (char*)malloc(sizeof(char)*size);
	i=0;
	while(read(fd, d2u_buf, 1)>0){
		if(d2u_buf[0]=='\r'){
			r_count++;
			continue;
		}
		if(r_count==1){
			if(d2u_buf[0]!='\n'){
				trim[i]='\r';
				i++;
			}
		}

		if(((input[len-1]=='c' || input[len-1]=='h') && input[len-2]=='.')  ||
		(input[len-1]=='a' && input[len-2]=='v' && input[len-3]=='a'  && input[len-4]=='j' && input[len-5]=='.')){

			if(d2u_buf[0]==' '){
				s_count++;
				if(s_count==(8*n)){
					trim[i]='\t';
					i++;
				}
				if(s_count>(8*n)){
					n++;
				}
				//temp[j]=' ';
				//j++;
				continue;
			}
			if(s_count!=0){
				if(s_count<8){
					for(i=0;i<s_count;i++){
						trim[i]=' ';
						i++;
					}
				} else if(s_count>(8*(n-1)) && n>1 && s_count!=(8*n)){
					for(i=0;i<s_count-(8*(n-1));i++){
						trim[i]=' ';
						i++;
					}
				}
			}
			n=1;
			s_count=0;
		}
		r_count=0;
		trim[i]=d2u_buf[0];
		i++;
	}
	r_size = D2U_trimming(trim, fd2, size);

	close(fd2);
	free(trim);
	trim='\0';

	return r_size;
}
int U2D_trimming(char* arr, int fd2, int size){
	int i=0;
	int j=0;
	int k;
	int count=0;
	int countR=0;
	int r_size=0;
	char* temp;

	temp = (char*)malloc(sizeof(char)*size);
	for(i=0;i<size;i++){
		if(arr[i]=='\t'){
			count++;
			temp[j]='\t';
			j++;
			continue;
		}else if(arr[i]==' '){
			count++;
			temp[j]=' ';
			j++;
			continue;
		}
		
		if((count!=0 && arr[i]=='\r')){
			countR++;
			temp[j]='\r';
			j++;
			continue;
		}
		

		if(!(countR==1 && count!=0 && arr[i]=='\n')){
			for(k=0;k<j;k++){
				r_size++;
				write(fd2, temp+k, 1);
			}
		}
		if(countR==1 && arr[i]=='\n'){
			r_size++;
			write(fd2, "\r", 1);
		}

		j=0;
		count=0;
		countR=0;
		r_size++;
		write(fd2, arr+i, 1);
	}
	return r_size;
}
int D2U_trimming(char* arr, int fd2, int size){
	int i=0;
	int j=0;
	int k;
	int countT=0;
	int countS=0;
	int r_size=0;
	char* temp;

	temp = (char*)malloc(sizeof(char)*size);
	for(i=0;i<size;i++){
		if(arr[i]=='\t'){
			countT++;
			temp[j]='\t';
			j++;
			continue;
		}else if(arr[i]==' '){
			countS++;
			temp[j]=' ';
			j++;
			continue;
		}
		if((countT!=0 && arr[i]!='\n') || (countS!=0 && arr[i]!='\n')){
			for(k=0;k<j;k++){
				r_size++;
				write(fd2, temp+k, 1);
			}
		}

		j=0;
		countT=0;
		countS=0;
		r_size++;
		write(fd2, arr+i, 1);
	}
	return r_size;
}
