/**
Enunciado:
    - Mostrar las piezas vendidas por los proveedores de Madrid
 */

SELECT ventas.codpie 
FROM
    ventas 
    NATURAL JOIN 
    (SELECT * FROM proveedor WHERE ciudad=="Madrid");
