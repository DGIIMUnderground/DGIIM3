/**
Enunciado:
    - Fichero que tengo para hacer pruebas sobre la base de datos
 */

/**
Mostrar el código de los proveedores que suministran todas las piezas
    Pi_{codpro, codpie}(ventas) % Pi_codpie(pieza)
 */

SELECT "Mi seleccion" AS "";
SELECT DISTINCT codpie, codpro FROM ventas WHERE codpie NOT IN (
    SELECT ventas.codpie FROM ventas, pieza WHERE ventas.codpie == pieza.codpie 
    EXCEPT
    SELECT codpie FROM ventas
);
