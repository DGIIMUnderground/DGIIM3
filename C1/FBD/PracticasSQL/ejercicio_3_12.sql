/**
Enunciado:
    - Seleccionar todas las ternas (codpie, codpro, codpj) que sean de la misma ciudad
    pero que hayan realizado alguna venta
 */

SELECT ventas.codpie, ventas.codpro, ventas.codpj 
FROM ventas, pieza, proveedor, proyecto
WHERE
    /*Hago la reunion natura para sacar las piezas,proveedor,proyecto relacionadas en una venta*/
    ventas.codpie = pieza.codpie AND
    ventas.codpro == proveedor.codpro AND
    ventas.codpj == proyecto.codpj AND

    /*Selecciono las pieza,proveedor,proyecto de una misma ciudad*/ 
    pieza.ciudad == proveedor.ciudad AND
    proveedor.ciudad == proyecto.ciudad;
