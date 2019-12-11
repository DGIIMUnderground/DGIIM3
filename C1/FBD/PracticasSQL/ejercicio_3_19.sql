/**
Enunciado:
    - Mostrar las piezas vendidas por los proveedores de Madrid
    - Usando el operador IN
    - Comparar con el ejercicio 3.15
 */

SELECT codpie
FROM ventas
WHERE codpro IN (SELECT codpro FROM proveedor WHERE ciudad="Madrid");
