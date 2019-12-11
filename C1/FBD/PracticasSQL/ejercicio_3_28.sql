/**
Enunciado:
    - Mostrar el codigo de la pieza de maximo peso
    - Comparar la solucion con los ejercicios 3.14 y 3.23
 */

SELECT codpie FROM (SELECT codpie, MAX(peso) FROM pieza);
