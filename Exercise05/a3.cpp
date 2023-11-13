#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct {
    char name[10];
    void (*f)();
} funcRef;

int min(int x, int y) {
    if (x < y) return x;
    else return y;
}

funcRef *initFuncRef(char* name, void(*g)()) {
    int len = strlen(name);
    funcRef *r = (funcRef*)calloc(1, sizeof(funcRef));
    r->f = g;    
    memcpy(r->name, name, min(10, len));    
    return r;
}



void renameFuncRef(funcRef *ref) {
    int i = 0;
    char k = 0;
    
    printf("Neuen Funktionsnamen eingeben: ");
    while (k != EOF && k != '\n') {
        k = getchar();
        if (k != EOF) {
            ref->name[i] = k;
            i++;
        }
    }
    ref->name[i] = 0;
}



void good() {
    printf("i am good\n"); 
    getchar();   
}

void evil() {
    printf("i am evil\n");
    getchar();   
}


int main() {
    funcRef *ref = initFuncRef("spass", good);
    renameFuncRef(ref);
    printf("neuer name: %s\n", ref->name);
    ref->f();
    return 0;
}