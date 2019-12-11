/**
Enunciado:
    - Encontrar parejas de proveedores que no sean de la misma ciudad
 */

SELECT p1.codpro, p2.codpro, p1.ciudad, p2.ciudad
FROM proveedor p1, proveedor p2 
WHERE p1.ciudad != p2.ciudad;
