/**
Enunciado:
    - Listar las ventas ordenadas por cantidad
    - Si algunas cantidades coinciden, ordenar en funcion de la fecha de manera descendente
 */

SELECT *
FROM ventas
ORDER BY cantidad ASC, fecha DESC;
