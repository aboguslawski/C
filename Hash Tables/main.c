#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct komorka{
    int liczba;
    char* nazwisko;
} kom;

int lines(FILE *fp){
    int i = 0;
    char x = fgetc(fp);
    while(x != EOF){
        if(x == '\n'){
            i++;
        }
        x = fgetc(fp);
    }

    return i;
}

int main()
{
    FILE* fp = fopen("nazwiska.txt", "r");
    int size = lines(fp);
    fp = fopen("nazwiska.txt", "r");

    char oneLine[99];
    char file[size][99];
    kom data[size];

    printf("size: %d\n\n", size);

    for(int i = 0; i < size; i++){
        fgets(oneLine, sizeof(oneLine), fp);
        strcpy(file[i], oneLine);
    }

    for(int i = 0; i < size; i++){
        printf("file %d. %s", i, file[i]);
    }

    char numbers[size][99];
    char names[size][99];
    int flaga = 0;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < 10; j++){
            printf("%c", file[i][j]);

        }
    }

















    fclose(fp);

    return 0;
}
