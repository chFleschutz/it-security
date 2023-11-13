#include <stdio.h>

void evil() {
    printf("i am evil\n");
}

void q() {
    // bitte modifizieren Sie nur im Bereich unterhalb dieser Zeile
    
    // Toben Sie sich hier aus!
    
    // bitte modifizieren Sie nur im Bereich oberhalb dieser Zeile
    printf("q is done \n");
}

void harmless() {
    q();
}

int main() {
    harmless();
    return 0;
}