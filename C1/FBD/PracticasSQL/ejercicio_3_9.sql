/**
Enunciado:
    - Mostrar todas las ciudades de la base de datos usando UNION
 */

SELECT ciudad FROM pieza UNION
SELECT ciudad FROM proveedor UNION
SELECT ciudad FROM proyecto;
