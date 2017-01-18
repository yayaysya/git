#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
	struct stat fstat;
	int err;
	char *dir = "./backup";
	err = stat(dir, &fstat);
	printf("err = %d\n ", err);
	if(err)
		printf("it is useful\n");
	return 0;
}
