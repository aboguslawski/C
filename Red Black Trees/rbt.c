//                                          pmp@inf.ug.edu.pl  2014
// drukowanie drzew cz-cz z wartownikiem wspolnym
// dla wszystkich wezlow:
// drukowanie na ekran tekstowy i przez dot z pakietu graphviz

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 1        /* stala oznaczajaca kolor wezla */
#define BLACK 0      /* stala oznaczajaca kolor wezla */
#define SZER_EKR 100 /* szerokosc ekranu */
#define IL_POZ   10   /* ilosc poziomow drzewa, ktore beda wydrukowane   */
                     /* gwiazdka bedzie sygnalizowac istnienie nizszych */
                     /* poziomow                                        */

/* struktury danych do reprezentowania drzewa */
typedef struct wezel *Wskwezla; /* wskaznik na wezel drzewa  */
typedef struct wezel{
    int klucz;
    Wskwezla left, right, p;
	int kolor;
} Twezla ;  /* typ wezla */

/* drzewa z wartownikami: wezel wskazywany przez "nil" jest wartownikiem
   zastepujacym NULL; dla korzenia pole "p" ma wartosc "nil";
   przy usuwaniu trzeba odpowiednio wypelnic pole nil->p
*/
//zmienne globalne
Wskwezla nil;
int count = 0;
int depth = 0;
int min = 0;
int max = 0;
int redd = 0;
Wskwezla korzen;

void nilInit(void){
/* funkcja inicjujaca nil; musi byc wywolana przed budowaniem drzewa */
  nil= (Wskwezla) malloc(sizeof(Twezla));
  nil->p = NULL;
  nil->kolor = BLACK;
  nil->left = nil->right = NULL;
}

void drukuj(Wskwezla w);
/* funkcja drukujaca drzewo binarne na ekranie (tutaj tylko deklaracja) */
/* funkcja drukuje drzewo o korzeniu "w"                                */

void drukujDot(Wskwezla r);
  // generuje w plikach drzewo0.gv,  drzewo1.gv ...
  // opis drzew o korzeniu r do wydrukowania przez program dot
  // zlecenie "dot -Tpdf -O drzewo1.gv" utworzy plik "drzewo1.gv.pdf"

/* ------------  implementacja ------------------------------------- */
char wydruk[IL_POZ+1][SZER_EKR];

void drukujost(Wskwezla w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       if (w==nil)   return;
       wydruk[poziom][srodek]='*';
}

void drukujwew(Wskwezla w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       int i,dl;
       char s[19];
       if (w==nil)    return;
       if (w->kolor==BLACK)
         dl=sprintf(s,"%d",w->klucz);
       else
	 //	        dl=sprintf(s,"\e[31m%+d\e[0m",w->klucz);
       dl=sprintf(s,"%+d",w->klucz);
       for (i=0;i<dl;i++)
         wydruk[poziom][srodek-dl/2+i]=s[i];
       if (++poziom<IL_POZ){
         drukujwew(w->left,l,srodek,poziom) ;
         drukujwew(w->right,srodek+1,p,poziom) ;
       }
       else {
         drukujost(w->left,l,srodek,poziom) ;
         drukujost(w->right,srodek+1,p,poziom) ;
       }
}

void drukuj(Wskwezla w){
  int j,i;
  for (i=0;i<=IL_POZ;i++)
    for (j=0;j<SZER_EKR;j++)
      wydruk[i][j] = ' ';
  drukujwew(w,0,SZER_EKR,0);
  for (i=0;i<=IL_POZ;i++){
      for (j=0;j<SZER_EKR;j++)
        putchar(wydruk[i][j]);
      printf("\n");
  }
}

void drukujKrawedz(FILE *plikwy, Wskwezla r, int z, Wskwezla syn, int zs){
// wezel r o numerze z jest juz wydrukowany
// teraz drukujemy jego syna "syn" o numerze zs oraz krawedz miedzy nimi
  if (syn == nil){
    fprintf(plikwy,"%d [shape=circle, style=invisible, label=\"",zs);
    fprintf(plikwy,"%d ",0);
    fprintf(plikwy,"\"]\n");
    fprintf(plikwy,"%d -- %d [style=invis];\n ",z,zs);
  } else{
    if (syn->kolor == RED)
      fprintf(plikwy,"%d [shape=circle, color=red, label=\"",zs);
    else
      fprintf(plikwy,"%d [shape=circle, color=black, label=\"",zs);
    fprintf(plikwy,"%d ",syn->klucz);
    fprintf(plikwy,"\"]\n");
    fprintf(plikwy,"%d -- %d ;\n",z,zs);
  }
}

int rekDrukujDot(Wskwezla r, int z, FILE *plikwy){
  // drukuje drzewo o korzeniu r
  // z  - numer wezla r
  // zwraca najwiekszy numer wezla w poddrzewie, ktorego jest korzeniem
  // zakladamy, ze korzen r jest juz wydrukowany
  int nz,i;
  if (r == nil) return z;
  else{
    drukujKrawedz(plikwy,r,z,r->left,z+1);
    nz=rekDrukujDot(r->left,z+1,plikwy);
    drukujKrawedz(plikwy,r,z,r->right,nz+1);
    nz=rekDrukujDot(r->right,nz+1,plikwy);
    return nz;
  }
}

void drukujDot(Wskwezla r){
  // generuje w plikach drzewo0.gv, dzrewo1.gv, ...
  // opisy drzew o korzeniu r pzeznaczone do wydrukowania przez program dot
  // zlecenie "dot -Tpdf -O drzewo1.gv" utworzy plik "drzewo1.gv.pdf"
  static int gdzie=0;
  char numer[10];
  char nazwap[20]="drzewo";
  FILE *plikwy;
  snprintf(numer,9,"%d",gdzie);
  strcat(nazwap,numer);
  strcat(nazwap,".gv");
  plikwy=fopen(nazwap,"w");
  gdzie++;
  fprintf(plikwy, "graph drzewo{\n");
  fprintf(plikwy, "size = \"2,20\"");
  if (r!=nil){
    if (r->kolor == RED)
      fprintf(plikwy,"%d [shape=circle, color=red, label=\"",0);
    else
      fprintf(plikwy,"%d [shape=circle, color=black, label=\"",0);
    fprintf(plikwy,"%d ",r->klucz);
    fprintf(plikwy,"\"]\n");
    rekDrukujDot(r,0,plikwy);
  }
  fprintf(plikwy, "}\n");
  fclose(plikwy);
  printf("wydrukowane %s\n",nazwap);
}

/* ------------- miejsce na inne funkcje -------------------- */

Wskwezla nowyWezel(int klucz, int kolor);
void repairInsert(Wskwezla x);
void repairRemove(Wskwezla n, Wskwezla m, Wskwezla x);
void deleteCase(Wskwezla x);

void rotateL(Wskwezla x){
    Wskwezla temp;
    if(x->p == nil){
        korzen = x->right;
        temp = x->right->left;
        x->p = x->right;
        x->p->left = x;
        x->p->p = nil;
        x->right = temp;
        x->right->p = x;
    }
    else if(x == x->p->left){
        temp = x->right->left;
        x->right->p = x->p; //
        x->p->left = x->right; //
        x->p = x->right;
        x->p->left = x;
        x->right = temp;
        x->right->p = x;
    }
    else if(x == x->p->right){
        temp = x->right->left;
        x->right->p = x->p; //
        x->p->right = x->right; //
        x->p = x->right;
        x->p->left = x;
        x->right = temp;
        x->right->p = x;
    }
}

void rotateR(Wskwezla x){
    Wskwezla temp;              //temp - "przeszczepiane" poddrzewo
    if(x->p == nil){            //jesli ojcem wezla jest nil to rotacja jest w korzeniu
        korzen = x->left;       //przestawienie korzenia
        temp = x->left->right;
        x->p = x->left;
        x->p->right = x;
        x->p->p = nil;
        x->left = temp;
        x->left->p = x;
    }
    else if(x == x->p->left){
        temp = x->left->right;
        x->left->p = x->p;
        x->p->left = x->left;
        x->p = x->left;
        x->p->right = x;
        x->left = temp;
        x->left->p = x;
    }
    else if(x == x->p->right){
        temp = x->left->right;
        x->left->p = x->p;
        x->p->right = x->left;
        x->p = x->left;
        x->p->right = x;
        x->left = temp;
        x->left->p = x;
    }
}

void insert(int key, Wskwezla root){
    if(key < root->klucz){
        if(root->left == nil){          //root to lisc, podczepia sie do niego n
            Wskwezla n = nowyWezel(key, RED);
            root->left = n;
            n->p = root;
            repairInsert(n);
        }
        else{
            insert(key, root->left);
        }
    }
    else{
        if(root->right == nil){
            Wskwezla n = nowyWezel(key, RED);
            root->right = n;
            n->p = root;
            repairInsert(n);
        }
        else{
            insert(key, root->right);
        }
    }
}

void repairInsert(Wskwezla x){
    Wskwezla wujek;
    if(x->p == nil){    //x jest w korzeniu
        x->kolor = BLACK;
    }
    else if(x->p->kolor == RED){

        //ojciec jest prawym synem
        if(x->p == x->p->p->right){
            wujek = x->p->p->left;

            //przypadek 2
            if(wujek->kolor == BLACK && x == x->p->left){
                rotateR(x->p);
                x->right->p->kolor = BLACK;
                x->right->p->p->kolor = RED;
                rotateL(x->right->p->p);
            }

            //przypadek 3
            else if(wujek->kolor == BLACK && x == x->p->right){
                x->p->kolor = BLACK;
                x->p->p->kolor = RED;
                rotateL(x->p->p);
            }

            //przypadek 1
            else if(wujek->kolor == RED){
                x->p->kolor = BLACK;
                wujek->kolor = BLACK;
                x->p->p->kolor = RED;
                repairInsert(x->p->p);
            }
        }

        //ojciec jest lewym synem
        else if(x->p == x->p->p->left){
            wujek = x->p->p->right;

            //przypadek 2
            if(wujek->kolor == BLACK && x == x->p->right){
                rotateL(x->p);
                x->left->p->kolor = BLACK;
                x->left->p->p->kolor = RED;
                rotateR(x->left->p->p);
            }

            //przypadek 3
            else if(wujek->kolor == BLACK && x == x->p->left){
                x->p->kolor = BLACK;
                x->p->p->kolor = RED;
                rotateR(x->p->p);
            }

            //przypadek 1
            else if(wujek->kolor == RED){
                x->p->kolor = BLACK;
                wujek->kolor = BLACK;
                x->p->p->kolor = RED;
                repairInsert(x->p->p);
            }
        }
    }
}

void print(Wskwezla korzen){    //polaczenie dwoch funkcji zeby latwiej bylo wywolywac w mainie
     drukuj(korzen);
     drukujDot(korzen);
}

void countRed(Wskwezla root){
    if(root->kolor == RED){
        count++;
    }
    if(root->left != nil){
        countRed(root->left);
    }
    if(root->right != nil){
        countRed(root->right);
    }
}

int getRed(Wskwezla root){
    count = 0;
    countRed(root);
    return count;
}

void getMinDepth(Wskwezla root){

    if(root->left != nil){
        count++;
        getMinDepth(root->left);
    }
    if(root->right != nil){
        count++;
        getMinDepth(root->right);
    }
    if(root->right == nil && root->left == nil){
        if(min == 0){
            min = 999;
        }
        if(count < min){
            min = count;
        }
    }
    count--;
}

void getMaxDepth(Wskwezla root){

    if(root->left != nil){
        count++;
        getMaxDepth(root->left);
    }
    if(root->right != nil){
        count++;
        getMaxDepth(root->right);
    }
    if(root->right == nil && root->left == nil){
        if(count > max){
            max = count;
        }
    }
    count--;
}

int getMinDepthValue(Wskwezla root){
    count = 1;
    getMinDepth(korzen);
    return min;
}

int getMaxDepthValue(Wskwezla root){
    count = 1;
    getMaxDepth(korzen);
    return max;
}

Wskwezla find(int key, Wskwezla root){
    if(key == root->klucz){
        return root;
    }
    else if(key < root->klucz){
        if(root->left != nil){
           find(key, root->left);
        }
        else{
            printf("w drzewie nie ma wezla o kluczu %d.\n", key);
            return NULL;
        }
    }
    else if(key > root->klucz){
        if(root->right != nil){
            find(key, root->right);
        }
        else{
            printf("w drzewie nie ma wezla o kluczu %d.\n", key);
            return NULL;
        }
    }
}

Wskwezla getMax(Wskwezla root){     //potrzebne do usuwania, zwraca wezel z najwyzsza wartoscia z poddrzewa
    if(root->right == nil){
        return root;
    }
    if(root->right != nil){
        getMax(root->right);
    }
}

void removeNode(int key, Wskwezla root){
    Wskwezla n = find(key, root);
    printf("usuwam n = %d...\n", n->klucz);
        //n jest lisciem
        if(n->left == nil && n->right == nil){
            if(n == n->p->left){
                n->p->left = nil;
                repairRemove(n, n->right, n->right);
                free(n);
            }
            else{
                n->p->right = nil;
                repairRemove(n, n->right, n->right);
                free(n);
            }
        }
        //n ma prawego syna
        else if(n->right != nil && n->left == nil){
            if(n == n->p->left){
                n->p->left = n->right;
                n->right->p = n->p;
                repairRemove(n, n->right, n->right);
                free(n);
            }
            if(n == n->p->right){
                n->p->right = n->right;
                n->right->p = n->p;
                repairRemove(n, n->right, n->right);
                free(n);
            }
        }
        //n ma lewego syna
        else if(n->right == nil && n->left != nil){
            if(n == n->p->left){
                n->p->left = n->left;
                n->left->p = n->p;
                repairRemove(n, n->left, n->left);
                free(n);
            }
            if(n == n->p->right){
                n->p->right = n->left;
                n->left->p = n->p;
                repairRemove(n, n->left, n->left);
                free(n);
            }
        }
        //n ma dwóch synów
        else if(n->right != nil && n->left != nil){
            Wskwezla m = getMax(n->left);
            n->klucz = m->klucz;
            if(m == m->p->right){
                m->p->right = m->left;
                m->left->p = m->p;
                repairRemove(n, m, n->left);
                free(m);
            }
            else if(m == m->p->left){
                m->p->left = m->left;
                m->left->p = m->p;
                repairRemove(n, m, m->left);
                free(m);
            }
        }

}
// n - node deleted, m - replacement
void repairRemove(Wskwezla n, Wskwezla m, Wskwezla x){
    if(n->kolor == RED && (m == nil || m->kolor == RED)){
        printf("naprawione\n");
    }
    else if(n->kolor == BLACK && m->kolor == RED){
        m->kolor = BLACK;
        printf("m na czarno, naprawione\n");
    }
    else if(n->kolor == RED &&  m->kolor == BLACK){
        m->kolor = RED;
        deleteCase(x);
    }
    else if(n->kolor == BLACK && (m == nil || m->kolor == BLACK)){
        deleteCase(x);
    }

}
// w - parent's other child
void deleteCase(Wskwezla x){
    int xRightChild = 0; // 0 - lewy , 1 - prawy
    Wskwezla w;
    if(x == x->p->right){
        xRightChild = 1;
        w = x->p->left;
    }
    else{
        xRightChild = 0;
        w = x->p->right;
    }



    //case 3 or 4
    if(x->kolor == BLACK && w->kolor == BLACK){
            //3, x left
        if(xRightChild == 0 && w->left->kolor == RED && w->right->kolor == BLACK){
            w->left->kolor = BLACK;
            w->kolor = RED;
            rotateR(w);
        }
            //3, x right
        else if(xRightChild == 1 && w->right->kolor == RED && w->left->kolor == BLACK){
            w->right->kolor = BLACK;
            w->kolor = RED;
            rotateL(w);
        }
            //4, x left
        if(xRightChild == 0 && w->right->kolor == RED){
            w->kolor = x->p->kolor;
            x->p->kolor = BLACK;
            w->right->kolor = BLACK;
            rotateL(x->p);
        }
            //4, x right
        else if(xRightChild == 1 && w->left->kolor == RED){
            w->kolor = x->p->kolor;
            x->p->kolor = BLACK;
            w->left->kolor = BLACK;
            rotateR(x->p);
        }
    }
    //case 2
    else if(x->kolor == BLACK && w->kolor == BLACK && (w->left->kolor == BLACK || w->left == nil) && (w->right->kolor == BLACK || w->right == nil)){
        w->kolor = RED;
        if(x->p->kolor == RED){
            x->p->kolor = BLACK;
        }
        else{
            deleteCase(x->p);
        }
    }
    //case 1
    else if(x->kolor == BLACK && w->kolor == RED){
        w->kolor = BLACK;
        x->p->kolor = RED;
        if(xRightChild == 0){
            rotateL(x->p);
        }
        else{
            rotateR(x->p);
        }
        deleteCase(x);
    }
    //case 0
    else if(x->kolor == RED){
        x->kolor = BLACK;
    }
}


/* ----------------- program testujacy -----------------------*/

Wskwezla nowyWezel(int klucz, int kolor){
// tworzy nowy wezel. Przydatne do testowania wydruku
     Wskwezla w = (Wskwezla) malloc(sizeof(Twezla));
     w->p = nil;
     w->klucz=klucz;
     w->kolor = kolor;
     w->left = w->right = nil;
     return w;
}

int main(){
     nilInit();
     korzen = nil;
     korzen->p = nil;
// ---------- test wstawiania, usuwania i naprawiania drzewa *trzeba odznaczyc*------------------------------

     korzen = nowyWezel(38, BLACK);
     insert(31,korzen);
     print(korzen);
     insert(22,korzen);
     print(korzen);
     insert(8,korzen);
     print(korzen);
     insert(20,korzen);
     print(korzen);
     insert(5,korzen);
     print(korzen);
     insert(10,korzen);
     print(korzen);
     insert(9,korzen);
     print(korzen);
     insert(21,korzen);
     print(korzen);
     insert(27,korzen);
     print(korzen);
     insert(29,korzen);
     print(korzen);
     insert(25,korzen);
     print(korzen);
     insert(28,korzen);

    redd = getRed(korzen);
    min = getMinDepthValue(korzen);
    max = getMaxDepthValue(korzen);

    printf("red: %d, minDepth: %d, maxDepth: %d\n", redd, min, max);
    print(korzen);

    removeNode(22, korzen);
    print(korzen);
    removeNode(31, korzen);
    print(korzen);
    removeNode(10, korzen);
    print(korzen);


// ------------ test zliczania czerwonych, min i max dlugosci sciezki dla 1000 wezlow ------------------------

    korzen = nowyWezel(1, BLACK);
    for(int i = 2; i <= 1000; i++){
        insert(i, korzen);
    }
    redd = getRed(korzen);
    min = getMinDepthValue(korzen);
    max = getMaxDepthValue(korzen);

    printf("n:1000, red: %d, minDepth: %d, maxDepth: %d\n", redd, min, max);



     //getchar();

     return 0;
}
