#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NELEM(x)  (sizeof(x) / sizeof(x[0]))

int Partition(int A[], int p, int r){
	int x = A[r];
	int i = p - 1;
	
	for(int j = p; j <= r; j++){
		if(A[j] <= x){
			i++;
			int temp = A[i];
			A[i] = A[j];
			A[j] = temp;
		}
		
	}
      
	if(i < r) return i;
	else return i-1;
}

void Quicksort(int A[], int p, int r){
	if(p < r){
		int q = Partition(A, p, r);
		Quicksort(A, p, q);
		Quicksort(A, q+1, r);
	}	
}

void wczytajL(int A[], int x){
  int los;
  time_t tt;
  los = time(&tt);
  srand(los);

  for(int i = 0; i < x; i++){
    A[i] = rand() % 1000000;
  }
}

void wczytajN(int A[], int x){
  for(int i = 0; i < x; i++){
    A[i] = i;
  }
}

double pomiar(int A[], int x){
  clock_t start, end;
  start = clock();
  Quicksort(A, 0, x - 1);
  end = clock();
  return ((double) (end - start)) / CLOCKS_PER_SEC;
}

int main(){
  printf("\n\n");
  
  printf("rozmiar tablicy N \t | (sredni) czas - dane losowe \t | czas - dane niekorzystne \n");

  for(int z = 1000; z <= 100000; z+=1000){
    int tab[z];
    wczytajL(tab, z);
    double pomiarL1 = pomiar(tab, z);
    wczytajL(tab, z);
    double pomiarL2 = pomiar(tab, z);
    wczytajL(tab, z);
    double pomiarL3 = pomiar(tab, z);
    double pomiarL = (pomiarL1 + pomiarL2 + pomiarL3) / 3;
    wczytajN(tab, z);
    double pomiarN = pomiar(tab, z);

    printf("%d \t\t\t | %f \t\t\t | %f \n", z, pomiarL, pomiarN);
  }

  return 0;
  
}
