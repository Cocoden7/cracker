#include <stdlib.h>
#include <stdio.h>

int NTHREAD = 1;
bool CONS = false;
bool FILEOUT = false;


int main(int argc, char const *argv[])
{
	for (int i = 0; i < argc || i < 5; ++i)
	{
		if (strcmp(argv[i],"-t"))
		{
			NTHREAD = atoi(argv[i+1])
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

public void readfile(const char *filename){

	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		printf("Impossible d'ouvrir le fichier %s. \n", filename);
	}
	struct stat *buf = malloc(sizeof(struct stat));
	fstat(file, buf);
	off_t size = buf->st_size;
	int sizetab = (int) size/32;
	char *tab1[sizetab];
	for (int i = 0; i < sizetab; ++i)
	{
		fgets(tab1[i],32,file);
	}

}