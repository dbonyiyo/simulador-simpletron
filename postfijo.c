#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char items[MAX];
    int top;
} Pila;

void init(Pila *p) {
    p->top = -1;
}

int isEmpty(Pila *p) {
    return p->top == -1;
}

void push(Pila *p, char elem) {
    if (p->top < MAX - 1) {
        p->items[++(p->top)] = elem;
    }
}

char pop(Pila *p) {
    if (!isEmpty(p)) {
        return p->items[(p->top)--];
    }
    return '\0';
}

char stackTop(Pila *p) {
    if (!isEmpty(p)) {
        return p->items[p->top];
    }
    return '\0';
}

int main() {
    printf("Estructura de pila inicializada.\n");
    return 0;
}