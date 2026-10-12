#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MEM_SIZE 100

typedef struct {
    int symbol;
    char type;
    int location;
} TableEntry;

TableEntry symbolTable[MEM_SIZE];
int symbolCount = 0;
int smlMemory[MEM_SIZE];
int flags[MEM_SIZE];
int instructionCounter = 0;
int dataCounter = MEM_SIZE - 1;

typedef struct {
    int items[MEM_SIZE];
    int top;
} Pila;

void initPila(Pila *p) { p->top = -1; }
int isEmpty(Pila *p) { return p->top == -1; }
void push(Pila *p, int elem) { if (p->top < MEM_SIZE - 1) p->items[++(p->top)] = elem; }
int pop(Pila *p) { return isEmpty(p) ? -1 : p->items[(p->top)--]; }
int stackTop(Pila *p) { return isEmpty(p) ? -1 : p->items[p->top]; }

void inicializarCompilador() {
    for (int i = 0; i < MEM_SIZE; i++) {
        smlMemory[i] = 0;
        flags[i] = -1;
    }
    symbolCount = 0;
    instructionCounter = 0;
    dataCounter = MEM_SIZE - 1;
}

int buscarSimbolo(int symbol, char type) {
    for (int i = 0; i < symbolCount; i++) {
        if (symbolTable[i].symbol == symbol && symbolTable[i].type == type) {
            return symbolTable[i].location;
        }
    }
    return -1;
}

void agregarSimbolo(int symbol, char type, int location) {
    symbolTable[symbolCount].symbol = symbol;
    symbolTable[symbolCount].type = type;
    symbolTable[symbolCount].location = location;
    symbolCount++;
}

int obtenerUbicacionDato(int symbol, char type) {
    int loc = buscarSimbolo(symbol, type);
    if (loc == -1) {
        loc = dataCounter;
        agregarSimbolo(symbol, type, dataCounter);
        dataCounter--;
    }
    return loc;
}

int prioridad(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

void procesarLet(char *expr) {
    char targetVar;
    sscanf(expr, "%c", &targetVar);
    int targetLoc = obtenerUbicacionDato(targetVar, 'V');
    
    char *rightSide = strstr(expr, "= ") + 2;
    
    char postfijo[256] = "";
    Pila p;
    initPila(&p);
    
    char *token = strtok(rightSide, " ");
    while (token != NULL) {
        if (isalnum(token[0])) {
            strcat(postfijo, token);
            strcat(postfijo, " ");
        } else {
            while (!isEmpty(&p) && prioridad(stackTop(&p)) >= prioridad(token[0])) {
                char opStr[3] = {(char)pop(&p), ' ', '\0'};
                strcat(postfijo, opStr);
            }
            push(&p, token[0]);
        }
        token = strtok(NULL, " ");
    }
    while (!isEmpty(&p)) {
        char opStr[3] = {(char)pop(&p), ' ', '\0'};
        strcat(postfijo, opStr);
    }
    
    initPila(&p);
    token = strtok(postfijo, " ");
    while (token != NULL) {
        int loc;
        if (isdigit(token[0])) {
            loc = obtenerUbicacionDato(atoi(token), 'C');
            push(&p, loc);
        } else if (isalpha(token[0])) {
            loc = obtenerUbicacionDato(token[0], 'V');
            push(&p, loc);
        } else {
            int rightLoc = pop(&p);
            int leftLoc = pop(&p);
            
            smlMemory[instructionCounter++] = 2000 + leftLoc;
            
            if (token[0] == '+') smlMemory[instructionCounter++] = 3000 + rightLoc;
            else if (token[0] == '-') smlMemory[instructionCounter++] = 3100 + rightLoc;
            else if (token[0] == '*') smlMemory[instructionCounter++] = 3300 + rightLoc;
            else if (token[0] == '/') smlMemory[instructionCounter++] = 3200 + rightLoc;
            
            int tempLoc = dataCounter--;
            smlMemory[instructionCounter++] = 2100 + tempLoc;
            push(&p, tempLoc);
        }
        token = strtok(NULL, " ");
    }
    
    int finalLoc = pop(&p);
    smlMemory[instructionCounter++] = 2000 + finalLoc;
    smlMemory[instructionCounter++] = 2100 + targetLoc;
}

void procesarIf(char *expr) {
    char *leftOp = strtok(expr, " ");
    char *op = strtok(NULL, " ");
    char *rightOp = strtok(NULL, " ");
    strtok(NULL, " "); 
    char *targetLineStr = strtok(NULL, " ");
    
    int leftLoc = isdigit(leftOp[0]) ? obtenerUbicacionDato(atoi(leftOp), 'C') : obtenerUbicacionDato(leftOp[0], 'V');
    int rightLoc = isdigit(rightOp[0]) ? obtenerUbicacionDato(atoi(rightOp), 'C') : obtenerUbicacionDato(rightOp[0], 'V');
    int targetLine = atoi(targetLineStr);
    
    smlMemory[instructionCounter++] = 2000 + leftLoc;
    smlMemory[instructionCounter++] = 3100 + rightLoc;
    
    int branchOp = 0;
    if (strcmp(op, "==") == 0) branchOp = 4200;
    else if (strcmp(op, "<") == 0) branchOp = 4100;
    else if (strcmp(op, ">=") == 0) branchOp = 4100; 
    
    int loc = buscarSimbolo(targetLine, 'L');
    if (loc != -1) {
        smlMemory[instructionCounter++] = branchOp + loc;
    } else {
        smlMemory[instructionCounter] = branchOp;
        flags[instructionCounter++] = targetLine;
    }
}

void procesarLinea(char *linea) {
    char lineaOriginal[256];
    strcpy(lineaOriginal, linea);
    
    char *token = strtok(linea, " ");
    if (token != NULL) {
        int numLinea = atoi(token);
        if (buscarSimbolo(numLinea, 'L') == -1) {
            agregarSimbolo(numLinea, 'L', instructionCounter);
        }

        token = strtok(NULL, " ");
        if (token != NULL) {
            if (strcmp(token, "rem") == 0) {
                return;
            } else if (strcmp(token, "input") == 0) {
                token = strtok(NULL, " ");
                smlMemory[instructionCounter++] = 1000 + obtenerUbicacionDato(token[0], 'V');
            } else if (strcmp(token, "print") == 0) {
                token = strtok(NULL, " ");
                smlMemory[instructionCounter++] = 1100 + obtenerUbicacionDato(token[0], 'V');
            } else if (strcmp(token, "goto") == 0) {
                token = strtok(NULL, " ");
                int targetLine = atoi(token);
                int loc = buscarSimbolo(targetLine, 'L');
                if (loc != -1) {
                    smlMemory[instructionCounter++] = 4000 + loc;
                } else {
                    smlMemory[instructionCounter] = 4000;
                    flags[instructionCounter++] = targetLine;
                }
            } else if (strcmp(token, "end") == 0) {
                smlMemory[instructionCounter++] = 4300;
            } else if (strcmp(token, "let") == 0) {
                char *expr = strstr(lineaOriginal, "let ") + 4;
                procesarLet(expr);
            } else if (strcmp(token, "if") == 0) {
                char *expr = strstr(lineaOriginal, "if ") + 3;
                procesarIf(expr);
            }
        }
    }
}

void segundaPasada() {
    for (int i = 0; i < MEM_SIZE; i++) {
        if (flags[i] != -1) {
            int targetLine = flags[i];
            int loc = buscarSimbolo(targetLine, 'L');
            if (loc != -1) {
                smlMemory[i] += loc;
            }
        }
    }
}

void primeraPasada(const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    
    if (archivo == NULL) {
        printf("*** Error fatal: No se pudo abrir el archivo '%s' ***\n", nombreArchivo);
        return;
    }

    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), archivo)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        procesarLinea(buffer);
    }

    fclose(archivo);
}

void imprimirSML() {
    printf("\n--- Codigo SML Generado ---\n");
    for (int i = 0; i < instructionCounter; i++) {
        printf("%02d %+05d\n", i, smlMemory[i]);
    }
}

int main() {
    char nombreArchivo[100];
    
    printf("Ingrese el nombre del archivo fuente (.simple): ");
    if (scanf("%99s", nombreArchivo) == 1) {
        inicializarCompilador();
        primeraPasada(nombreArchivo);
        segundaPasada();
        imprimirSML();
    }

    return 0;
}