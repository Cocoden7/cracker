
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