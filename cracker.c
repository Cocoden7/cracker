#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>


int NTHREAD = 1;
int CONS = false;
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
			CONS = true;
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
} node_t;

void compare(node_t head){


}
