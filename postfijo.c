#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 256

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
    postfijo[0] = '\0';
    
    char *token = strtok(infijo, " ");
    
    while (token != NULL) {
        if (isalnum(token[0])) {
            strcat(postfijo, token);
            strcat(postfijo, " ");
        } else if (token[0] == '(') {
            push(&p, '(');
        } else if (token[0] == ')') {
            while (!isEmpty(&p) && stackTop(&p) != '(') {
                int len = strlen(postfijo);
                postfijo[len] = pop(&p);
                postfijo[len + 1] = ' ';
                postfijo[len + 2] = '\0';
            }
            if (!isEmpty(&p)) {
                pop(&p);
            }
        } else {
            while (!isEmpty(&p) && prioridad(stackTop(&p)) >= prioridad(token[0])) {
                int len = strlen(postfijo);
                postfijo[len] = pop(&p);
                postfijo[len + 1] = ' ';
                postfijo[len + 2] = '\0';
            }
            push(&p, token[0]);
        }
        token = strtok(NULL, " ");
    }

    while (!isEmpty(&p)) {
        int len = strlen(postfijo);
        postfijo[len] = pop(&p);
        postfijo[len + 1] = ' ';
        postfijo[len + 2] = '\0';
    }
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