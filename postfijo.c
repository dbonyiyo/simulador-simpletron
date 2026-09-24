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

int prioridad(char op) {
    switch (op) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default:  return 0;
    }
}

void convertirPostfijo(char *infijo, char *postfijo) {
    Pila p;
    init(&p);
    int j = 0;

    for (int i = 0; infijo[i] != '\0'; i++) {
        char symb = infijo[i];

        if (isspace(symb)) {
            continue;
        }

        if (isalnum(symb)) {
            postfijo[j++] = symb;
        } else if (symb == '(') {
            push(&p, symb);
        } else if (symb == ')') {
            while (!isEmpty(&p) && stackTop(&p) != '(') {
                postfijo[j++] = pop(&p);
            }
            if (!isEmpty(&p)) {
                pop(&p);
            }
        } else {
            while (!isEmpty(&p) && prioridad(stackTop(&p)) >= prioridad(symb)) {
                postfijo[j++] = pop(&p);
            }
            push(&p, symb);
        }
    }

    while (!isEmpty(&p)) {
        postfijo[j++] = pop(&p);
    }
    postfijo[j] = '\0';
}

int main() {
    char infijo[MAX];
    char postfijo[MAX];

    printf("Expresion: ");
    if (fgets(infijo, sizeof(infijo), stdin) != NULL) {
        infijo[strcspn(infijo, "\n")] = '\0';
        convertirPostfijo(infijo, postfijo);
        printf("Postfijo: %s\n", postfijo);
    }

    return 0;
}