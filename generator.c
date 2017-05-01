#define CHYBA -1
#define ARG_COUNT 3
#define MIN_N 1
#define N_POS 2
#define FILE_NAME_POS 1
#define FILE_DESC_MIN 0
#define MIN_DLZKA 10
#define MAX_DLZKA 20

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

typedef struct node{
	struct node *next;
	float *seq;
	unsigned int len;
} seq_t;

seq_t *create_seq_t(float *seq, unsigned int len)
{
	seq_t *s=malloc(sizeof(seq_t));
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

void add_head(seq_t *new,seq_t **head)
{
	new->next=*head;
	*head=new;
}

void print_seq_t(seq_t *s){
	int i;

	if(s==NULL)
		return;
	else {
		printf("%d: ",s->len);
		
		for(i=0;i<s->len;i++)
			printf("%.1f ",s->seq[i]);
		
		printf("\n");
		print_seq_t(get_next(s));
	}
}

seq_t *random_seq_t(unsigned int min_n, unsigned int max_n)
{
	unsigned int range;
	unsigned int len;
	float *seq;

	range=max_n-min_n+1;
	len=rand()%range+min_n;
	seq=malloc(len*sizeof(float));

	for(int i=0;i<len;i++)
		seq[i]=drand48()*FLT_MAX*2-FLT_MAX;

	return create_seq_t(seq,len);
}

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


	float A[]={3.5,1.8,-2.4,5.665,5};
	float *a;
	seq_t *s;
	seq_t *head;

	//TEST kontruktoru, printu, destruktoru
	
	a=malloc(sizeof(A));
	memcpy(a,A,sizeof(A));
	s=create_seq_t(a,5);
	print_seq_t(s);
	
	//TEST add_head

	head=s;
	a=malloc(sizeof(A));
	memcpy(a,A,sizeof(A));
	s=create_seq_t(a,5);
	add_head(s,&head);
	print_seq_t(head);

	//TEST random_seq_t
	s=random_seq_t(MIN_DLZKA,MAX_DLZKA);
	add_head(s,&head);
	s=random_seq_t(MIN_DLZKA,MAX_DLZKA);
	add_head(s,&head);
	print_seq_t(head);
	free_seq_t(s);

	close(file);
}