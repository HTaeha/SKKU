#include <stdio.h>
#include <math.h>
struct quadruple{
   char data[16];
};
typedef struct quadruple quadruple;
typedef unsigned char *byte_pointer;

typedef union{
   double num;
   unsigned char su[8];
   unsigned long long n;
}doub;
typedef union{
   double num;
   unsigned char su[8];
   long long n;
}quad;
void show_bytes(byte_pointer start, int len);
void ToBinary(double, char*);
quadruple mult(double num1, double num2);
char sign;
char frac[52];

int main(){
   doub double_num1, double_num2;
   char binary_num1[128];
   char binary_num2[128];
   double double_n1,double_n2;
   int i;

   scanf("%f", &double_n1);
   scanf("%f", &double_n2);

   double_num1.num=double_n1;
   double_num2.num=double_n2;

   mult(double_num1.num, double_num2.num);

   return 0;
}
void show_bytes(byte_pointer start, int len){
   int i;
   for(i=0;i<len;i++){
      printf("%.2x", start[i]);
   }
   printf("\n");
}

void ToBinary(double num, char* binary_num){

   doub double_num;

	unsigned char mask;
   int i, j, k;
   double_num.num = num;


	for(i = 0 ; i < 128 ; i++)
		binary_num[i] = 0;

   for(i=7;i>=0;i--){
	   mask = 0x80;
	   printf("%x\n",double_num.su[i]);
	   for(k = 0 ; k < 8 ; k++){
			if((double_num.su[i] & mask) != 0){
				binary_num[i*8 + k]++;
			}
			mask >>= 1;
		}

		printf("[");
		for(j = 0 ; j < 8 ; j++)
			printf("%d",binary_num[i*8 + j]);

		printf("]\n");

   }
}

quadruple mult(double num1, double num2){
	quadruple quadruple;
	doub mantisa,exp,n1,n2;
	char quadnum[128];
	char binary_num1[128];
    char binary_num2[128];
	char arr_mat_num1[128];
	char arr_mat_num2[128];
	char arr_mat_num[128];
	int i,j;
	int num1_exp=-1023;
	int	num2_exp=-1023;
	double mat_num1=1;
	double mat_num2=1;
	unsigned char mask,mask1;

	ToBinary(num1,binary_num1);
    ToBinary(num2,binary_num2);

	n1.num=num1;
	n2.num=num2;

	arr_mat_num1[75]=1;
	arr_mat_num2[75]=1;
	mask = 0x40;                            //double input normalize인지 denormalize인지 check
	mask1 = 0x80;
	while(1){
		if(n1.su[7]&mask !=0){
			break;
		}
		mask>>=1;
		if(n1.su[6]&mask1 !=0){
			break;
		}
		if(mask1==0x10){
			mat_num1=0;
			arr_mat_num1[75]=0;
		}
		mask1>>=1;
	}
	while(1){
		if(n2.su[7]&mask !=0){
			break;
		}
		mask>>=1;
		if(n2.su[6]&mask1 !=0){
			break;
		}
		if(mask1==0x10){
			mat_num2=0;
			arr_mat_num2[75]=0;
		}
		mask1>>=1;
	}

	sign=(n1.su[7]&0x80)^(n2.su[7]&0x80);			//sign부분: 두 인풋의 sign부분을 xor해서 대입

	
	mask=0x40;										//exponent부분 : 두 인풋의 exponent부분을 십진수로 바꾸어서 대입
	for(j=0;j<7;j++){
		num1_exp=num1_exp + (n1.su[7]&mask)*pow(2.0,10-j);
		num2_exp=num2_exp + (n2.su[7]&mask)*pow(2.0,10-j);
		mask>>=1;
	}
	mask=0x80;
	for(j=0;j<4;j++){
		num1_exp=num1_exp + (n1.su[6]&mask)*pow(2.0,3-j);
		num2_exp=num2_exp + (n2.su[6]&mask)*pow(2.0,3-j);
		mask>>=1;
	}
	exp.num = num1_exp + num2_exp;
	exp.n=exp.n+16383;

	for(i=0;i<128;i++){                           //mantisa char[128]행렬에 끝에서부터 대입
		arr_mat_num1[i]=0;
		arr_mat_num2[i]=0;
	}
	for(j=0;j<6;j++){
		mask=0x01;
		for(i=0;i<8;i++){
			if(n1.su[j]&mask==1){
				arr_mat_num1[127-i-8*j]++;
				mask<<1;
			}
			if(n2.su[j]&mask==1){
				arr_mat_num2[127-i-8*j]++;
				mask<<1;
			}
		}
	}
	mask=0x10;
	for(i=0;i<4;i++){
		if(n1.su[6]%mask==1){
			arr_mat_num1[79-i]++;
		}
		if(n2.su[6]%mask==1){
			arr_mat_num2[79-i]++;
		}
	}


	mask=0x80;
	for(i=6;i>=0;i--){
		if(n1.su[i]&mask==1){
			
		}
		
	}
	for(j=4;j<8;j++){                              //mantisa 부분을 십진수로 바꿔서 저장
			mat_num1 = mat_num1 + n1.su[6]*(pow(2.0,-(j-3)));
			mat_num2 = mat_num2 + n2.su[6]*(pow(2.0,-(j-3)));
		}
	for(i=5;i>=0;i--){
		for(j=0;j<8;j++){
			mat_num1 = mat_num1 + n1.su[i]*(pow(2.0, (8*i-j-45)));
			mat_num2 = mat_num2 + n2.su[i]*(pow(2.0, (8*i-j-45)));
		}
	}

	if(sign==0){                                  //quadruple 첫번째 자리에 sign비트 넣음
		quadruple.data[0]=0x00;
	}else{
		quadruple.data[0]=0x80;
	}

	mask=0x40;									  //quadruple exp넣기 data[0]
	for(i=0;i<7;i++){
		if(exp.n%2==1){
			quadruple.data[0]=quadruple.data[0]|mask;
			mask>>1;
		}
	}
	mask = 0x80;									//quadruple exp넣기 data[1]
	for(i=0;i<8;i++){
		if(exp.n%2==1){
			quadruple.data[1]=quadruple.data[0]|mask;
			mask>>1;
		}
	}

	return quadruple
}