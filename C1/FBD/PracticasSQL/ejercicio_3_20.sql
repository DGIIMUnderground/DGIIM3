/**
Enunciado:
    - Proyectos que estan en una ciudad en la que se fabrica alguna pieza
 */

SELECT * 
FROM proyecto
WHERE ciudad in (
    SELECT ciudad 
    FROM pieza
);
