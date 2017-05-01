#define CHYBA -1
#define ARG_COUNT 3
#define MIN_N 1
#define N_POS 2
#define FILE_NAME_POS 1
#define FILE_DESC_MIN 0
#define MIN_DLZKA 10
#define MAX_DLZKA 20
#define MIN_VAL -100
#define MAX_VAL 100

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

typedef struct node{
	struct node *next;
	float *seq;
	unsigned char len;
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

float rand_float(float min, float max)
{
	float range;

	range=max-min;
	return drand48()*range+min;
}

seq_t *random_seq_t(unsigned int min_n, unsigned int max_n)
{
	unsigned int range;
	unsigned char len;
	float *seq;

	range=max_n-min_n+1;
	len=rand()%range+min_n;
	seq=malloc(len*sizeof(float));

	for(int i=0;i<len;i++)
		seq[i]=rand_float(MIN_VAL,MAX_VAL);

	return create_seq_t(seq,len);
}

void write_seq_t(int file_desc, seq_t *s)
{
	if(s==NULL)
		return;
	else {
		write(file_desc,&(s->len),sizeof(char));
		write(file_desc,s->seq,s->len*sizeof(float));
		write_seq_t(file_desc,get_next(s));
	}
}

void add_head(seq_t *new,seq_t **head)
{
	new->next=*head;
	*head=new;
}

void generator(seq_t **s,unsigned int n)
{
	seq_t *current;
	int i;

	for(i=0;i<n;i++) {
		current=random_seq_t(MIN_DLZKA,MAX_DLZKA);
		add_head(current,s);
	}
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

	n = atoi(argv[N_POS]); 

	if ( n < MIN_N ) {
		printf("Cislo n je neplatne!\n");
		return CHYBA;
	} 
	
	file = creat(argv[FILE_NAME_POS],S_IRWXU);
	
	if( file < FILE_DESC_MIN) {
		printf("Subor sa nepodarilo otvorit.\n");
		return CHYBA;
	} 

	head=NULL;
	generator(&head,n);
	print_seq_t(head);
	write_seq_t(file,head);
	free_seq_t(head);

	close(file);
}