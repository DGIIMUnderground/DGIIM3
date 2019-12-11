/**
Enunciado:
    - Codigos de piezas con peso maximo
 */

SELECT codpie
FROM pieza
WHERE peso == (
    SELECT MAX(peso) FROM pieza
);
