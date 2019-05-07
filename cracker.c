#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>


int NTHREAD = 1;
int CONS = 0;
int FILEOUT = 0;
sem_t empty; // Nombre de places vides
sem_t full; // Nombre de places pleines
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

pthread_mutex_init(&mutex1, NULL);
pthread_mutex_init(&mutex2, NULL);
sem_init(&empty, 0, NTHREAD);
sem_init(&full, 0, 0);
const char *filename = 'c';


int main(int argc, char const *argv[])
{
	for (int i = 0; i < argc || i < 5; ++i)
	{
		if (strcmp(argv[i],"-t"))
		{
			NTHREAD = atoi(argv[i+1]);
			i++;
		}
		if (strcmp(argv[i],"-c"))
		{
			CONS = 1;
		}
		if (strcmp(argv[i],"-o"))
		{
			FILEOUT = 1;
			i++;
		}
		else{
            const char *filename = argv[i];
		}
	}
	#define NTHREAD NTHREAD
	uint8_t *TAB1[NTHREAD];

    int file = open(filename, O_RDONLY);
    if (file == -1)
        {
            printf("Impossible d'ouvrir le fichier %s. \n", filename);
            return -1;
        }
    readFile(file);

	return 0;
}

int readFile(int file){
    int i = 0;
	while(1){
        sem_wait(&empty);
        pthread_mutex_lock(&mutex1);
        if(read(file, TAB1[i], 32) == -1){
            printf("Erreur lors de la lecture.");
        }
        i++;
        if(i >= NTHREAD){
            i = 0;
        }
        pthread_mutex_unlock(&mutex1);
        sem_post(&full);
	}

	close(file);

}

void traduction(){
	/* à modifier */
	while(true){
	    int i = 0;
        sem_wait(&full); // Verifie que les threads ne lisent pas un buffer vide
        pthread_mutex_lock(&mutex2); // Verifie que 2 threads ne le font pas en meme temps
        reversehash(TAB1[i], TAB2[i], 32);
        pthread_mutex_unlock(&mutex2);
        sem_post(&empty);
	}
}

typedef struct node
{
	struct node *next;
	char* mdp;
}node_t;

typedef struct list {
  struct node *first;
  int size;
} list_t;

char **tab2;
int sizetab;
char voyelles[6] = {'a','e','i','o','u','y'};

int isvoyelle(char c){
	if (c == voyelles[0] || c == voyelles[1] || c == voyelles[2] || c == voyelles[3] || c == voyelles[4] || c == voyelles[5])
	{
		return 1;
	}
	else{
		return 0;
	}
}

int isconsonne(char c){
	if (c != voyelles[0] && c != voyelles[1] && c != voyelles[2] && c != voyelles[3] && c != voyelles[4] && c != voyelles[5])
	{
		return 1;
	}
	else{
		return 0;
	}
}

int countl(int cons, char *str){
	int count = 0;
	if (cons==1){
		for (int i = 0; i < 6; ++i)
		{
			if (isconsonne(str[i])==1)
			{
				++count;
			}
		}
	}
	else{
		for (int i = 0; i < 6; ++i)
		{
			if (isvoyelle(str[i])==1)
			{
				++count;
			}
		}
	}
	return count;
}

node_t *init_node(char *value) {
	char* val = malloc(sizeof(value));
	strcpy(val,value);
	node_t n = {NULL,val};
	node_t* nod = malloc(sizeof(n));
	if(nod==NULL){
	    return NULL;
	}
	*nod=n;
	return nod;
}

int add_node(list_t *list, char *value){
	if(list==NULL){
    	return 1;
	}
	node_t* n = init_node(value);
	if(n==NULL){
	    return 1;
	}
	n->next=list->first;
	list->first=n;
	list->size++;
	return 0;
}

void compare(int cons){
	struct list l = {NULL,0};
	struct list *liste = malloc(sizeof(l));
	*liste = l;
	add_node(liste,tab2[0]);
	for (int i = 1; i < sizetab; ++i)
	{
		 if (countl(cons, tab2[i])>countl(cons, liste->first->mdp))
		 {
		 	free(liste);
		 	liste = malloc(sizeof(struct list));
			*liste = (struct list) {NULL,0};
		 	int j = add_node(liste,tab2[i]);
		 	if (j==1)
		 	{
		 		printf("erreur add_node\n");
		 	}
		 }
		 else if (countl(cons, tab2[i])==countl(cons, liste->first->mdp))
		 {
		 	add_node(liste,tab2[i]);
		 }
	}
	struct node *pointeur = malloc(sizeof(struct node));
	pointeur = liste->first;
	int file_out = open("file_out.txt",O_WRONLY|O_CREAT|O_TRUNC|O_APPEND);
	for (int i = 0; i<liste->size; ++i)
	{
		printf("%s\n",pointeur->mdp);
		write(file_out,pointeur->mdp,sizeof(pointeur->mdp));
		pointeur=pointeur->next;
	}
	close(file_out);
}