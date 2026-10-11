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

void procesarLinea(char *linea) {
    char *token = strtok(linea, " ");
    if (token != NULL) {
        int numLinea = atoi(token);
        printf("Linea detectada: %d", numLinea);

        token = strtok(NULL, " ");
        if (token != NULL) {
            printf(" | Comando: %s\n", token);
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

int main() {
    char nombreArchivo[100];
    
    printf("Ingrese el nombre del archivo fuente (.simple): ");
    if (scanf("%99s", nombreArchivo) == 1) {
        inicializarCompilador();
        primeraPasada(nombreArchivo);
    }

    return 0;
}