//DS_Exercise 1.2b
//Sean Smith: 2904982
#include <iostream>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv) {
  // initialise MPI 
  MPI_Init(NULL, NULL);
  
  //World size and ranl
  int world_size, rank;
  int int_amount = 500;
  int total = 0; int number; int subtotal = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank == 0){
  	int *scatter_data = new int[(int_amount*(world_size-1))];
  	int *recieve = new int[int_amount];

  	MPI_Scatter(scatter_data, int_amount / world_size, MPI_INT, recieve, int_amount / world_size, MPI_INT, 0, MPI_COMM_WORLD);

  	delete scatter_data;
  	delete recieve;

    for(int i = 0; i < world_size - 1; i++){
      MPI_Recv(&total, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      subtotal = subtotal + total;
    }

    std::cout << "Rank "<< rank << " I am the Master" << " - Subtotal of all processes is: " << subtotal <<std::endl;
  }else{
  	// allocate memory for recieving 10 integers
  	int *recieve = new int[int_amount];

    // take part in the scatter to recieve our data and print it out to console
    MPI_Scatter(NULL, 0, MPI_INT, recieve, int_amount / world_size, MPI_INT, 0, MPI_COMM_WORLD);

    srand (time(NULL)*rank);
    for(int i = 0; i < int_amount; i++){
      recieve[i] = rand() % 1000 + 1; //random number in range of 0 - 1000
      if(rank % 2 == 0 && rank != 0)
        recieve[i] = recieve[i] * -1;
      total = total + recieve[i];
    }

    if(rank % 2 == 0 && rank != 0)
      std::cout << "Even Rank "<< rank << " - Total Sum:" << total << " - Average:" << (total / int_amount) <<std::endl;
    else
      std::cout << "Odd Rank: "<< rank << " - Total Sum:" << total << " - Average:" << (total / int_amount) <<std::endl;
    // deallocate recieved integers memory
    delete recieve;

    MPI_Send(&total, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();  //Finalise MPI
  return 0;
}