/**
Enunciado:
    - Hallar los codigos de las piezas suministradas a todos los proyectos localizados
    en Londres
*/

SELECT ventas.codpie FROM ventas 
EXCEPT
    SELECT ventas.codpie
    FROM ventas NATURAL JOIN proyecto
    WHERE proyecto.ciudad == "Londres"
    EXCEPT
        SELECT ventas.codpie 
        FROM ventas;
