/**
Enunciado:
    - Ciudad y codigo de las piezas suministradas a cualquier proyecto por un
      proveedor que esta en la misma ciudad donde esta el proyecto
Aproximacion:
    1. Tomo los proveedores y proyectos de la misma ciudad usando una reunion con el JOIN
    2. Tomo todas las ventas,proveedor,proyecto en las que proveedor y proyecto son de la misma ciudad gracias a 1.
    3. Tomo los codigos de las piezas que estan en una venta de un proveedor a un proyecto de la misma ciudad
    4. Con el codigo de las piezas, consigo el codigo de la pieza junto a la ciudad de la pieza
 */
SELECT pieza.codpie, pieza.ciudad   /*4*/
FROM                                /*4*/
    pieza                           /*4*/
    NATURAL JOIN                    /*4*/
    (SELECT DISTINCT ventas.codpie  /*3*/
    FROM                            /*3*/
        ventas                      /*2*/
        NATURAL JOIN                /*2*/
        (SELECT * FROM proveedor JOIN proyecto ON (proveedor.ciudad == proyecto.ciudad))); /*1*/
