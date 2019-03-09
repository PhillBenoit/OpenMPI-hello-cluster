#include <stdio.h>
#include <string.h>
#include "unistd.h"
#include "mpi.h"

//watch this run at:
//https://youtu.be/WPt4W0JNVDA

//build guide for the machine I used to test this code:
//https://forum.pine64.org/showthread.php?tid=7077

//entry point
int main(int argc, char* argv[]){
	
	//used to hold process ID
	int my_rank;
	
	//number of processes
	int p;
	
	//used to determine source and destination processes
	int source;
	int dest = 0;
	int tag = 0;

	//"hello world" message to pass to other processes
	int message_size = 256;
	char message[message_size];
	
	//holds the host name
	int host_size = 100;
	char hostname[host_size];

	//get MPI process information 
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	//get hostname
	gethostname(hostname,host_size-1);

	//format output message
	sprintf(message, "Hello from process [%d] on [%s]", my_rank, hostname);

	//send all non 0 ID processes
	if (my_rank != 0) MPI_Send(message, strlen(message)+1, MPI_CHAR,
				   dest, tag, MPI_COMM_WORLD);
	
	//output from 0 ID process
	else {
		//display output from 0
		printf("%s\n", message);
		
		//display output from other processes
		for (source = 1; source < p; source++){
			
			//receive data from a specific process
			MPI_Recv(message, message_size, MPI_CHAR, source,
				 tag, MPI_COMM_WORLD, &status);
			
			//display received data
			printf("%s\n",message);
		}
	}

	//terminate MPI calls
	MPI_Finalize();

	//end program
	return 0;
}
