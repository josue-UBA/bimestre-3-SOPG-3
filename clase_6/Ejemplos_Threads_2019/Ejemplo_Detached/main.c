#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void* start_thread (void* message)
{
	printf ("%s\n", (const char *) message);
	sleep(1);
	return NULL;
}


int main (void)
{
	pthread_t thing1, thing2;
	int err;
	const char *message1 = "Thing 1";
	const char *message2 = "Thing 2";

	pthread_create (&thing1, NULL, start_thread, (void *) message1);
	pthread_create (&thing2, NULL, start_thread, (void *) message2);
	
	pthread_detach(thing1);

	err = pthread_join (thing1, NULL);
	printf("Resultado join t1:%d\n",err);
	if(err==EINVAL)
		printf("Thread no joineable\n");

	err = pthread_join (thing2, NULL);
	printf("Resultado join t2:%d\n",err);
	return 0;
}


