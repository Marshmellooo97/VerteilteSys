#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    char* name;
    float x;
    float y;
    float radius;

} Kreis;

Kreis* KreisConstr(float _x, float _y, float _r) {

Kreis* ret = malloc( sizeof(Kreis) );
Kreis* ret2 = malloc( sizeof(Kreis) );
//Kreis ret;
int v1 = 1;
int v2 = 1;
ret->name = "Kreis1";
ret->x = _x;
ret->y = _y;
ret -> radius = _r;
printf("v1: %p v2: %p data of ret: %p %p", &v1, &v2, ret, ret2);

return ret;

}

void makeEven(int* _i){
    if(*_i % 2 == 1)
        *_i +=1;
    //if(_i[0] % 2 == 1)
        //_i[0] +=1;
}

int main (int argc , char** argv) {

    int zahl = 7;
    makeEven(&zahl);
    printf("Hallo Welt %d \n", zahl);

    Kreis* k1 = KreisConstr(1.0, 2.0, 1.2);

    printf("Kreis: name: %s x: %f y: %f radius: %f\n", k1->name, k1->x, k1->y, k1->radius);
    //free(k1);
    printf("______________________________________________");
    for (int i = 0; i < 2000000; i++) {

        void* ptr = malloc(1024);  // erzeugen
        //free(ptr);                   // Speicher freigeben
        //printf("______________________________________________");
    }
    sleep(15);

int x = 10;
int y = x / 3;
printf("______________________________________________");
printf("%d\n", y);

return 0;
}