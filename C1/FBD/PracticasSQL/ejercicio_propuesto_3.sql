/**
Enunciado:  
    - Piezas que hayan sido vendidas por todos los proveedores verificando que
    tienen un proyecto en su misma ciudad
 */

SELECT "Codigo SQL de Antonio Merino" AS "";

SELECT codpie
FROM pieza pie
WHERE NOT EXISTS (
    SELECT v.codpro
    FROM ventas v, proyecto pj, proveedor pro
    WHERE 
        v.codpro == pro.codpro AND
        v.codpj == pj.codpj AND
        pj.ciudad == pro.ciudad

    MINUS

    SELECT codpro
    FROM ventas w
    WHERE w.codpie == pie.codpie
);

SELECT "Codigo de Antonio Merino usando dos no existe" AS "";

SELECT codpie
FROM pieza pie
WHERE NOT EXISTS (
    SELECT v.codpro
    FROM ventas v, proyecto pj, proveedor pro
    WHERE 
        v.codpro == pro.codpro AND
        v.codpj == pj.codpj AND
        pj.ciudad == pro.ciudad AND
        NOT EXISTS (
            SELECT *
            FROM ventas w
            WHERE w.codpie == pie.codpie AND w.codpro == v.codpro
        )
);
