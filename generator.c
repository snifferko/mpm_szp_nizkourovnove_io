#define CHYBA -1
#define ARG_COUNT 3
#define MIN_N 1
#define N_POS 2
#define FILE_NAME_POS 1
#define FILE_DESC_MIN 0

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv){
	int file;
	int n;

	if(argc!=ARG_COUNT){
		printf("Nespravny pocet argumentov!\n");
		return CHYBA;
	} 
	
	if ( n = atoi(argv[N_POS]) < MIN_N ) {
		printf("Cislo n je neplatne!\n");
		return CHYBA;
	} 
	
	file = creat(argv[FILE_NAME_POS],S_IRWXU);
	
	if( file < FILE_DESC_MIN) {
		printf("Subor sa nepodarilo otvorit.\n");
		return CHYBA;
	} 

	close(file);
}