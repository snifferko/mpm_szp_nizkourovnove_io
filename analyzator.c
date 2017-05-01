#define CHYBA -1
#define ARG_COUNT 2
#define FILE_NAME_POS 1
#define READ_BYTE_LIMIT 1
#define END_OF_FILE 0
#define OK 0
#define FILE_DESC_MIN 1

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct node{
	struct node *next;
	float *seq;
	unsigned char len;
	float avg;
} seq_t;

seq_t *get_next(seq_t *s)
{
	return s->next;
}

void print_seq_t(seq_t *s)
{
	int i;

	if(s==NULL)
		return;
	else {
		printf("%d: ",s->len);
		
		for(i=0;i<s->len;i++)
			printf("%.2f ",s->seq[i]);
		
		printf("\n");
		print_seq_t(get_next(s));
	}
}


seq_t *create_seq_t(float *seq, unsigned char len)
{
	seq_t *s;

	s=malloc(sizeof(seq_t));
	s->seq=seq;
	s->len=len;
	s->next=NULL;
	return s;
}



void free_seq_t(seq_t *s)
{
	if(s==NULL)
		return;
	else {
		free_seq_t(get_next(s));
		free(s->seq);
		free(s);
	}
}


char read_seq_t(int file_desc, seq_t **s){
	char len;
	int stav;
	int read_limit;
	float *seq;

	stav = read(file_desc,&len,sizeof(char));
	
	if(stav < READ_BYTE_LIMIT)
		return END_OF_FILE;


	printf("len=%d\n",len);
	read_limit = len * sizeof(float);
	seq = malloc(read_limit);
	stav = read(file_desc,seq,read_limit);
	
	if(stav < read_limit)
		return CHYBA;

	*s=create_seq_t(seq,len);
	return OK;
}

void add_head(seq_t *new,seq_t **head)
{
	new->next=*head;
	*head=new;
}

int main(int argc, char **argv)
{
	int file;
	int n;
	seq_t *head;

	if(argc!=ARG_COUNT){
		printf("Nespravny pocet argumentov!\n");
		return CHYBA;
	} 
	
	file = open(argv[FILE_NAME_POS],O_RDONLY);
	
	if( file < FILE_DESC_MIN) {
		printf("Subor sa nepodarilo otvorit.\n");
		return CHYBA;
	} 

	int a = read_seq_t(file,&head);
	if( a == CHYBA){
		printf("CHYBA\n");
		return 0;
	}
	print_seq_t(head);
	free_seq_t(head);

	close(file);
}