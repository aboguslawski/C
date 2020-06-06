// Adam Boguslawski - najdluzszy wspolny podciag: wersja za 5(max) punktow

// wszystkie wykomentowane printf'y sluzyly do sprawdzania przebiegu funkcji w trakcie dzialania
// i wykrywania / naprawiania bledow
// stwierdzilem ze nie zaszkodzi je zostawic, moze Pan bedzie sam chcial na to zerknac
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// definiowanie dlugosci tablic "recznie" w makrach ulatwilo mi znacznie pisanie programu
// teoretycznie da sie to zautomatyzowac ale chyba nie o to chodzilo w tym zadaniu
// wiec nie tracilem na to czasu, mam nadzieje ze nie jest to problemem
#define m 6
#define n 8

// przekazywane wyrazy jako zmienne globalne
// potem w mainie sa wstawiane do tablic
char *word1 = "abbaac";
char *word2 = "bacbacba";

// funkcja zwracajaca dlugosc najdluzszego wspolnego podciagu
// punktem wyjscia byl pseudokod z wykladu
// jednak przez zastosowanie indeksowania od 1 musialem go nieco zmienic
// mozliwe ze takie przekazywanie dwuwymiarowych tablic do funkcji nie jest "najelegantsze"
// jednak z tego co czytalem to trzeba juz na wstepie podac chociaz drugi ich wymiar
// m+1 lub n+1 dlatego, bo n i m to wielkosci tablic z wyrazami
// a tablice b i c przechowuja dodatkowo ten "zerowy" wiersz i kolumne
int lcslen(char x[], char y[], char b[m+1][n+1], int c[m+1][n+1]){
    //printf("x[5] = %c, y[7] = %c, m = %d, n = %d\n", x[5], y[7], m, n);

    //zerowanie
    for(int i = 0; i < m; i++){ 
        c[i][0] = 0;
    }
    for(int j = 0; j < n; j++){
        c[0][j] = 0;
    }
    
    //wypelnianie tablic b i c, inspirowane pseudokodem z materialow dr. Paczkowskiego
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            if(x[i] == y[j]){
                c[i+1][j+1] = c[i][j] + 1;
                b[i+1][j+1] = '\\';
                //printf("x[%d] = %c, y[%d] = %c, c[%d][%d] = %d\n", i, x[i], j, y[j], i+1, j+1, c[i+1][j+1]);
            }
            else if(c[i][j+1] >= c[i+1][j]){
                c[i+1][j+1] = c[i][j+1];
                b[i+1][j+1] = '|';
            }
            else{
                c[i+1][j+1] = c[i+1][j];
                b[i+1][j+1] = '-';
            }
        }
    }
    //printf("\n\nc[%d][%d] = %d \n\n", m, n, c[m][n]);
    return c[m][n];
}

// rekurencyjne wypisywanie "pierwszego napotkanego" najdluzszego wspolnego podciagu
// zaczynajac od danych 'i' i 'j' w dol
void lcsprint(char x[], char b[m+1][n+1], int i, int j){
    if(i == 0 || j == 0){
        return;
    }

    // po napotkaniu '\' najpierw wywolanie potem wypisanie
    // dzieki temu zachowana zostaje odpowiednia kolejnosc
    if(b[i][j] == '\\'){
        lcsprint(x, b, i - 1, j - 1);
        printf("%c", x[i-1]);
    }
    else if(b[i][j] == '|'){
        lcsprint(x, b, i - 1, j);
    }
    else{
        lcsprint(x, b, i, j - 1);
    }
}

// rozwiazanie bylo podejrzanie proste
// zamiast modyfikowac poprzedni algorytm wypisywania stworzylem nowa funkcje
// ktora wywoluje go dla kazdej pozycji w ktorej dlugosc byla maksymalna
// czyli jesli w tabelce byly 4 pozycje z '\' i wartoscia 5 (ktora byla maksymalna)
// to lcsprintall wywola lcsprint 4 razy - po razie z kazdej pozycji
void lcsprintall(char x[], char b[m+1][n+1], int c[m+1][n+1], int max){
    for(int i = 0; i < m + 1; i++){
        for(int j = 0; j < n + 1; j++){
            if(b[i][j] == '\\' && c[i][j] == max){
                printf("-");
                lcsprint(x, b, i, j);
                printf("\n");
            }
        }
    }
}

int main(){
    char x[strlen(word1) + 1];
    char y[strlen(word2) + 1];
    strcpy(x, word1);
    strcpy(y, word2);
    int c[m+1][n+1];
    char b[m+1][n+1];

    printf("given words:\n");
    puts(x);
    puts(y);

    int len = lcslen(x, y, b, c);

    printf("all longest common subsequences for %s and %s:\n",x, y);
    // dla "abbaac" i "bacbacba" lcsprintall wyrzuci dwa razy "abba"
    // dzieje sie tak, poniewaz to ostatnie 'a' to sa dwa rozne a z "abb{aa}c"
    lcsprintall(x, b, c, len);
    printf("length: %d\n", len);

    return 0;
}