#include<stdio.h>
#include<stdlib.h>
#include<shmem.h>

#define M 16
#define N 16
#define Q 16

int global_A[M][Q];
int global_B[Q][N];
int global_C[M][N];

/*generate data*/
void generate_data();

/* print the result */
void print_matrics();

/*elementary computation*/
void vector_multi_vector(i,j);

/* main function*/
int main(int argc, char** argv){
	int num_pes, mype;

	/*only one process, generate data*/
	generate_data();
	print_matrics();//verify
	
	/*get parallel*/
	start_pes(0);
	num_pes= _num_pes();
	mype = _my_pe();

	/*
	 * printf("I am rank = %d\n",mype);
	 * print_matrics();
	*/

	/*the input data are ALREADY symmetric! JUST DO COMPUTE then.*/
	/*for every PE*/
	int t;
	for(t = 0;t<= (M/num_pes); t++){
		int temp;
		temp = t * num_pes + mype;
		if(temp < Q){
			int j;
			for(j = 0;j<N;j++){
				vector_multi_vector(temp, j);
			}
						
		/*Now a row in C[temp][*]has been computed. put it into rank==0
		 *shmem_int_put(int *target, const int*source, size_t nelems, intpe);
		 */
		shmem_int_put(&(global_C[temp][0]), &(global_C[temp][0]), N, 0);
		}
	}

	shmem_barrier_all();
	if(mype == 0){
		print_matrics();
	}
	return 0;
}

void generate_data(){
	int i, j;
	for(i=0;i<M;i++){
		for(j=0;j<Q;j++){
			global_A[i][j] = i+j;
		}
	}

	for(i = 0;i<Q;i++){
		for(j = 0;j<N;j++){
			global_B[i][j] = i+j;
		}
	}

	for(i = 0;i<M;i++){
		for(j = 0;j<N;j++){
			global_C[i][j] = 0;
		}
	}
}


void print_matrics(){
	int i, j;

	printf("+++++++++++++++++++++++++\n\t\tThe input matrics are\n+++++++++++++++++++++++++");

	for(i = 0;i<M;i++){
		for(j = 0;j<Q;j++){
			if(j == 0) printf("\n");
			printf("%8d",global_A[i][j]);
		}
	}
	printf("\n\n");
	for(i = 0;i < Q;i++){
		for(j = 0;j<N;j++){
			if(j == 0) printf("\n");
			printf("%8d",global_B[i][j]);
		}
	}
	printf("\n\n");
	printf("+++++++++++++++++++++++\n\t\tThe result matrix is \n+++++++++++++++++++++++");
	for(i = 0;i < M;i++){
		for(j = 0; j < N;j++){
			if(j == 0) printf("\n");
			printf("%8d", global_C[i][j]);
		}
	}
	printf("\n\n");
}

void vector_multi_vector(i,j){
	/*Since all the data objects are symmetric
	 * all we should know is A[i][*] and B[*][j].
	 * Do the elementary computation.
	*/
	int k;
	for(k = 0;k<Q;k++)
	global_C[i][j] += global_A[i][k]*global_B[k][j];
}

