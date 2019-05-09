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
bool isTab1Empty(uint8_t **tab);
bool isTab2Empty(char **tab);

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

char **filename;

int FINISH = 0;
uint8_t **tab1;
char **tab2;
int r=1;
int COMPTEUR3 = 0;
int count = 0;
char *flag;

int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex1, NULL);
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i],"-t")==0)
		{
			printf("-t\n");
			NTHREAD = atoi(argv[i+1]);
			i+=1;
		}
		else if (strcmp(argv[i],"-c")==0)
		{
			printf("-c\n");
			CONS = 1;
		}
		else if (strcmp(argv[i],"-o")==0)
		{
			printf("-o\n");
			FILEOUT = (char*) malloc(sizeof(argv[i+1]));
			strcpy(FILEOUT, argv[i+1]);
			i+=1;
		}
		else{
			printf("file\n");
			filename = (char**) malloc(argc-i);
			for (int j = 0; j <= argc-i; ++j)
			{
				filename[j] = (char*) malloc(sizeof(argv[i]));
            	strcpy(filename[j], argv[i]);
            	printf("%s\n", filename[j]);
            	++i;
			}
			
		}

	}
	tab1 = (uint8_t**)malloc(NTHREAD*sizeof(char*));
	for (int i = 0; i < NTHREAD; ++i)
	{
		tab1[i] = (uint8_t*)malloc(32);
	}
	tab2 = (char**)malloc(NTHREAD*sizeof(char*));
	for(int i = 0; i < NTHREAD; ++i)
	{
		tab2[i] = (char*)malloc(16);
	}
	flag = (char*)malloc(NTHREAD);
	for (int i = 0; i < NTHREAD; ++i)
	{
		flag[i]='0';
	}
	for (int i = 0; i < NTHREAD; ++i)
	{
		flag[i]='0';
	}
    sem_init(&number_of_empty, 0, NTHREAD);
    sem_init(&number_of_full, 0, 0);
    sem_init(&empty2, 0, NTHREAD);
    sem_init(&full2, 0, 0);
    sem_init(&gocomp, 0, 0);
    sem_init(&finish, 0, 0);
    sem_init(&tradgo, 0, 0);
    
    pthread_t **list_of_threads = (pthread_t**)malloc(NTHREAD * sizeof(pthread_t*));
    for(int i = 0; i < NTHREAD; i++){
    	list_of_threads[i] = (pthread_t*) malloc(sizeof(pthread_t));
        pthread_create(list_of_threads[i], NULL, traduction, 0);
        printf("%d\n", i);
    }
    pthread_t *thread_compare = (pthread_t*)malloc(sizeof(pthread_t));
    /*printf("avant pthread_create\n");*/
    pthread_create(thread_compare, NULL, compare, 0);
    printf("apres avoir cree le thread_compare\n");
    int file;
    for (int i = 0; filename[i] != NULL; ++i)
    {
    	file = open(filename[i], O_RDONLY);
    	if (file == -1)
        {
            printf("Impossible d'ouvrir le fichier %s. \n", filename[i]);
            return -1;
        }
        readFile(file);
        close(file);
        free(filename[i]);
    }
    FINISH = 1;
    
    printf("apres readfile\n");
	sem_wait(&finish);
	for (int i = 0; i < NTHREAD; ++i)
	{
		free(tab1[i]);
		free(tab2[i]);
		pthread_exit(list_of_threads[i]);
		free(list_of_threads[i]);
	}
	free(tab1);
	free(tab2);
	free(list_of_threads);
	pthread_exit(thread_compare);
	free(thread_compare);
	return 0;
}
int COMPTEUR = 0;

// Prends un fd en argument, stock son contenu dans tab1.
int readFile(int file){
	/*printf("debut readfile\n");*/
	/*for (int i = 0; i < NTHREAD; ++i)
	{
		r = read(file, tab1[i], 32);
    	if(r == -1){
        	printf("error\n");
        	exit(1);
    	}
    	printf("read %d\n", i);
    	sem_post(&number_of_full);
    	//sem_post(&tradgo);
	}*/
    
    /*printf("apres read\n");*/
	while(r != 0){
		sem_wait(&number_of_empty);
		//printf("rentre dans la boucle readfile\n");
        // pthread_mutex_lock(&mutex1);
        bool trad = true;
        if (*tab1[COMPTEUR]=='\0')
        {
        	r = read(file, tab1[COMPTEUR], 32);
        	if(r == -1){
            	printf("Erreur lors de la lecture.");
            	exit(1);
        	}
        	count+=1;
        	printf("%d\n", count);
        	printf("écriture dans le tableau à la place %d du hash %u\n", COMPTEUR, *tab1[COMPTEUR]);
        }
        else{
        	trad = false;
        	sem_post(&number_of_empty);
        }
        
        COMPTEUR+=1;
        if(COMPTEUR >= NTHREAD){
            COMPTEUR = 0;
        }
        /*if (r == 0)
        {
        	FINISH = 1;
        	printf("FINISH\n");
        }*/
        // pthread_mutex_unlock(&mutex1);
        if (trad)
        {
        	sem_post(&number_of_full);
        }
        
        //printf("fin de readfile\n");
	}
	return(EXIT_SUCCESS);
}

void *traduction(void *arg){
	//sem_wait(&tradgo);
	//printf("debut traduction\n");
	int COMPTEUR2 = -1;
	while(!isTab1Empty(tab1) || !FINISH){
        sem_wait(&number_of_full); // Verifie que les threads ne lisent pas un buffer vide
        pthread_mutex_lock(&mutex2); // Verifie que 2 threads ne le font pas en meme temps
		//printf("rentre dans le while traduction\n");
		if (FINISH)
		{
			sem_post(&full2);
			return(EXIT_SUCCESS);
		}
		COMPTEUR2++;
        if(COMPTEUR2 >= NTHREAD){
            COMPTEUR2 = 0;
        }
        pthread_mutex_unlock(&mutex2);
        
        //printf("hash : %u, flag : %c, place : %d\n",*tab1[COMPTEUR2], flag[COMPTEUR2], COMPTEUR2);
        if (*tab1[COMPTEUR2] != '\0' && flag[COMPTEUR2] == '0')
        {
        	sem_wait(&empty2);
        	
        	flag[COMPTEUR2]='1';
        	reversehash(tab1[COMPTEUR2], tab2[COMPTEUR2], 16);
        	*tab1[COMPTEUR2]='\0';
        	flag[COMPTEUR2]='0';
        	printf("--------------------------%s à la place %d-----------------------------\n",tab2[COMPTEUR2], COMPTEUR2);
        	sem_post(&full2);
        	sem_post(&number_of_empty);
        }
        else{
        	sem_post(&number_of_full);
        }
        
        //printf("fin traduction\n");
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
		for (int i = 0; str[i]!='\0'; ++i)
		{
			if (isconsonne(str[i])==1)
			{
				++count;
			}
		}
	}
	else{
		for (int i = 0; str[i]!='\0' ; ++i)
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
	while(!isTab1Empty(tab1) || !FINISH){
		sem_wait(&full2);
		//printf("boucle compare\n");
		if (*tab2[COMPTEUR3]!='\0' && (first == 1 || strcmp(tab2[COMPTEUR3],liste->first->mdp)!=0))
		{
			//printf("comparaison de %s\n", tab2[COMPTEUR3]);
			if (first)
			{
				//printf("ok1\n");
				add_node(liste,tab2[COMPTEUR3]);
				first=0;
			}
			else if (countl(CONS, tab2[COMPTEUR3])>countl(CONS, liste->first->mdp))
			{
				printf("ok2\n");
				free(liste);
				liste = malloc(sizeof(struct list));
				*liste = (struct list) {NULL,0};
				int j = add_node(liste,tab2[COMPTEUR3]);
				if (j==1)
				{
					printf("erreur add_node\n");
				}

			}
			else if (countl(CONS, tab2[COMPTEUR3])==countl(CONS, liste->first->mdp))
			{
				//printf("ok3\n");
				add_node(liste,tab2[COMPTEUR3]);
			}
			//tab2[COMPTEUR3]='\0';
		}
		COMPTEUR3+=1;
		if (COMPTEUR3>=NTHREAD)
		{
			COMPTEUR3=0;
		}
		sem_post(&empty2);
		//printf("fin boucle compare\n");
	}
	//printf("apres le while compare\n");
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

bool isTab1Empty(uint8_t** tab){
	for (int i = 0; i < NTHREAD; ++i)
	{
		if (*tab[i] !='\0')
		{
			return false;
		}
	}
	//printf("empty\n");
	return true;
}

bool isTab2Empty(char** tab){
	for (int i = 0; i < NTHREAD; ++i)
	{
		if (*tab[i] !='\0')
		{
			return false;
		}
	}
	return true;
}