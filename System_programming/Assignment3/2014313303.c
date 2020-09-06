#ifdef RUNTIME

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int call;


int system(const char *string){

	return 0;

}

int unlink( const char *path){

	return 0;

} 

char *strtok(char *s1, const char *s2){

	return "Tigger";


}

unsigned int sleep( unsigned int seconds){

	return 0;

}

size_t fwrite( const void *ptr, size_t size, size_t nitems, FILE *stream)
{
	return 13;
}

char * fgets ( char * str, int num, FILE * stream )
{
	if(call==0)
	{
		call++;
		return str;
	}
	else
	{
		strcpy(str, "Pikachoo	zwi\n");
		return 0;
	}
}
#endif
