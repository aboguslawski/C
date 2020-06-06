#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MDN 254
#define LIT 26
//zamienia wielkie litery na male
void maleLitery(char **A, int ilosc){
    for(int i = 0; i < ilosc; i++){
        for(int j = 0; j < MDN; j++){
            if(A[i][j] >= 65 && A[i][j] <= 90){
                A[i][j] += 32;
            }
        }
    }
}

void countingSort(char **A, char **B, int *C, int ilosc, int n){
    maleLitery(A, ilosc);
    for(int i = 0; i < LIT; i++){
        C[i] = 0;
    }
    for(int j = 0; j < ilosc; j++){
        if(strlen(A[j]) < n) C[0]++;
        else C[A[j][n-1] -96]++;
    }
    for(int i = 1; i <= LIT; i++){
        C[i] += C[i-1];
    }
    for(int j = ilosc - 1; j >= 0; j--){
        if(strlen(A[j]) < n) B[--C[0]] = A[j];
        else B[--C[A[j][n-1] -96]] = A[j];
    }
}

//drukowanie
void drukuj(char **A, int ilosc){
    printf("\n");
    for(int i = 0; i < ilosc; i++){
        printf("%s\n", A[i]);
    }
}

//czytanie pliku
void czytaj(char **A, int dlugosci[]){
    int i = 0;
    FILE *fp = NULL;
    fp = fopen("slowa.txt","r");
    char slowo[MDN+1];
    while(!feof(fp)){
        fscanf(fp, "%s", slowo);
        A[i] = (char*) malloc(MDN + 1);
        strcpy(A[i], slowo);
        dlugosci[i] = strlen(slowo);
        i++;
    }
}

//przechodzi przez plik i zwraca ilosc przejsc
int liczenie(){
    int i = 0;
    FILE *fp = NULL;
    fp = fopen("slowa.txt","r");
    char slowo[MDN+1];
    while(!feof(fp)){
        fscanf(fp, "%s", slowo);
        i++;
    }
    return i;
}

//szukanie najdluzszego stringa w tablicy
int maxD(int A[], int ilosc){
    int max = 0;
    for(int i = 0; i < ilosc; i++){
        if(A[i] > max) max = A[i];
    }
    return max;
}

int main(){
    int ilosc = liczenie();  //ilosc napisow w inpucie
    int dlugosci[ilosc];
    char **A, **B, **T;
    int *C;
    A = (char**) malloc((ilosc) * sizeof(char**));
    B = (char**) malloc((ilosc) * sizeof(char**));
    C = (int *) malloc((LIT + 1) * sizeof(int*));

    czytaj(A, dlugosci); //wczytywanie danych i zliczenie dlugosci

    int max = maxD(dlugosci,ilosc); //najdluzszy string w inpucie

    printf("\nA:\n");
    drukuj(A, ilosc);
    printf("\nB:\n");

    //Radix sort
    for(int i = max; i > 0; i--){
        countingSort(A,B,C,ilosc,i);
        printf("\ni - %d:",i);
        drukuj(B, ilosc);
        if(i > 1) T=A; A=B; B=T;
    }

    printf("\n\n\t\tkoniec\n");
}
