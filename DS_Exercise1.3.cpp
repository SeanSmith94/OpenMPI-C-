//DS_Exercise 1.3
//Sean Smith: 2904982

#include <iostream>
#include <mpi.h>
#include <limits>
#include <array>
using namespace std;

int main(int argc, char **argv) {
  //Initialise MPI
  MPI_Init(NULL, NULL);
  
  //World Size and rank and num for user input
  int world_size, rank, num;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  //master
  if(rank == 0){
  	double n = 1; // for the denominator
  	double pi = 0; //for pi
  	bool inputCompletionFlag = true; //to check if user enter vaild input
	do{
		cout<<"\nEnter a number:";
	    cin >> num; //take in input
	    if(cin.fail() || num == 0 || num < 0){  //error checking
	      //Clear the fail status
	      cin.clear();
	      //Delete whats in the input
	      cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    }else{
	      inputCompletionFlag = false; //set flag to break loop if input is okay
	    }
	}while(inputCompletionFlag);
  
  	//Arrays for data 
  	double *scatter_data = new double[num];
  	double *recieve = new double[num / world_size];
  	double *totals = new double[num / world_size];
  
  	//put deniminators into the scatter array for the denominators for the Gregory-Leibniz series
  	for(unsigned int i = 0; i < num; i++){
    	scatter_data[i] = n;
    	n = n + 2;
  	}

	//Send the number intervals to all nodes
	for(int i = 0; i < world_size; i++) {
	   	if(i != 0) {
	       MPI_Send(&num, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
	    }
	}
  
  	//Scatter the data to all nodes
  	MPI_Scatter(scatter_data, num / world_size, MPI_DOUBLE, recieve, num / world_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  	for(unsigned int i = 0; i < num / world_size; i++){
  		if((i+1) % 2 == 0 && i != 0){ //Take into account that you must chance from a - to a +. All odds are + all evens are -
  			recieve[i] = -1 * (4 / recieve[i]);
  		}else{
  			recieve[i] = (4 / recieve[i]);
  		}
  	}
  	
  	//Take part in the reduce 
  	MPI_Reduce(recieve, totals, num/world_size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
  	//Add up all values in the reduce to get PI
  	for(unsigned int i = 0; i < num / world_size; i++)
  		pi = pi + totals[i];
  	
  	//Deallocate memory of arrays
  	delete scatter_data;
  	delete recieve;
  	std::cout << "PI is: " << pi << std::endl;
  }
  else{
  	//Get the number of intervals
  	MPI_Recv(&num, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	
  	//Allocate memory for recieving array
  	double *recieve = new double[num / world_size];

  	//Take part in the scatter to get the data from master
  	MPI_Scatter(NULL, 0, MPI_DOUBLE, recieve, num / world_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  	
  	//Take into account that you must chance from a - to a +. All odds are + all evens are -
  	for(unsigned int i = 0; i < num / world_size; i++){
  		if((i+1) % 2 == 0){
  			recieve[i] = -1 * (4 / recieve[i]);
  		}else{
  			recieve[i] = (4 / recieve[i]);
  		}
  	}

  	//Take part in the reduce so that data will be summed up and put into the totals array
  	MPI_Reduce(recieve, NULL, num/world_size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  	
  	//Deallocate recieved array memory
  	delete recieve;
  }
  
  //Finalise MPI
  MPI_Finalize();
  return 0;
}