/**
Enunciado:
    - Codigos de los proyectos que no utilizan ninguna pieza roja que este 
    suministrada por un proveedor de Londres
    - Es ambiguo, doy mi interpretacion:
        - Se aceptan piezas de Londres pero que no sean rojas
        - Solo no se aceptan piezas rojas y que sean de londres
Pasos:
    - Seleeciono los proyectos suministrados por Londres con piezas rojas
    - Seleeciono los proyectos que no esten en ese grupo de proyectos
 */

SELECT codpj 
FROM ventas 
WHERE codpj NOT IN (
    /*Proyectos provistos por un proveedor de Londres que da una pieza roja*/
    SELECT codpj FROM ventas 
    WHERE codpro IN (SELECT codpro FROM proveedor WHERE ciudad == "Londres")
    AND codpie IN (SELECT codpie FROM pieza WHERE color == "Rojo")
);

