#define CHYBA -1
#define ARG_COUNT 2
#define FILE_NAME_POS 1
#define READ_BYTE_LIMIT 1
#define END_OF_FILE 0
#define OK 1
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
//		printf("%d: ",s->len);
		
		for(i=0;i<s->len;i++)
			printf("%f ",s->seq[i]);
		
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

void add_head(seq_t *new,seq_t **head)
{
	new->next=*head;
	*head=new;
}

char read_seq_t(int file_desc, seq_t **s){
	char len;
	int stav;
	int read_limit;
	float *seq;

	stav = read(file_desc,&len,sizeof(char));
	
	if(stav < READ_BYTE_LIMIT)
		return END_OF_FILE;

	read_limit = len * sizeof(float);
	seq = malloc(read_limit);
	stav = read(file_desc,seq,read_limit);
	
	if(stav < read_limit)
		return CHYBA;

	*s=create_seq_t(seq,len);
	return OK;
}

char read_seqs_t(int file_desc,seq_t **head){
	int stav;
	int n;
	seq_t **current;

	current=head;
	stav = read_seq_t(file_desc,current);
	n=0;
	
	while(stav == OK) { 
		current=&((*current)->next);
		stav = read_seq_t(file_desc,current);
		n++;	
	}

	return stav == CHYBA ? CHYBA : n;
}

void avg_seq_t(seq_t* s)
{
	int i;
	float sum;

	sum=0;

	for(i=0; i<s->len; i++)
		sum+=s->seq[i];
	
	s->avg=sum/s->len;
}

void avg_seqs_t(seq_t* s)
{
	if(s == NULL)
		return;
	else {
		avg_seq_t(s);
		avg_seqs_t(get_next(s));
	}
}

float analyzator(seq_t *s)
{
	int count;
	float sum; 

	sum=0;
	count=0;
	
	while(s != NULL){
		count++;
		sum+=s->avg;
		s = get_next(s);
	}

	return sum/count;
}


int main(int argc, char **argv)
{
	int file;
	int n,a;
	float avg_avgs;
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

	a = read_seqs_t(file,&head);
	
	if( a == CHYBA){
		printf("Nastala chyba pri citani suboru.\n");
		return CHYBA;
	}

	avg_seqs_t(head);
	avg_avgs=analyzator(head);

	printf("%d\n",a);
	printf("%f\n",avg_avgs);
	print_seq_t(head);

	free_seq_t(head);

	close(file);
	return 0;
}