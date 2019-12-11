/**
Enunciado:
    - Seleccionar los codigos de los proyectos a los que solo abastece S1
Aproximacion:
    - La primera seleccion coge los proyectos a los que abastece S1
    - La segunda seleccion coge los proyectos a los que abastece un proyecto que no sea S1
    - EXCEPT en SQLite equivale a MINUS
 */
SELECT codpj FROM ventas WHERE codpro="S1" EXCEPT SELECT codpj FROM ventas WHERE codpro!="S1";
