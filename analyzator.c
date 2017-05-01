#define CHYBA -1
#define ARG_COUNT 2
#define FILE_NAME_POS 1

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

seq_t *create_seq_t(float *seq, unsigned char len)
{
	seq_t *s;

	s=malloc(sizeof(seq_t));
	s->seq=seq;
	s->len=len;
	s->next=NULL;
	return s;
}

seq_t *get_next(seq_t *s)
{
	return s->next;
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
	
	file = creat(argv[FILE_NAME_POS],S_IRWXU);
	
	if( file < FILE_DESC_MIN) {
		printf("Subor sa nepodarilo otvorit.\n");
		return CHYBA;
	} 

	head=NULL;

	free_seq_t(head);

	close(file);
}