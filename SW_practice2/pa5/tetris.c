//PA 5
//2014313303
//Taeha Hong
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

#include <time.h>
#include "term_control.h"

#define MSEC 1000

enum { KEY_CHANGED_NONE = 0, KEY_CHANGED_OK, KEY_CHANGED_QUIT };

static int posX;
static int posY;
static int key_changed = KEY_CHANGED_NONE;
static int shapeNo;
static int rotationNo;
int keep = -1;
int restart_flag = 0;
int over_flag = 0;
int pause_flag=0;
int stage_num=1;
int score=0;
int end_flag=0;
char tetris[22][12] = { \
	' ','-','-','-','-','-','-','-','-','-','-',' ',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	'|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','|',\
	' ','-','-','-','-','-','-','-','-','-','-',' '\
};

void *UI_Work(void *arg);
void *Key_Work(void *arg);
void *Gravity_Work();

void printScreen();
int main()
{
	pthread_t ui_thread, key_thread, gravity_thread;

	signal(SIGINT, SIG_IGN);
	srand((unsigned)time(NULL));
	shapeNo = rand() % 7;

	clearScreen();
	printScreen();
	pthread_create(&ui_thread, NULL, UI_Work, NULL);
	pthread_create(&key_thread, NULL, Key_Work, NULL);
	pthread_create(&gravity_thread, NULL, Gravity_Work, NULL);

	pthread_join(key_thread, NULL);
	pthread_join(ui_thread, NULL);
	pthread_join(gravity_thread, NULL);

	return 0;
}
void *Gravity_Work(){
	while(1){
		
		if(pause_flag == 1 || over_flag == 1){
			if(key_changed == KEY_CHANGED_QUIT){
				pthread_exit(0);
			}
		}else if(score <=200){
			sleep(1);
			posY++;
			stage_num = 1;
			printScreen();
		}else if(score <=500){
			usleep(1000*1000*0.8);
			posY++;
			stage_num = 2;
			printScreen();
		}else if(score <=1000){
			usleep(1000*1000*0.6);
			posY++;
			stage_num = 3;
			printScreen();
		}else if(score <=2000){
			usleep(1000*1000*0.4);
			posY++;
			stage_num = 4;
			printScreen();
		}else{
			usleep(1000*1000*0.2);
			posY++;
			stage_num = 5;
			printScreen();
		}
	}
}
void printScreen()
{
	int i,j,k;
	int count = 0;
	int index_len = 0;
	int index[4] = {0,};
	


	clearScreen();

	if(restart_flag == 1){
		tetris[0][0]=' ';
		tetris[0][11]=' ';
		tetris[21][0]=' ';
		tetris[21][11]=' ';
		for(j=1;j<11;j++){
			tetris[0][j]='-';
			tetris[21][j]='-';
		}
		
		for(i=1;i<21;i++){
			tetris[i][0]='|';
			tetris[i][11]='|';
		}
		for(i=1;i<21;i++){
			for(j=1;j<11;j++){
				tetris[i][j]=' ';
			}
		}
		score = 0;
		stage_num = 1;
		posY = 0;
		posX = 0;
		shapeNo = rand() % 7;
		restart_flag = 0;
	}
	printf("Stage : %d\n",stage_num);
	printf("Score : %d\n",score);
	
	if(rotationNo==0){
		for(i=0; i<22; i++){
			for(j=0; j<12; j++){
				
				
				
				if(shapeNo == 0){
					setColor(0);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+3 && j == posX+1){
						printf("x");
					}else if(i == posY+4 && j == posX+1){
						printf("x");
					}else if(i == posY+5 && j == posX+1){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+3][posX+1] = 'x';
							tetris[posY+4][posX+1] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
						
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}else if(shapeNo == 1){
					setColor(1);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+3){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2) || (i == posY+3 && j == posX+3) ){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+2][posX+3] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
						
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}else if(shapeNo == 2){
					setColor(2);
					if(i == posY+1 && j == posX+3){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+3){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2) || (i == posY+3 && j == posX+3) ){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+3] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+2][posX+3] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
						
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}else if(shapeNo == 3){
					setColor(3);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
						
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}else if(shapeNo == 4){
					setColor(4);
					if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+3){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2) || (i == posY+3 && j == posX+3) ){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+2][posX+3] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
						
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}else if(shapeNo == 5){
					setColor(5);
					if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+1 && j == posX+3){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2) || (i == posY+2 && j == posX+3) ){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+1][posX+3] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
						
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}else if(shapeNo == 6){
					setColor(6);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+3){
						printf("x");
					}else if((i == posY+2 && j == posX+1) || (i == posY+3 && j == posX+2) || (i == posY+3 && j == posX+3) ){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+2][posX+3] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
						
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(j==11){
					if(i==4){
						printf(" ");printf("-");printf("k");printf("e");printf("e");printf("p");printf("-");printf(" ");
					}
					if(i==9){
						printf(" ");printf("-");printf("-");printf("-");printf("-");printf("-");printf("-");printf(" ");
					}
					if(i==5){
						printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
					}
					if(i==8){
						printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
					}
					if(keep == 0){
						if(i==6){
							printf("|");printf(" ");setColor(0);printf("x");printf("x");printf("x");printf("x");setColor(8);printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 1){
						
						if(i==6){
							printf("|");printf(" ");setColor(1);printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(1);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 2){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");printf(" ");setColor(2);printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(2);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 3){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(3);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");setColor(3);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 4){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(4);printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(4);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 5){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(5);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(5);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 6){
						
						if(i==6){
							printf("|");printf(" ");setColor(6);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");setColor(6);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == -1){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}
					}
				}
			}
			printf("\n");
		}
	}else if(rotationNo==1){
		for(i=0; i<22; i++){
			for(j=0; j<12; j++){
				
				if(shapeNo == 0){
					setColor(0);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+1 && j == posX+3){
						printf("x");
					}else if(i == posY+1 && j == posX+4){
						printf("x");
					}else if((i == posY+2 && j == posX+1) || (i == posY+2 && j == posX+2) || (i == posY+2 && j == posX+3) || (i == posY+2 && j == posX+4)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+1][posX+3] = 'x';
							tetris[posY+1][posX+4] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 1){
					setColor(1);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+3 && j == posX+1){
						printf("x");
					}else if((i == posY+4 && j == posX+1) || (i == posY+2 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+3][posX+1] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 2){
					setColor(2);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+3 && j == posX+1){
						printf("x");
					}else if(i == posY+3 && j == posX+2){
						printf("x");
					}else if((i == posY+4 && j == posX+1) || (i == posY+4 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+3][posX+1] = 'x';
							tetris[posY+3][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 3){
					setColor(3);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 4){
					setColor(4);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+1){
						printf("x");
					}else if((i == posY+4 && j == posX+1) || (i == posY+3 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+3][posX+1] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 5){
					setColor(5);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+4 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+3][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 6){
					setColor(6);
					if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+1){
						printf("x");
					}else if((i == posY+4 && j == posX+1) || (i == posY+3 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+3][posX+1] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(j==11){
					if(i==4){
						printf(" ");printf("-");printf("k");printf("e");printf("e");printf("p");printf("-");printf(" ");
					}
					if(i==9){
						printf(" ");printf("-");printf("-");printf("-");printf("-");printf("-");printf("-");printf(" ");
					}
					if(i==5){
						printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
					}
					if(i==8){
						printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
					}
					if(keep == 0){
						if(i==6){
							printf("|");printf(" ");setColor(0);printf("x");printf("x");printf("x");printf("x");setColor(8);printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 1){
						
						if(i==6){
							printf("|");printf(" ");setColor(1);printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(1);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 2){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");printf(" ");setColor(2);printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(2);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 3){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(3);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");setColor(3);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 4){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(4);printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(4);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 5){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(5);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(5);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 6){
						
						if(i==6){
							printf("|");printf(" ");setColor(6);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");setColor(6);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == -1){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}
					}
				}
			}
			printf("\n");
		}
	}else if(rotationNo==2){
		for(i=0; i<22; i++){
			for(j=0; j<12; j++){
				
				if(shapeNo == 0){
					setColor(0);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+3 && j == posX+1){
						printf("x");
					}else if(i == posY+4 && j == posX+1){
						printf("x");
					}else if((i == posY+5 && j == posX+1)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+3][posX+1] = 'x';
							tetris[posY+4][posX+1] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 1){
					setColor(1);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+1 && j == posX+3){
						printf("x");
					}else if(i == posY+2 && j == posX+3){
						printf("x");
					}else if((i == posY+2 && j == posX+1) || (i == posY+2 && j == posX+2) || (i == posY+3 && j == posX+3)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+1][posX+3] = 'x';
							tetris[posY+2][posX+3] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 2){
					setColor(2);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+1 && j == posX+3){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+2 && j == posX+2) || (i == posY+2 && j == posX+3)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+1][posX+3] = 'x';
							tetris[posY+2][posX+1] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 3){
					setColor(3);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 4){
					setColor(4);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+1 && j == posX+3){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if((i == posY+2 && j == posX+1) || (i == posY+3 && j == posX+2) || (i == posY+2 && j == posX+3)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+1][posX+3] = 'x';
							tetris[posY+2][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 5){
					setColor(5);
					if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+1 && j == posX+3){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2) || (i == posY+2 && j == posX+3)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+1][posX+3] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 6){
					setColor(6);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+3){
						printf("x");
					}else if((i == posY+2 && j == posX+1) || (i == posY+3 && j == posX+2) || (i == posY+3 && j == posX+3)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+2][posX+3] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(j==11){
					if(i==4){
						printf(" ");printf("-");printf("k");printf("e");printf("e");printf("p");printf("-");printf(" ");
					}
					if(i==9){
						printf(" ");printf("-");printf("-");printf("-");printf("-");printf("-");printf("-");printf(" ");
					}
					if(i==5){
						printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
					}
					if(i==8){
						printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
					}
					if(keep == 0){
						if(i==6){
							printf("|");printf(" ");setColor(0);printf("x");printf("x");printf("x");printf("x");setColor(8);printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 1){
						
						if(i==6){
							printf("|");printf(" ");setColor(1);printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(1);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 2){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");printf(" ");setColor(2);printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(2);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 3){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(3);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");setColor(3);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 4){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(4);printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(4);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 5){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(5);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(5);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 6){
						
						if(i==6){
							printf("|");printf(" ");setColor(6);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");setColor(6);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == -1){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}
					}
				}
			}
			printf("\n");
		}
	}else if(rotationNo==3){
		for(i=0; i<22; i++){
			for(j=0; j<12; j++){
				
				if(shapeNo == 0){
					setColor(0);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+1 && j == posX+3){
						printf("x");
					}else if(i == posY+1 && j == posX+4){
						printf("x");
					}else if((i == posY+2 && j == posX+1) || (i == posY+2 && j == posX+2) || (i == posY+2 && j == posX+3) || (i == posY+2 && j == posX+4)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+1][posX+3] = 'x';
							tetris[posY+1][posX+4] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 1){
					setColor(1);
					if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+1){
						printf("x");
					}else if((i == posY+4 && j == posX+1) || (i == posY+4 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+3][posX+2] = 'x';
							tetris[posY+3][posX+1] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 2){
					setColor(2);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+2){
						printf("x");
					}else if((i == posY+2 && j == posX+1) || (i == posY+4 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+3][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 3){
					setColor(3);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+3 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 4){
					setColor(4);
					if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+4 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+3][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 5){
					setColor(5);
					if(i == posY+1 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+2){
						printf("x");
					}else if((i == posY+3 && j == posX+1) || (i == posY+4 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+1] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+3][posX+2] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(shapeNo == 6){
					setColor(6);
					if(i == posY+1 && j == posX+2){
						printf("x");
					}else if(i == posY+2 && j == posX+1){
						printf("x");
					}else if(i == posY+2 && j == posX+2){
						printf("x");
					}else if(i == posY+3 && j == posX+1){
						printf("x");
					}else if((i == posY+4 && j == posX+1) || (i == posY+3 && j == posX+2)){
						setColor(8);
						if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
							tetris[posY+1][posX+2] = 'x';
							tetris[posY+2][posX+1] = 'x';
							tetris[posY+2][posX+2] = 'x';
							tetris[posY+3][posX+1] = 'x';

							end_flag=1;
						}
						printf("%c",tetris[i][j]);
					}else{
						setColor(8);
						printf("%c", tetris[i][j]);
					}
				}
				if(j==11){
					if(i==4){
						printf(" ");printf("-");printf("k");printf("e");printf("e");printf("p");printf("-");printf(" ");
					}
					if(i==9){
						printf(" ");printf("-");printf("-");printf("-");printf("-");printf("-");printf("-");printf(" ");
					}
					if(i==5){
						printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
					}
					if(i==8){
						printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
					}
					if(keep == 0){
						if(i==6){
							printf("|");printf(" ");setColor(0);printf("x");printf("x");printf("x");printf("x");setColor(8);printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 1){
						
						if(i==6){
							printf("|");printf(" ");setColor(1);printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(1);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 2){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");printf(" ");setColor(2);printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(2);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 3){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(3);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");setColor(3);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 4){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(4);printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(4);printf("x");printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 5){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");setColor(5);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");setColor(5);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}
					}else if(keep == 6){
						
						if(i==6){
							printf("|");printf(" ");setColor(6);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");setColor(6);printf("x");printf("x");setColor(8);printf(" ");printf(" ");printf("|");
						}
					}else if(keep == -1){
						
						if(i==6){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}if(i==7){
							printf("|");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf(" ");printf("|");
						}
					}
				}
			}
			printf("\n");
		}

	}

	if(end_flag == 1){
		posX = 3;
		posY = -1;
		shapeNo = rand()%7;
		rotationNo = 0;
		end_flag = 0;
	}
	for(i=21;i>0;i--){
		for(j=0;j<12;j++){
			if(tetris[i][j] == 'x'){
				count++;
			}
			if(count == 10){
				index_len++;
				for(k=0;k<4;k++){
					if(index[k] == 0){
						index[k] = i;
						break;
					}
				}
				break;
			}
		}
		count = 0;
	}
	if(index_len == 1){
		score = score + 10; 
	}else if(index_len ==2){
		score = score + 40;
	}else if(index_len == 3){
		score = score + 120;
	}else if(index_len == 4){
		score = score + 320;
	}
	for(k=3;k>=0;k--){
		if(index[k] != 0){
			for(i=index[k]-1;i>0;i--){
				for(j=0;j<12;j++){
					tetris[i+1][j] = tetris[i][j];
				}
			}
		}
	}
	if(pause_flag == 1){
		clearScreen();
		setColor(4);
		printf("Stage : %d\n",stage_num);
		printf("Score : %d\n",score);
		printf(" "); for(i=0;i<10;i++){printf("-");}printf(" \n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<2;i++){printf(" ");}printf("Pause");for(i=0;i<3;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<2;i++){printf(" ");}printf("Resume");for(i=0;i<2;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<0;i++){printf(" ");}printf(":SpaceBar");for(i=0;i<1;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<3;i++){printf(" ");}printf("Quit");for(i=0;i<3;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<1;i++){printf(" ");}printf(":press q");for(i=0;i<1;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
		printf(" "); for(i=0;i<10;i++){printf("-");}printf(" \n");
		setColor(8);
	}
	for(j=0;j<12;j++){
		if(tetris[1][j] == 'x'){
			clearScreen();
			setColor(1);
			printf("Stage : %d\n",stage_num);
			printf("Score : %d\n",score);
			printf(" "); for(i=0;i<10;i++){printf("-");}printf(" \n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<3;i++){printf(" ");}printf("Game");for(i=0;i<3;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<3;i++){printf(" ");}printf("Over");for(i=0;i<3;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<1;i++){printf(" ");}printf("Restart");for(i=0;i<2;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<2;i++){printf(" ");}printf(":Enter");for(i=0;i<2;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<3;i++){printf(" ");}printf("Quit");for(i=0;i<3;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<1;i++){printf(" ");}printf(":press q");for(i=0;i<1;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf("|"); for(i=0;i<10;i++){printf(" ");}printf("|\n");
			printf(" "); for(i=0;i<10;i++){printf("-");}printf(" \n");
			over_flag=1;
			setColor(8);
		}
	}
}
void *UI_Work(void *arg){
	while(1){
		usleep(5*MSEC);
		if(key_changed == KEY_CHANGED_QUIT){
			pthread_exit(0);
		}
		else if(key_changed == KEY_CHANGED_NONE)
			continue;
		else if(key_changed == KEY_CHANGED_OK){
			key_changed = KEY_CHANGED_NONE;
			printScreen();
		}
		else
			continue;
	}
}

void *Key_Work(void *arg){
	char key;
	int i,j;

	set_disp_mode(0);

	while(1){
		usleep(5*MSEC);
		key = getchar();
			if(key == 27){
				pause_flag = 1;
			}else if(pause_flag == 1){
				if( key == 'q' ){
					key_changed = KEY_CHANGED_QUIT;
					set_disp_mode(1);
					pthread_exit(NULL);
				}else if(key == ' '){
					pause_flag = 0;
				}
			}else if(over_flag == 1){
				if( key == 'q' ){
					key_changed = KEY_CHANGED_QUIT;
					set_disp_mode(1);
					pthread_exit(NULL);
				}else if(key == '\n'){
					key_changed = KEY_CHANGED_OK;
					restart_flag = 1;
					over_flag = 0;
				}
			}else if(key == 'w'){
				key_changed = KEY_CHANGED_OK;
				if(rotationNo==0){
					if(shapeNo == 0){
						if(!((tetris[posY+1][posX+2] == 'x' || tetris[posY+1][posX+2] == '|') || 
									(tetris[posY+1][posX+3] == 'x' || tetris[posY+1][posX+3] == '|') ||
									(tetris[posY+1][posX+4] == 'x' || tetris[posY+1][posX+4] == '|')) ){
							rotationNo++;
						} 									
					}else if(shapeNo == 1){
						if(!((tetris[posY+1][posX+2] == 'x' || tetris[posY+1][posX+2] == '|') || 
									(tetris[posY+3][posX+1] == 'x' || tetris[posY+3][posX+1] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 2){
						if(!((tetris[posY+1][posX+1] == 'x' || tetris[posY+1][posX+1] == '|') || 
									(tetris[posY+3][posX+1] == 'x' || tetris[posY+3][posX+1] == '|') ||
									(tetris[posY+3][posX+2] == 'x' || tetris[posY+3][posX+2] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo ==3){
						rotationNo++;
					}else if(shapeNo == 4){
						if(!((tetris[posY+1][posX+1] == 'x' || tetris[posY+1][posX+1] == '|') || 
									(tetris[posY+3][posX+1] == 'x' || tetris[posY+3][posX+1] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 5){
						if(!((tetris[posY+1][posX+1] == 'x' || tetris[posY+1][posX+1] == '|') || 
									(tetris[posY+3][posX+2] == 'x' || tetris[posY+3][posX+2] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 6){
						if(!((tetris[posY+2][posX+1] == 'x' || tetris[posY+2][posX+1] == '|') || 
									(tetris[posY+3][posX+1] == 'x' || tetris[posY+3][posX+1] == '|')) ){
							rotationNo++;
						}
					}


				}else if(rotationNo==1){
					if(shapeNo == 0){
						if(!((tetris[posY+2][posX+1] == 'x' || tetris[posY+2][posX+1] == '|') || 
									(tetris[posY+3][posX+1] == 'x' || tetris[posY+3][posX+1] == '|') ||
									(tetris[posY+4][posX+1] == 'x' || tetris[posY+4][posX+1] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 1){
						if(!((tetris[posY+1][posX+3] == 'x' || tetris[posY+1][posX+3] == '|') || 
									(tetris[posY+2][posX+3] == 'x' || tetris[posY+2][posX+3] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 2){
						if(!((tetris[posY+1][posX+2] == 'x' || tetris[posY+1][posX+2] == '|') || 
									(tetris[posY+1][posX+3] == 'x' || tetris[posY+1][posX+3] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo ==3){
						rotationNo++;
					}else if(shapeNo == 4){
						if(!((tetris[posY+1][posX+2] == 'x' || tetris[posY+1][posX+2] == '|') || 
									(tetris[posY+1][posX+3] == 'x' || tetris[posY+1][posX+3] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 5){
						if(!((tetris[posY+1][posX+2] == 'x' || tetris[posY+1][posX+2] == '|') || 
									(tetris[posY+1][posX+3] == 'x' || tetris[posY+1][posX+3] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 6){
						if(!((tetris[posY+1][posX+1] == 'x' || tetris[posY+1][posX+1] == '|') || 
									(tetris[posY+2][posX+3] == 'x' || tetris[posY+2][posX+3] == '|')) ){
							rotationNo++;
						}
					}


				}else if(rotationNo==2){
					if(shapeNo == 0){
						if(!((tetris[posY+1][posX+2] == 'x' || tetris[posY+1][posX+2] == '|') || 
									(tetris[posY+1][posX+3] == 'x' || tetris[posY+1][posX+3] == '|') ||
									(tetris[posY+1][posX+4] == 'x' || tetris[posY+1][posX+4] == '|')) ){
							rotationNo++;
						} 									
					}else if(shapeNo == 1){
						if(!((tetris[posY+2][posX+2] == 'x' || tetris[posY+1][posX+2] == '|') || 
									(tetris[posY+3][posX+1] == 'x' || tetris[posY+3][posX+1] == '|') ||
									(tetris[posY+3][posX+2] == 'x' || tetris[posY+3][posX+2] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 2){
						if(!((tetris[posY+2][posX+2] == 'x' || tetris[posY+2][posX+2] == '|') || 
									(tetris[posY+3][posX+2] == 'x' || tetris[posY+3][posX+2] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo ==3){
						rotationNo++;
					}else if(shapeNo == 4){
						if(!((tetris[posY+2][posX+1] == 'x' || tetris[posY+2][posX+1] == '|') || 
									(tetris[posY+3][posX+2] == 'x' || tetris[posY+3][posX+2] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 5){
						if(!((tetris[posY+1][posX+1] == 'x' || tetris[posY+1][posX+1] == '|') || 
									(tetris[posY+3][posX+2] == 'x' || tetris[posY+3][posX+2] == '|')) ){
							rotationNo++;
						}
					}else if(shapeNo == 6){
						if(!((tetris[posY+2][posX+1] == 'x' || tetris[posY+2][posX+1] == '|') || 
									(tetris[posY+3][posX+1] == 'x' || tetris[posY+3][posX+1] == '|')) ){
							rotationNo++;
						}
					}


				}else if(rotationNo==3){
					if(shapeNo == 0){
						if(!((tetris[posY+2][posX+1] == 'x' || tetris[posY+2][posX+1] == '|') || 
									(tetris[posY+3][posX+1] == 'x' || tetris[posY+3][posX+1] == '|') ||
									(tetris[posY+4][posX+1] == 'x' || tetris[posY+4][posX+1] == '|')) ){
							rotationNo = 0;
						}
					}else if(shapeNo == 1){
						if(!((tetris[posY+1][posX+1] == 'x' || tetris[posY+1][posX+1] == '|') || 
									(tetris[posY+2][posX+1] == 'x' || tetris[posY+2][posX+1] == '|') ||
									(tetris[posY+2][posX+3] == 'x' || tetris[posY+2][posX+3] == '|')) ){
							rotationNo = 0;
						}
					}else if(shapeNo == 2){
						if(!((tetris[posY+1][posX+3] == 'x' || tetris[posY+1][posX+3] == '|') || 
									(tetris[posY+2][posX+1] == 'x' || tetris[posY+2][posX+1] == '|') ||
									(tetris[posY+2][posX+3] == 'x' || tetris[posY+2][posX+3] == '|')) ){
							rotationNo = 0;
						}
					}else if(shapeNo ==3){
						rotationNo = 0;
					}else if(shapeNo == 4){
						if(!((tetris[posY+2][posX+3] == 'x' || tetris[posY+2][posX+3] == '|')) ){
							rotationNo = 0;
						}
					}else if(shapeNo == 5){
						if(!((tetris[posY+1][posX+2] == 'x' || tetris[posY+1][posX+2] == '|') || 
									(tetris[posY+1][posX+3] == 'x' || tetris[posY+1][posX+3] == '|')) ){
							rotationNo = 0;
						}
					}else if(shapeNo == 6){
						if(!((tetris[posY+1][posX+1] == 'x' || tetris[posY+1][posX+1] == '|') || 
									(tetris[posY+2][posX+3] == 'x' || tetris[posY+2][posX+3] == '|')) ){
							rotationNo = 0;
						}
					}
				}
			}else if(key == 'a'){
				key_changed = KEY_CHANGED_OK;
				if(rotationNo==0){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(shapeNo == 0){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+4 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 1){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 2){
								if(i == posY+1 && j == posX+3){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 3){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 4){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 5){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 6){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}
						}

					}
				}else if(rotationNo==1){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(shapeNo == 0){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 1){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 2){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 3){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 4){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 5){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 6){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}
						}

					}
				}else if(rotationNo==2){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(shapeNo == 0){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+4 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 1){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+3){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 2){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 3){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 4){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 5){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 6){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}
						}

					}
				}else if(rotationNo==3){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(shapeNo == 0){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 1){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 2){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 3){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 4){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 5){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 6){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j-1] == 'x' || tetris[i][j-1] == '|'){
										end_flag = 1;
									}
								}
							}
						}
					}
				}
				if(end_flag != 1){
					posX--;
				}else{
					end_flag = 0;
				}
			}else if(key == 's'){
				key_changed = KEY_CHANGED_OK;

				if(shapeNo!=0){
					if(posY < 18){
						posY++;
					}
				}else{
					if(posY < 16){
						posY++;
					}
				}
			}else if(key == 'd'){
				key_changed = KEY_CHANGED_OK;
				if(rotationNo==0){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(shapeNo == 0){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+4 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 1){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 2){
								if(i == posY+1 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 3){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 4){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 5){
								if(i == posY+1 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 6){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}
						}

					}
				}else if(rotationNo==1){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(shapeNo == 0){
								if(i == posY+1 && j == posX+4){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 1){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 2){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 3){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 4){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 5){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 6){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}
						}

					}
				}if(rotationNo==2){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(shapeNo == 0){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+4 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 1){
								if(i == posY+1 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 2){
								if(i == posY+1 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 3){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 4){
								if(i == posY+1 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 5){
								if(i == posY+1 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 6){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+3){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}
						}

					}
				}else if(rotationNo==3){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(shapeNo == 0){
								if(i == posY+1 && j == posX+4){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 1){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 2){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 3){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 4){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 5){
								if(i == posY+1 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}else if(shapeNo == 6){
								if(i == posY+1 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+2 && j == posX+2){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}else if(i == posY+3 && j == posX+1){
									if(tetris[i][j+1] == 'x' || tetris[i][j+1] == '|'){
										end_flag = 1;
									}
								}
							}
						}
					}
				}
				if(end_flag != 1){
					posX++;
				}else{
					end_flag = 0;
				}

			}else if(key == 'f'){
				if(keep == -1){
					keep = shapeNo;
					shapeNo = rand()%7;
					posX=5;
					posY=-1;
				}else{
					shapeNo = keep;
					posX=5;
					posY=-1;
					keep = -1;
				}
			}else if(key == '\n'){
				key_changed = KEY_CHANGED_OK;
				if(shapeNo == 6){
					if(posY == 17 || posY == 18){
						posY = 16;
					}
					shapeNo=0;
				}else if(shapeNo==0){
					if(posX == 8 || posX == 9){
						posX = 7;
					}
					shapeNo++;
				}else if(shapeNo == 3){
					if(posX == 8){
						posX = 7;
					}
					shapeNo++;
				}else{
					shapeNo++;
				}
			}else if(key == ' '){
				key_changed = KEY_CHANGED_OK;
				if(rotationNo==0){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(i >= posY+1){
								if(shapeNo == 0){
									if(j == posX+1){
										if(tetris[i][j] == 'x' || tetris[i][j] == '-'){
											posY = i-5;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 1){
									if(j == posX+1 || j == posX+2 || j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 2){
									if(j == posX+1 || j == posX+2 || j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 3){
									if(j == posX+1 || j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 4){
									if(j == posX+1 || j == posX+2 || j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 5){
									if(j == posX+1 || j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}else if(j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 6){
									if(j == posX+2 || j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}else if(j == posX+1){
										if(tetris[i][j+1] == '-' || tetris[i][j+1] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}
								}
							}
						}
						if(end_flag == 1){
							end_flag = 0;
							break;
						}
					}
				}else if(rotationNo==1){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(i >= posY+1){
								if(shapeNo == 0){
									if(j == posX+1 || j == posX+2 || j == posX+3 || j == posX+4){
										if(tetris[i][j] == 'x' || tetris[i][j] == '-'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 1){
									if(j == posX+1){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i+1][j-1] == '-' || tetris[i+1][j-1] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}else if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 2){
									if(j == posX+1 || j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 3){
									if(j == posX+1 || j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 4){
									if(j == posX+1){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 5){
									if(j == posX+1){
										if(tetris[i][j+1] == '-' || tetris[i][j+1] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 6){
									if(j == posX+1){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}
							}
						}
						if(end_flag == 1){
							end_flag = 0;
							break;
						}
					}
				}else if(rotationNo==2){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(i >= posY+1){
								if(shapeNo == 0){
									if(j == posX+1){
										if(tetris[i][j] == 'x' || tetris[i][j] == '-'){
											posY = i-5;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 1){
									if(j == posX+1 || j == posX+2){
										if(tetris[i][j+2] == '-' || tetris[i][j+2] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}else if(j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 2){
									if(j == posX+1){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2 || j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 3){
									if(j == posX+1 || j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 4){
									if(j == posX+1 || j == posX+3){
										if(tetris[i][posX+2] == '-' || tetris[i][posX+2] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 5){
									if(j == posX+1 || j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}else if(j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 6){
									if(j == posX+2 || j == posX+3){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}else if(j == posX+1){
										if(tetris[i][j+1] == '-' || tetris[i][j+1] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}
								}
							}
						}
						if(end_flag == 1){
							end_flag = 0;
							break;
						}
					}
				}else if(rotationNo==3){
					for(i=1; i<22; i++){
						for(j=0; j<12; j++){
							if(i >= posY+1){
								if(shapeNo == 0){
									if(j == posX+1 || j == posX+2 || j == posX+3 || j == posX+4){
										if(tetris[i][j] == 'x' || tetris[i][j] == '-'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 1){
									if(j == posX+1){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 2){
									if(j == posX+1){
										if(tetris[i+1][j+1] == '-' || tetris[i+1][j+1] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}else if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-2;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 3){
									if(j == posX+1 || j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 4){
									if(j == posX+1){
										if(tetris[i][j+1] == '-' || tetris[i][j+1] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 5){
									if(j == posX+1){
										if(tetris[i][j+1] == '-' || tetris[i][j+1] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}
								}else if(shapeNo == 6){
									if(j == posX+1){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-4;
											end_flag = 1;
											break;
										}
									}else if(j == posX+2){
										if(tetris[i][j] == '-' || tetris[i][j] == 'x'){
											posY = i-3;
											end_flag = 1;
											break;
										}
									}
								}
							}
						}
						if(end_flag == 1){
							end_flag = 0;
							break;
						}
					}
				}

			}else{
				continue;
			}
//		}
	}
}
