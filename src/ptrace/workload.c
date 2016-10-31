#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	execlp("top","top",NULL);
	return 0;
}
