#include <stdio.h>
#include <stdlib.h>

#define SZUKAJ(a,b,...) szukaj( a, b, 0 )

typedef struct node{
    float miesiac_dzien;
    int poziom;
    struct node* lewy;
    struct node* prawy;
} node;

node* nowyOdczyt(float miesiac_dzien, int poziom){
    node* odczyt = (node*)malloc(sizeof(node));

    odczyt->miesiac_dzien = miesiac_dzien;
    odczyt->poziom = poziom;

    odczyt->lewy = NULL;
    odczyt->prawy = NULL;

    return odczyt;
}

node* wstaw(node* korzen, float miesiac_dzien, int poziom){
    if(korzen == NULL){
        return(nowyOdczyt(miesiac_dzien, poziom));
    }
    else{
        if(miesiac_dzien <= korzen->miesiac_dzien){
            korzen->lewy = wstaw(korzen->lewy, miesiac_dzien, poziom);
        }
        else{
            korzen->prawy = wstaw(korzen->prawy, miesiac_dzien, poziom);
        }
    }
}

node* szukaj(node* korzen, float miesiac_dzien, int poziom){
    if(korzen->miesiac_dzien == miesiac_dzien){
        return korzen;
    }
    else if(korzen->lewy == NULL && korzen->prawy == NULL){
        return nowyOdczyt(miesiac_dzien, poziom);
    }
    else if(miesiac_dzien > korzen->miesiac_dzien){
        szukaj(korzen->prawy, miesiac_dzien, korzen->poziom);
    }
    else{
        szukaj(korzen->lewy, miesiac_dzien, korzen->poziom);
    }
}


void drukuj(node* odczyt){
    if(odczyt->lewy != NULL){
        drukuj(odczyt->lewy);
    }
    printf("| %05.2f - %d |", odczyt->miesiac_dzien, odczyt->poziom);
    if(odczyt->prawy != NULL){
        drukuj(odczyt->prawy);
    }
}

node* najmniejszaWartosc(node* odczyt){
    while(odczyt->lewy != NULL){
        odczyt = odczyt->lewy;
    }
    return odczyt;
}

node* usun(node* korzen, float miesiac_dzien){
    if(korzen == NULL){
        return NULL;
    }
    else if(miesiac_dzien < korzen->miesiac_dzien){
        korzen->lewy = usun(korzen->lewy, miesiac_dzien);
    }
    else if(miesiac_dzien > korzen->miesiac_dzien){
        korzen->prawy = usun(korzen->prawy, miesiac_dzien);
    }
    else{
        if(korzen->lewy == NULL && korzen->prawy == NULL){
            korzen == NULL;
        }
        else if(korzen->lewy == NULL){
            korzen = korzen->prawy;
        }
        else if(korzen->prawy == NULL){
            korzen = korzen->lewy;
        }
        else{
            node* temp = najmniejszaWartosc(korzen->prawy);
            korzen->miesiac_dzien = temp->miesiac_dzien;
            korzen->prawy = usun(korzen->prawy, temp->miesiac_dzien);
        }
    }
    return korzen;
}

void polacz(node* korzen1, node* korzen2){
    if(korzen2->lewy != NULL){
        polacz(korzen1, korzen2->lewy);
    }
    korzen1 = wstaw(korzen1, korzen2->miesiac_dzien, korzen2->poziom);
    if(korzen2->prawy != NULL){
        polacz(korzen1, korzen2->prawy);
    }
}

void drukujPomiedzy(node* odczyt, float d1, float d2){
    if(odczyt->lewy != NULL){
        drukujPomiedzy(odczyt->lewy, d1, d2);
    }
    if(odczyt->miesiac_dzien >= d1 && odczyt->miesiac_dzien <= d2){
        printf("| %05.2f - %d |", odczyt->miesiac_dzien, odczyt->poziom);
    }
    if(odczyt->prawy != NULL){
        drukujPomiedzy(odczyt->prawy, d1, d2);
    }
}

int main()
{
    node* zestaw1 = NULL;
    node* zestaw2 = NULL;

    zestaw1 = wstaw(zestaw1, 1.20, 5);
    zestaw1 = wstaw(zestaw1, 8.22, 6);
    zestaw1 = wstaw(zestaw1, 5.30, 4);
    zestaw1 = wstaw(zestaw1, 7.01, 5);
    zestaw1 = wstaw(zestaw1, 10.16, 5);

    zestaw2 = wstaw(zestaw2, 2.15, 7);
    zestaw2 = wstaw(zestaw2, 6.24, 6);
    zestaw2 = wstaw(zestaw2, 11.12, 7);
    zestaw2 = wstaw(zestaw2, 12.24, 8);


    printf("\nzestaw1: \n");
    drukuj(zestaw1);

    printf("\n\nzestaw2: \n");
    drukuj(zestaw2);

    printf("\n\npolaczone: \n");
    polacz(zestaw1, zestaw2);
    drukuj(zestaw1);

    printf("\n\npolaczone, drukuj w lato: \n");
    drukujPomiedzy(zestaw1, 06.20, 09.22);

    printf("\n\n szukaj:\n");
    printf("%05.2f - %d\n", 1.20, SZUKAJ(zestaw1, 1.20)->poziom);
    printf("%05.2f - %d\n", 12.20, SZUKAJ(zestaw1, 12.20)->poziom);

    return 0;
}
