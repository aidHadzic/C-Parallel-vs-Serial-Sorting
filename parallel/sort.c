#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

const int n = 300;

//generate random numbers
void give_me_random(float* list){

	int i;
	for(i = 0;i < n;i++)
		list[i] = ((float)rand()/(float)(RAND_MAX)) * 1000; //from 0 to 1000

}

//print list
void print(float* list){

	int i;
	for (i = 0;i < n;i++)
		printf("%f ",list[i]);
	printf("\n");
	
}

//function needed as c qsort parameter
int compare(const void* a,const void* b){

	return (*(int*)a - *(int*)b);

}

//index of max element in list
int find_max(float* list){

	int i, index = 0;
	float max = list[0];

  	for (i = 1; i < n; i++) {
    		if (list[i] > max) {
      			max = list[i];
      			index = i;
    		}
  	}

	return index;

}

//index of min element in list
int find_min(float* list) {

	int i, index = 0; 
	float min = list[0];

	for (i = 1; i < n; i++) {
    		if (list[i] < min) {
      			min = list[i];
      			index = i;
    		}
  	}

  	return index;
	
}

//sequential bubble sort
void bubble(float* list, int sze){
	
	int i,j,t;
	clock_t start = clock();

	for (i = 0;i < (n-1); i++){
    		for (j = 0 ; j < n-i-1; j++){
      			if (list[j] > list[j+1]){
        			t = list[j];
        			list[j] = list[j+1];
        			list[j+1] = t;
      			}
    		}
  	}

	clock_t end = clock();

	float elapsed = (float)(end - start) / CLOCKS_PER_SEC * 1000;
	printf("\nIt needed %f miliseconds \n", elapsed);
 
  	printf("Sequential sorted list:\n");
 
  	for (i = 0;i < n; i++ )
     		printf("%f ", list[i]);

	printf("\n");


}

//parallel bubble sort - odd even transposition sort
void sort(float* list, int rnk, int sze){
	
	clock_t begining = clock();

	int i;
	float second_list[n];
	for (i = 0;i < sze;i++){
	
		qsort(list, n, sizeof(float), &compare);
		int partner;
		
		//odd phase
		if(i%2 != 0){
			//even process
			if (rnk % 2 == 0) {
        			partner = rnk - 1;
      			} else {
        			partner = rnk + 1;
      			}
		} 
		//even phase
		else {
			//even process
			if (rnk % 2 == 0) {
        			partner = rnk + 1;
      			} else {
        			partner = rnk - 1;
      			}
		}

		if (partner >= sze || partner < 0 ) {
      			continue;
    		}
		
		//even process sends first, odd recieves first
	 	if (rnk % 2 == 0) {
      			MPI_Send(list, n, MPI_INT, partner, 0, MPI_COMM_WORLD);
      			MPI_Recv(second_list, n, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    		} else {
      			MPI_Recv(second_list, n, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      			MPI_Send(list, n, MPI_INT, partner, 0, MPI_COMM_WORLD);
    		}
		
		//smaller elements in smaller process
		if(rnk < partner){
		
			while(1){

				int min_index = find_min(second_list);
				int max_index = find_max(list);
				float t;

				if(second_list[min_index] < list[max_index]){
					t = second_list[min_index];
					second_list[min_index] = list[max_index];
					list[max_index] = t;
				}				
				else break;

			}
		//larger elements in larger process
		} else {

			while(1){

				int min_index = find_min(list);
				int max_index = find_max(second_list);
				float t;
			
				if (second_list[max_index] > list[min_index]) {
          				t = second_list[max_index];
          				second_list[max_index] = list[min_index];
          				list[min_index] = t;
        			} else break;
        		
      			}

		}
		
	}

	clock_t end = clock();
	float elapsed = (float)(end - begining) / CLOCKS_PER_SEC * 1000;
	printf("It needed %f miliseconds \n", elapsed);

} 

int main(int argc, char* argv[]){

	int rnk, sze = 300;
	float list[n];
	 
  	MPI_Init(&argc, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rnk);
  	MPI_Comm_size(MPI_COMM_WORLD, &sze);
	give_me_random(list);
	printf("List before sorting: \n");
	print(list);
	printf("\n\n");
	sort(list,rnk,sze);
	printf("List after parallel sorting: \n");
	print(list);
	printf("\n\n");
	bubble(list,sze);

	MPI_Finalize( );
	
	return 0;

}

