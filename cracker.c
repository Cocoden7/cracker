#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "src/reverse.h"
#include "src/sha256.h"
#include <inttypes.h>

typedef u_int8_t uint8_t;
// Signatures fonction
void *traduction(void *arg);
void *compare(void *arg);
int readFile(int file);
bool isTabEmpty(uint8_t **tab);

int NTHREAD = 1;
int CONS = 0;
char* FILEOUT = 0;
sem_t number_of_empty; // Nombre de places vides
sem_t number_of_full; // Nombre de places pleines
sem_t empty2;
sem_t full2;
sem_t gocomp;
sem_t finish;
sem_t tradgo;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

char *filename;

int FINISH = 0;
uint8_t *tab1;
char *tab2;
int r;

int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex1, NULL);
	for (int i = 1; i < argc && i < 5; ++i)
	{
		if (strcmp(argv[i],"-t")==0)
		{
			NTHREAD = atoi(argv[i+1]);
			i+=1;
		}
		if (strcmp(argv[i],"-c")==0)
		{
			CONS = 1;
		}
		if (strcmp(argv[i],"-o")==0)
		{
			FILEOUT = (char*) malloc(sizeof(argv[i+1]));
			strcpy(FILEOUT, argv[i+1]);
			i+=1;
		}
		else{
			filename = (char*) malloc(sizeof(argv[i]));
            strcpy(filename, argv[i]);
		}

	}
	tab1 = (uint8_t*)malloc(NTHREAD*sizeof(char));
	/*tab1[0] = (uint8_t*)malloc(sizeof(uint8_t)*32);*/
	tab2 = (char*)malloc(16*sizeof(char));
    sem_init(&number_of_empty, 0, NTHREAD-1);
    sem_init(&number_of_full, 0, 1);
    sem_init(&empty2, 0, 1);
    sem_init(&full2, 0, 0);
    sem_init(&gocomp, 0, 0);
    sem_init(&finish, 0, 0);
    sem_init(&tradgo, 0, 0);
    int file = open(filename, O_RDONLY);
    if (file == -1)
        {
            printf("Impossible d'ouvrir le fichier %s. \n", filename);
            return -1;
        }
    pthread_t *list_of_threads = (pthread_t*)malloc(NTHREAD * sizeof(pthread_t));
    for(int i = 0; i < NTHREAD; i++){
        pthread_create(&list_of_threads[i], NULL, traduction, 0);
    }
    pthread_t *thread_compare = (pthread_t*)malloc(sizeof(pthread_t));
    /*printf("avant pthread_create\n");*/
    pthread_create(thread_compare, NULL, compare, 0);
    printf("apres avoir cree le thread_compare\n");
    readFile(file);
    printf("apres readfile\n");
	sem_wait(&finish);
	return 0;
}
int COMPTEUR = 1;

// Prends un fd en argument, stock son contenu dans tab1.
int readFile(int file){
	/*printf("debut readfile\n");*/
    r = read(file, tab1, 32);
    if(r == -1){
        printf("error\n");
        exit(1);
    }
    sem_post(&tradgo);
    /*printf("apres read\n");*/
	while(r != 0){
		sem_wait(&number_of_empty);
		printf("rentre dans la boucle readfile\n");
        // pthread_mutex_lock(&mutex1);
        r = read(file, tab1, 32);
        if(r == -1){
            printf("Erreur lors de la lecture.");
            exit(1);
        }
        COMPTEUR++;
        if(COMPTEUR >= NTHREAD){
            COMPTEUR = 0;
        }
        // pthread_mutex_unlock(&mutex1);
        sem_post(&number_of_full);
        printf("fin de readfile\n");
	}
    FINISH = 1;

	close(file);
	return 0;
}

int COMPTEUR2 = 0;
void *traduction(void *arg){
	sem_wait(&tradgo);
	printf("debut traduction\n");
	while(r != 0){
        sem_wait(&number_of_full); // Verifie que les threads ne lisent pas un buffer vide
        pthread_mutex_lock(&mutex2); // Verifie que 2 threads ne le font pas en meme temps
		printf("rentre dans le while traduction\n");
        COMPTEUR2++;
        if(COMPTEUR2 >= NTHREAD){
            COMPTEUR2 = 0;
        }
        pthread_mutex_unlock(&mutex2);
        
        sem_wait(&empty2);
        printf("avant reversehash\n");
        reversehash(tab1, tab2, 16);
        *tab1='\0';
        printf("%s\n",tab2);
        sem_post(&full2);
        sem_post(&number_of_empty);
        printf("fin traduction\n");
	}
	return(EXIT_SUCCESS);
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

void *compare(void *arg){
	printf("debut compare\n");
	struct list l = {NULL,0};
	struct list *liste = malloc(sizeof(l));
	*liste = l;
	int first=1;
	while(!FINISH){
		sem_wait(&full2);
		printf("boucle compare\n");
		if (first)
		{
			printf("ok1\n");
			add_node(liste,tab2);
			first=0;
		}
		else if (countl(CONS, tab2)>countl(CONS, liste->first->mdp))
		{
			printf("ok2\n");
			free(liste);
			liste = malloc(sizeof(struct list));
			*liste = (struct list) {NULL,0};
			int j = add_node(liste,tab2);
			if (j==1)
			{
				printf("erreur add_node\n");
			}
		}
		else if (countl(CONS, tab2)==countl(CONS, liste->first->mdp))
		{
			printf("ok3\n");
			add_node(liste,tab2);
		}
		sem_post(&empty2);
		printf("fin boucle compare\n");
	}
	printf("apres le while compare\n");
	struct node *pointeur = malloc(sizeof(struct node));
	pointeur = liste->first;
	/*int file_out = open("file_out.txt",O_WRONLY|O_CREAT|O_TRUNC|O_APPEND);*/
	for (int i = 0; i<liste->size; ++i)
	{
		printf("%s\n",pointeur->mdp);
		/*write(file_out,pointeur->mdp,sizeof(pointeur->mdp));*/
		pointeur=pointeur->next;
	}
	/*close(file_out);*/
	free(liste);
	sem_post(&finish);
	return EXIT_SUCCESS;
}

bool isTabEmpty(uint8_t** tab){
	for (int i = 0; i < NTHREAD; ++i)
	{
		if (*tab[i] !='\0')
		{
			return false;
		}
	}
	return true;
}