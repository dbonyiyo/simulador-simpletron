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

void procesarLinea(char *linea) {
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
                int loc = obtenerUbicacionDato(token[0], 'V');
                smlMemory[instructionCounter] = 1000 + loc;
                instructionCounter++;
            } else if (strcmp(token, "print") == 0) {
                token = strtok(NULL, " ");
                int loc = obtenerUbicacionDato(token[0], 'V');
                smlMemory[instructionCounter] = 1100 + loc;
                instructionCounter++;
            } else if (strcmp(token, "goto") == 0) {
                token = strtok(NULL, " ");
                int targetLine = atoi(token);
                int loc = buscarSimbolo(targetLine, 'L');
                if (loc != -1) {
                    smlMemory[instructionCounter] = 4000 + loc;
                } else {
                    smlMemory[instructionCounter] = 4000;
                    flags[instructionCounter] = targetLine;
                }
                instructionCounter++;
            } else if (strcmp(token, "end") == 0) {
                smlMemory[instructionCounter] = 4300;
                instructionCounter++;
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
        imprimirSML();
    }

    return 0;
}