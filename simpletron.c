#include <stdio.h>
#include <stdlib.h>

void memoryDump(int acc, int ic, int ir, int op, int opnd, int mem[]) {
    printf("\nRegistros:\n");
    printf("acumulador:          %+05d\n", acc);
    printf("instructionCounter:     %02d\n", ic);
    printf("instructionRegister: %+05d\n", ir);
    printf("operationcode:          %02d\n", op);
    printf("operand:                %02d\n", opnd);

    printf("\nMEMORIA\n");
    printf("   ");
    for (int i = 0; i < 10; i++) {
        printf("%5d ", i);
    }
    printf("\n");
    for (int i = 0; i < 100; i += 10) {
        printf("%2d ", i / 10);
        for (int j = 0; j < 10; j++) {
            printf("%+05d ", mem[i + j]);
        }
        printf("\n");
    }
}

int main() {
    int memory[100] = {0};
    int accumulator = 0;
    int instructionCounter = 0;
    int instructionRegister = 0;
    int operationCode = 0;
    int operand = 0;

    printf("*** Bienvenido a Simpletron! ***\n");
    printf("*** Introduzca su programa una instruccion ***\n");
    printf("*** (o palabra de datos) a la vez en la linea ***\n");
    printf("*** de texto de entrada. Yo indicare el numero ***\n");
    printf("*** de posicion y una interrogacion (?). Usted ***\n");
    printf("*** tecleara entonces la palabra para esa ***\n");
    printf("*** posicion. Haga clic en el boton LISTO para ***\n");
    printf("*** dejar de introducir su programa. ***\n\n");

    int inst = 0;
    int i = 0;
    
    while (1) {
        printf("%02d ? ", i);
        scanf("%d", &inst);
        
        if (inst == 9999) {
            break;
        }
        
        if (inst < -9999 || inst > 9998) {
            continue;
        }
        
        memory[i] = inst;
        i++;
    }

    printf("*** Se termino de cargar el programa ***\n");
    printf("*** Comienza la ejecucion del programa ***\n\n");

    while (instructionCounter < 100) {
        instructionRegister = memory[instructionCounter];
        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;

        switch (operationCode) {
            case 10:
                printf("? ");
                scanf("%d", &memory[operand]);
                instructionCounter++;
                break;
            case 11:
                printf("%+05d\n", memory[operand]);
                instructionCounter++;
                break;
            case 12:
                printf("\n");
                instructionCounter++;
                break;
            case 20:
                accumulator = memory[operand];
                instructionCounter++;
                break;
            case 21:
                memory[operand] = accumulator;
                instructionCounter++;
                break;
            case 30:
                accumulator += memory[operand];
                instructionCounter++;
                break;
            case 31:
                accumulator -= memory[operand];
                instructionCounter++;
                break;
            case 32:
                if (memory[operand] == 0) {
                    printf("*** Intento de dividir entre cero ***\n");
                    printf("*** La ejecucion de Simpletron termino anormalmente ***\n");
                    memoryDump(accumulator, instructionCounter, instructionRegister, operationCode, operand, memory);
                    return 1;
                }
                accumulator /= memory[operand];
                instructionCounter++;
                break;
            case 33:
                accumulator *= memory[operand];
                instructionCounter++;
                break;
            case 34:
                if (memory[operand] == 0) {
                    printf("*** Intento de residuo con divisor cero ***\n");
                    printf("*** La ejecucion de Simpletron termino anormalmente ***\n");
                    memoryDump(accumulator, instructionCounter, instructionRegister, operationCode, operand, memory);
                    return 1;
                }
                accumulator %= memory[operand];
                instructionCounter++;
                break;
            case 40:
                instructionCounter = operand;
                break;
            case 41:
                if (accumulator < 0) {
                    instructionCounter = operand;
                } else {
                    instructionCounter++;
                }
                break;
            case 42:
                if (accumulator == 0) {
                    instructionCounter = operand;
                } else {
                    instructionCounter++;
                }
                break;
            case 43:
                printf("*** Termino la ejecucion de Simpletron ***\n");
                memoryDump(accumulator, instructionCounter, instructionRegister, operationCode, operand, memory);
                return 0;
            default:
                return 1;
        }
    }
    return 0;
}
