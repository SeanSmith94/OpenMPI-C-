#include <iostream>
#include <mpi.h>
#include <string>

int main(int argc, char **argv) {
  //Initialise MPI
  MPI_Init(NULL, NULL);
  //World size and rank
  int world_size, rank, char_amount;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int numberOfMessages = (world_size-1)*2;

  //0 = Master 0+ = slave
  if(rank == 0){
    for(int i = 0; i != numberOfMessages; ++i){
      MPI_Status status;
      
      MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_CHAR, &char_amount);
      char myStr[char_amount];
      MPI_Recv(myStr, char_amount, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      std::cout << myStr <<std::endl;
    } 
  }else{
    std::string s1 = "Hello from Slave: ";
    s1 += std::to_string(rank);
    MPI_Send(s1.c_str(), s1.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    std::string s2 = "Goodbye from Slave: ";
    s2 += std::to_string(rank);
    MPI_Send(s2.c_str(), s2.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }
  
  //Finialise MPI
  MPI_Finalize();
  return 0;
}
