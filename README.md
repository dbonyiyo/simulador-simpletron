# Proyecto Simpletron y Compilador SML
**Materia:** Programación Avanzada

Este repositorio contiene la implementación completa de un simulador de la máquina virtual Simpletron y un compilador que traduce lenguaje de alto nivel a código de máquina SML (Simpletron Machine Language).

## Estructura del Repositorio
* `src/`: Contiene el código fuente en C.
  * `simpletron.c`: Simulador de la máquina virtual con instrucciones matemáticas extendidas (Módulo y Exponenciación).
  * `compilador.c`: Compilador completo con validación de sintaxis, traducción de comandos `let` e `if`, y resolución de saltos lógicos.
* `programas/`: Contiene los archivos de prueba en lenguaje de máquina.
  * `cuenta_regresiva.sml`: Programa SML cargable manualmente para validar la máquina virtual.
* `docs/`: Documentación general del proyecto.

## Instrucciones de Compilación y Ejecución
Para compilar los programas desde la terminal, utilice el compilador GCC:

```bash
gcc src/simpletron.c -o simpletron
gcc src/compilador.c -o compilador