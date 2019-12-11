/**
Enunciado:
    - Proveedores que habiendo tenido relacion con todos los proyectos de paris
    tienen un sumatorio de cantidades de ventas superior al de la media de cualquier
    proveedor
Resuelto por Antonio Merino Gallardo
 */

/*Division para calcular los proveedores que hayan tenido relacion con todos los proyectos de paris*/
SELECT codpro
FROM proveedor pro
WHERE NOT EXISTS(
    SELECT codpj FROM proyecto WHERE ciudad=="Paris"
    EXCEPT SELECT codpj FROM ventas v WHERE v.codpro == pro.codpro
)

INTERSECT

/*Proveedores cuya sumatoria esta por encima de la media*/
SELECT codpro
FROM ventas
GROUP BY codpro
HAVING SUM(cantidad) > SELECT AVG(SUM(cantidad)) FROM ventas GROUP BY codpro);
