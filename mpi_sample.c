#include <stdio.h>
#include <string.h>
#include "unistd.h"
#include "mpi.h"

int main(int argc, char* argv[]){
	int my_rank;
	int p;
	int source;
	int dest = 0;
	int tag = 0;

	int message_size = 100;
	char message[message_size];
	
	int host_size = 256;
	char hostname[host_size];

	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	MPI_Comm_size(MPI_COMM_WORLD, &p);

	gethostname(hostname,host_size-1);

	sprintf(message, "Hello from process [%d] on [%s]", my_rank, hostname);

	if (my_rank != 0) MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	else {
		printf("%s\n", message);
		for (source = 1; source < p; source++){
			MPI_Recv(message, message_size, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
			printf("%s\n",message);
		}
	}

	MPI_Finalize();

	return 0;
}
