/*
Crea la tabla de proveedores
 */
CREATE TABLE proveedor(
    /*
        Codigo de proveedor
            - No nulo
            - Clave primaria
     */
    codpro VARCHAR2(3) CONSTRAINT codpro_no_nulo NOT NULL
    CONSTRAINT codpro_clave_primaria PRIMARY KEY,

    /*
        Nombre proveedor
            - No nulo
     */
    nompro VARCHAR2(30) CONSTRAINT nompro_no_nulo NOT NULL,

    /*
        Status
            - Valor en el intervalo [1,10]
     */
    status NUMBER CONSTRAINT status_entre_1_y_10
    CHECK (status>=1 and status<=10),

    ciudad VARCHAR2(15)
);
