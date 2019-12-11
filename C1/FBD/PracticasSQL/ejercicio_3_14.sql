/**
Enunciado:
    - Encontrar la pieza con el maximo peso
Aproximacion:
    - A la lista de todas las piezas le resto la lista de las piezas tales que
    existe otra pieza con peso mayor
    - Esto porque la condicion de la pieza de peso maximo es que no existe otra
    pieza tal que su peso sea mayor
*/

SELECT codpie, peso 
FROM pieza
EXCEPT                      /*Equivale a MINUS en Oracle*/
SELECT p1.codpie, p1.peso
FROM pieza p1, pieza p2
WHERE p1.peso < p2.peso
