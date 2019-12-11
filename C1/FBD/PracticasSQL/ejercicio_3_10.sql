/**
Enunciado:
    - Seleccionar todas las ciudades de la base de datos usando UNION ALL
 */

SELECT ciudad FROM pieza UNION ALL
SELECT ciudad FROM proveedor UNION ALL
SELECT ciudad FROM proyecto;
