#include <stdlib.h>
#include <stdio.h>
#include <reverse.h>
#include <sha256.h>
#include <semaphore.h>
#include <pthread.h>


int NTHREAD = 1;
bool CONS = false;
bool FILEOUT = false;
uint8_t *TAB1[NTHREAD];
pthread_t mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[])
{
	for (int i = 0; i < argc || i < 5; ++i)
	{
		if (strcmp(argv[i],"-t"))
		{
			NTHREAD = atoi(argv[i+1]);
		}
		if (strcmp(argv[i],"-c"))
		{
			CONS = true;
		}
		if (strcmp(argv[i],"-o"))
		{
			FILEOUT = true;
		}
	}



	return 0;
}

public int readFile(const char *filename){

	int file = open(filename, O_RDONLY);
	if (file == -1)
	{
		printf("Impossible d'ouvrir le fichier %s. \n", filename);
		return -1;
	}
	int i = 0
	while(true){
        pthread_mutex_lock(&mutex1);
        int cursor = read(file, TAB1[i], 32);
        if(cursor == -1){
            printf("Erreur lors de la lecture.");
        }
        pthread_mutex_unlock(&mutex1);
        i++;
        if(i == NTHREAD){
            i = 0;
        }
	}

	close(file);

}

public void traduction(){
	/* à modifier */
	char *tab2[sizetab];
	for (int i = 0; i < sizetab; ++i)
	{
		reversehash(tab1[i],tab2[i],32);
	}
}

typedef struct node
{
	struct node *next;
	char* mdp;
} node_t;

public void compare(node_t head){


}
