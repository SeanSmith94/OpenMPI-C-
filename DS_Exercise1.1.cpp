//DS_Exercise 1.1
//Sean Smith: 2904982
#include <iostream>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv) {
  // initialise MPI 
  MPI_Init(NULL, NULL);
  
  //World size and ranl
  int world_size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(rank % 2 == 0){ //If even
  	int sum = 0;
  	int random;
  	srand (time(NULL)*rank);

  	for(int i =0; i < 100; i++){
  		random = rand() % 10 + 1; //random number in range of 0 - 10
  		sum = sum + random;
  	}

  	std::cout << "Even Rank: "<< rank << " // World Size: " << world_size << " // Sum of Random Numbers: " << sum <<std::endl;

  }else{ //If odd
  	std::cout << "Odd Rank: "<< rank << " // World Size: " << world_size << " // Student Number: 2904982, Student Name: Seán Smith"<< std::endl;
  }
  
  //Finalise MPI
  MPI_Finalize();
  return 0;
}