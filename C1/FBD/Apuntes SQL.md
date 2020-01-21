---
title: Apuntes de SQL
author: Sergio Quijano Rey
---

--------------------------------------------------------------------------------

# DDL

* El objetivo del *Data Description Language* es crear las tablas, imponer restricciones y alterar los esquemas de las tablas

## Creación de tablas

~~~sql
CREATE TABLE nombre-tabla(
    nombre-atributo1 tipo-atributo1 [NOT NULL] [DEFAULT expr],
    nombre-atributo2 tipo-atributo2 [NOT NULL] [DEFAULT expr],...
    [PRIMARY KEY(nombre-atributo1, nombre-atributo2...),]
    [UNIQUE (nombre-atributo1, nombre-atributo2...),]
    [FOREIGN KEY(nombre-atributo1, nombre-atributo2...)
    REFERENCES nombre-tabla(nombre-atributo, ...),]
    [CHECK(condicion)]
);
~~~

## Otros comandos

* `DROP TABLE <nombre_tabla>`: elimina por completo una tabla

## Alteración de tablas

* `ALTER TABLE nombre_tabla modificador;`: esquema básico
* `ADD (atributo [tipo] [DEFAULT expresion] [restriccion_atributo]);`, se añade una columna
* `ADD CONSTRAINT nombre_restriccion [[UNIQUE | PRYMARY KEY] (lista_columnas) | FOREING KEY (lista_columnas) REFERENCES tabla(lista_col) | CHECK (condicion)];`: se añade una restricción de integridad
* `DROP CONSTRAINT nombre_restriccion [CASCADE];`: se borra una restricción de integridad

--------------------------------------------------------------------------------

# DML

* El objetivo del *Data Manipulation Language* es modifar tuplas, insertar tuplas y borrar tuplas

## Inserción de tuplas

* Forma general: `INSERT INTO nombre_tabla [(column1, column2,...)] VALUES(valor1, valor2,...);`
* Insertar tuplas usando valores de otra tabla: `INSERT INTO nombre_tabla [(column1, column2,...)] (SELECT column1, column2,... FROM nombre_tabla2);`

## Selección de tuplas

* Mostrar contenido de las tablas: `SELECT * FROM tabla`;
* Forma general simple: `SELECT campo1, campo2, ... FROM tabla WHERE condicion`
* Consultar las tablas utilizando el catálogo: `SELECT table_name FROM user_tables;` (propio de MySQL)

### Consulta completa

~~~sql
SELECT 
    [DISTINCT | ALL ]
    expresion [alias_columna_expresion]
    {, expresion [alias_columna_expresion]}
    FROM [esquema.]tabla|vista [alias_tabla_o_vista]
    [WHERE condicion]
    [HAVING condicion]
    [{UNION | UNION_ALL | INTERSECT | MINUS } <SELECT instruccion> ] /*Operaciones sobre conjuntos*/
    [ORDER BY {expresion} [ASC | DESC]] /*Ordenar las tuplas segun algun criterio*/
~~~

* `DISTINCT`: para no mostrar tuplas repetidas
* `UNION` hace la unión eliminando duplicados, `UNION ALL` no elimina duplicados

### Operadores adicionales

* `LIKE`: para usar expresiones regulares
    * `%` sustituye por cualquier cadena de caracteres: `*`
    * `_` sustituye por solo un caracter: `?`
* `round(float, num_of_decimals)`: para redondear
* `trunc(float, num_of_decimals)`: para truncar
* `floor(float, num_of_decimals)`: para redondear a la baja
* `ceil(float, num_of_decimals)`: para redondear a la alta
* `IS [NOT] NULL`: para comprobar valores nulos
* Ejemplos:
    * `SELECT * FROM proveedor WHERE ciudad LIKE "L%";`
    * `SELECT round(cantidad / 12, 3) FROM ventas;`

### Producto cartesiano

* `SELECT proyeccion FROM tabla1, tabla2,... WHERE condicion;`
* Por ejemplo, `SELECT * FROM ventas, proveedor;`
* Podemos hacer la reunión natural del ejemplo con: `SELECT * FROM ventas, proveedor WHERE ventas.codpro == proveedor.codpro;`

### Reunión Natural

* Se puede hacer a mano como ya se ha indicado: `SELECT * FROM ventas, proveedor WHERE ventas.codpro == proveedor.codpro;`
* `SQL` dispone del operador `NATURAL JOIN` para hacer la equireunión de dos tablas sobre los campos que se llamen igual
* Forma general: `SELECT proyeccion FROM table1 NATURAL JOIN table2`
* Se puede hacer `SELECT proyeccion FROM table1 NATURAL JOIN SELECT(...)`
* Se puede hacer una reunión no natural:
    * Forma general: `SELECT proyeccion FROM table1 JOIN table2 ON (condicion_booleana)`
    * Ejemplo: `SELECT nompro, cantidad FROM proveedor JOIN ventas ON ventas.codpro == proveedor.codpro);`
    
### Funciones de agregación

* Usando los operadores `SUM(), MIN(), MAX(), AVG(), COUNT(), STDDEV()`
* Se puede usar `DISTINCT` para que se hagan los cálculos sin repetir valores
* Ejemplos:
    * `SELECT MAX(cantidad), MIN(cantidad), SUM(cantidad) FROM ventas;`
    * `SELECT MAX(DISTINCT cantidad), MIN(DISTINCT cantidad), SUM(DISTINCT cantidad) FROM ventas;`

### Creación de Grupos

* Forma general: `SELECT expresion FROM tablas WHERE condicion GROUP BY expresion {, expresion} HAVING condicion_grupo`
* Permite hacer consultas sobre condiciones sobre un atributo
* Ejemplo: para cada proveedor, mostrar la cantidad de ventas realizadas y el máximo de
unidades suministrado en una venta:

~~~sql
SELECT codpro, COUNT(*), MAX(cantidad)
FROM ventas
GROUP BY (codpro);
~~~

* Ejemplo: Hallar la cantidad media de ventas realizadas por cada proveedor, indicando solamente los códigos de proveedores que han hecho más de 3 ventas.

~~~sql
SQL> SELECT codpro, AVG(cantidad)
FROM ventas
GROUP BY (codpro)
HAVING COUNT(*) >3;
~~~

### División

#### Enfoque Álgebra Relacional

* `R1 % R2 = PiA(R1) - PiA((PiA(R1) x R2) - R1)` donde `A = {AtributosR1} - {AtributosR2}`
* Así para hacer una división en `SQL` traducimos esta fórmula a los operadores `SQL`

#### Enfoque Cálculo sobre tuplas

* `R1 % R2 = {t / R1(t) ^ not exists p(R2(p) and t_a not in p)}`, donde `A` son los atributos sobre los que hacemos la división
* Se traduce el conjunto con operadores de `SQL`

#### Enfoque Mixto

* Usar la diferencia (algebraico) con el no existe (del cálculo)
* Elementos que hagan que el conjunto de todos los elementos menos el conjunto de los elementos que verifican los atributos sea vacío
* Es decir, que no haya tuplas en la diferencia de los conjuntos

### Definicion de alias

* Útil para acortar nombres, pero es necesario en algunos casos. Por ejemplo, cuando hacemos el producto cartesiano de una tabla por sí misma
* Notación: `SELECT proyeccion FROM tabla1 alias1, tabla2 alias2, .... WHERE condicion_usando_alias;`

### Ordenacion de resultados 

* En principio, no hay orden en tuplas ni columnas, aunque en verdad sí. Pero no debemos presuponerlo. Para ello usamos la ordenación que nos ofrece `SQL`
* Forma general: `SELECT proyeccion FROM tablas WHERE condicion ORDER BY expresion [ASC | DESC]`
* Se puede usar más de una expresión de ordenación separadas por comas

### Consultas sobre el catálogo (propio de Oracle)

* `SELECT table_name FROM USER_TABLES;`
* `SELECT table_name FROM ALL_TABLES`

## Subconsultas

* Forma general: `SELECT <expresion> FROM tabla WHERE <expresion> OPERADOR <SELECT instruccion>`
* En esta sección vamos a ver cómo podemos usar la parte de `OPERADOR <SELECT instruccion>`

### Pertenencia

* Con el operador `IN`
* Por ejemplo, para seleccionar piezas provistas por los proveedores de Londres sin usar una reunión:

~~~sql
SELECT codpie
FROM ventas
WHERE codpro IN
(SELECT codpro FROM proveedor WHERE ciudad = 'Londres');
~~~

### Existencia

* El operador devuelve verdadero cuando existe alguna tupla que cumple la selección, es decir, cuando la selección sobre la que se aplica no es vacía
* Ejemplo: encontrar proveedores que suministran la pieza "P1"

~~~sql
SELECT codpro
FROM proveedor
WHERE EXISTS (
    SELECT * FROM ventas
    WHERE ventas.codpro = proveedor.codpro
    AND ventas.codpie='P1'
);
~~~

### Comparadores sobre conjuntos

* Operadores relacionales `<, <=, >, >=, <>` junto a cuantificadores como `ANY, ALL`

## Editar tuplas

* Forma general: `UPDATE nombre_tabla SET nombre_atributo = 'nuevovalor' [, nombre_atributo2 = 'nuevovalor2'...] [WHERE <condicion> ];`

## Borrado de tuplas

* Forma general: `DELETE [FROM] tabla [WHERE condicion]`
* Borrar todas las tuplas de una tabla: `DELETE [FROM] tabla`
 
## Otros

### Tipo de dato fecha

* `SYSDATE`: devuelve la fecha actual, útil para trabajar con fechas u horas
* Operaciones sobre `DATE`:
    * Suma y resta: para obtener fechas relativas a una dada
* Formateo de fechas: 
    * `INSERT INTO tabla VALUES (val1, ... , TO_DATE("22/10/2019", "dd/mm/yyyy"))`
    * `SELECT TO_CHAR(fecha_alta, "dd/mm/yyyy") FROM tabla`
* **Notas**:
    * `TO_CHAR`: se usa para cuando queremos formatear las salidas de las consultas
    * `TO_DATE`: se usa en los `WHERE` para hacer comparaciones entre fechas

--------------------------------------------------------------------------------

# Definición de vistas en un DBMS

* Se crea una pseudo-tabla con la salida de una consulta sobre las tablas de la base de datos
* Puede ser usada como variable en la que se recoge la salida de una consulta, aunque no es demasiado práctico
* Normalmente las vistas no guardan los datos, para acceder a ellos se ejecuta la consulta que genera la vista. Esto depende de cada DBMS y de las configuraciones
    * Vista materializada: si contiene datos, se usa en entornos distribuidos
* Se pueden hacer restricciones de integridad sobre las vistas con `INSTEAD OF`
* Se usa el comando `CREATE VIEW`
* Por ejemplo:

~~~sql
CREATE VIEW VentasParis (codpro,codpie,codpj,cantidad,fecha) AS
    SELECT codpro,codpie,codpj,cantidad,fecha
    FROM ventas
    WHERE (codpro,codpie,codpj) IN
        (SELECT codpro,codpie,codpj
        FROM proveedor,pieza,proyecto
        WHERE proveedor.ciudad='Paris' AND
        pieza.ciudad='Paris' AND
        proyecto.ciudad='Paris');
~~~

* Se puede consultar el catálogo: `all_views(owner,view_name,text)`
    * `owner`: el propietario de la vista
    * `view_name`: nombre que se le ha dado a la vista
    * `text`: comando que genera la vista
* Una vez creada la vista, se puede hacer consultas sobre la vista como si se tratara de una tabla
* Solo puede hacer consultas el usuario autorizado: añade un nivel de seguridad

## Actualización de una vista

* Las vistas no son tablas reales: dan problemas a la hora de insertar datos o modificar su valor
* Los comandos `DELETE, INSERT, UPDATE` están restringidos a casos concretos
    * La definición de la vista no puede incluir cláusulas de agrupamiento (`GROUP BY`) o funciones de agregación (`MAX, AVG, COUNT`)
    * La definición de la vista no puede incluir la cláusula `DISTINCT`
    * La definición de la vista no puede incluir operaciones sobre conjuntos ni reuniones. Debe construirse sobre una única tabla base
    * Todos los atributos que deban tomar siempre valor `NOT NULL` y `PRIMARY KEY` han de estar en la definición de la vista
* Usando `INSTEAD OF` se pueden resolver ambigüedades, relajando estas restricciones para usar las operaciones de modificación
* Cuando se añade la cláusula `WITH CHECK OPTION` al final de la definición de la vista, cada vez que se inserta una nueva tupla la base de datos comprueba si esta satisface la consulta generadora de la vista

## Eliminación de vistas

* `DROP VIEW nombre_vista`
* Si queremos cambiar una vista, es mejor usar `CREATE OR REPLACE VIEW nombre_vista AS comando_select`

--------------------------------------------------------------------------------

# Catálogo

## Algunas vistas

* `USER TABLES`: muestra información de las tablas de las que el usuario es propietario
    * `TABLE_NAME`: nombre de la tabla
    * `TABLESPACE_NAME`: espacio de tablas donde se encuentra almacenada la tabla
    * `NUM_ROWS`: numero de tuplas de la tabla
    * `AVG_SPACE`: espacio que ocupa la tabla

--------------------------------------------------------------------------------

# Gestión de privilegios

## Privilegios del sistema

* Tipos de privilegios del sistema:
    * Privilegios que autorizan operaciones de alto nivel en el sistema: `CREATE SESSION` y `CREATE TABLESPACE`
    * Privilegios que autorizan la gestión de objetos en el propio esquema de usuario `CREATE TABLE`
    * Privilegios que autorizan la gestión de objetos en cualquier esquema `CREATE ANY TABLE`
* Comando de concesión de privilegios:
    * `PUBLIC`: referencia a  todos los usuarios
    * `WITH ADMIN OPTION`: el usuario puede dar privilegios a otros usuarios

~~~sql
GRANT {system_priv | role} [,{system_priv | role}] ...
TO {user | role | PUBLIC} [,{user | role | PUBLIC}] ...
[WITH ADMIN OPTION]
~~~

* Comando de denegación de privilegios:
    * Sólo permite derogar privilegios que hayan sido explícitamente concedidos mediante `GRANT`
    * Hay que vigilar los efectos sobre otros privilegios al derogar uno dado.
    * No hay efecto de cascada

~~~sql
REVOKE {system_priv | role} [,{system_priv | role}] ... 
FROM {user | role | PUBLIC} [,{user | role | PUBLIC}]...
~~~

* Comando de concesión de privilegios sobre objetos
    * La lista de columnas sólo tiene sentido cuando se refieren a privilegios `INSERT`, `REFERENCES` o `UPDATE`
    * `ALL` se refiere a todos los privilegios que han sido concedidos sobre el objeto con `WITH GRANT OPTION`
    * `WITH GRANT OPTION` autoriza al usuario para conceder a su vez el privilegio. No se puede usar al conceder privilegios a role

~~~sql
GRANT {object_priv [(column_list)] [,object_priv [(column_list)]] ... | ALL [PRIVILEGES]
ON [schema.]object
TO {user | role | PUBLIC} [,{user | role | PUBLIC}]... [WITH GRANT OPTION]
~~~

* Comando de denegación de privilegios sobre objetos:
    * `CASCADE CONSTRAINTS` propaga la derogación hacia restricciones de integridad referencial relacionadas con el privilegio `REFERENCES` o `ALL`
    * Un usuario sólo puede derogar aquellos privilegios que él mismo haya concedido mediante `GRANT` a otro
    * Siempre se hace derogación en cascada con respecto a la concesión con la opción `WITH GRANT OPTION`

~~~sql
REVOKE {object_priv [(column_list)] [,object_priv [(column_list)]]... | ALL [PRIVILEGES]} 
ON [schema.]object FROM {user | role | PUBLIC} [,{user | role | PUBLIC}]... [CASCADE CONSTRAINTS]
~~~


--------------------------------------------------------------------------------

# Nivel Interno

## Índices

* Estructura adicional para acelerar las consultas
* El SGBD usa un índice para incrementar el rendimiento cuando:
    * Busca registros con valores específicos en columnas indexadas
    * Accede a una tabla en el orden de las columnas del índice
* Ralentizan actualizaciones sobre la tabla y ocupan memoria en disco
* Una vez creado un índice, el SGBD actualiza, mantiene y usa automáticamente el índice
* *Oracle* crea automáticamente un índice sobre la clave primaria --> no crear un índice sobre la clave primaria, empeoramos el rendimiento de la base de datos
* Es más eficiente crear una tabla e insertar el groso de los datos para después crear el índice, que crear la tabla e inmediatamente el índice
* El orden de los índices afectan a la eficiencia de las consultas que no involucren todos los campos del índice
* Se suele usar la estructura de árbol *B-estrella* (*B* viene de balanceado y no de binario) 

### Creación de un índice normal

~~~sql
CREATE INDEX nombre_del_indice
ON tabla(campo [ASC | DESC],...)
~~~

### Creación de índice invertido

* Mejora los accesos a claves consecutivas, porque evita contención de bloques
* Empeora las búsquedas en rangos de la clave

~~~sql
CREATE INDEX nombre_del_indice 
ON tabla(campo [ASC | DESC],...) REVERSE;
~~~

### Creación de índice BITMAP

* El rango del campo es muy bajo
* Para consultas de tipo `OR`, las de tipo `AND` sobre el mismo campo no tienen sentido

~~~sql
CREATE BITMAP INDEX nombre_indice ON Tabla(campo1, ...);
~~~

### Tablas organizadas por índice: IOT

* Se organizan las tuplas según el valor de la clave, con un árbol B-estrella
* Se declara en la creación de la tabla, no se crea un índice aparte
* Se diferencia de un índice corriente en que las hojas están en las tuplas, no en los RID que apuntan a las tuplas

~~~sql
CREATE TABLE tabla (
    columna1
    columna2
    ...
) 
ORGANIZATION INDEX
~~~

### Borrado de un índice

* Cuando se borra una tabla, automáticamente se borran todos los índices de la tabla
* Para borrar el índice sobre la clave primaria de una tabla, hay que eliminar primero la restricción de clave primaria

~~~sql
DROP INDEX nombre_del_indice;
~~~

## Cluster

* Conjunto de tablas que se organizan en los mismos bloques de datos
* Las tablas comparten campos comunes, que son la clave del cluster
* Útil cuando se accede frecuentemente a los campos comunes de forma conjunta
* Las tablas deben ser muy consultadas y poco alteradas para que tenga sentido
* Beneficios:
    * Se reducen los accesos a disco
    * Se mejora la eficiencia en consultas que impliquen reuniones sobre las tablas involucradas
    * La clave del cluster se almacena solo una vez --> mayor eficiencia en el almacenamiento
* Primero se crea el cluster, y después tablas que pertenecen al cluster
* Antes de introducir tuplas en el cluster, se debe crear un índice para acceder al cluster sobre la clave primaria
    * El SGBD hace esto automáticamente para tablas. Para cluster, el procedimiento es manual

### Creación de cluster indizado

~~~sql
/*Se crea el cluster*/
CREATE CLUSTER nombre_del_cluster(campo tipo_de_dato);

/*Se crea una tabla que pertenece al cluster*/
CREATE TABLE nombre_tabla(
    campo tipo_dato,
    campo tipo_dato,
    ...
    CLUSTER nombre_del_cluster(clave_cluster)
);

/*Se crea el indice sobre el cluster*/
CREATE INDEX indice_cluster ON CLUSTER nombre_cluster;
~~~

### Creación de cluster hash

~~~sql
CREATE CLUSTER nombre_del_cluster(campo tipo_de_dato) [HASH IS campo]
SIZE <tamaño> HASKEYS <cant_valores_clave>;
~~~

* La cláusula `HASH IS` se puede usar solamente si la clave del cluster está compuesta por un único atributo de tipo `NUMBER` y contiene enteros uniformemente distribuidos. 
    * Si esto no ocurre, se usa la función hash por defecto
* La cláusla `SIZE` establece cuanto espacio van a ocupar todas las tuplas que tengan un mismo valor de la clave cluster
    * Este valor debe estimarse
    * Si el valor es muy pequeño: muchas colisiones
    * Valor muy grande: mejor rendimiento a costa de gasto innecesario de espacio
    * Añadir un 15% al valor esperado
* El parámetro `HASHKEYS` establece cuantos valores distintos va a tomar la clave del cluster
    * Sirve para determinar el número primo que se va a usar en la función hash

### Creación de cluster hash de una sola tabla

* Mismo proceso que para un cluster hash pero usando la cláusula `SINGLE TABLE`

~~~sql
CREATE CLUSTER nombre_cluster(campo tipo_dato)
SIZE size SINGLE TABLE HASHKEYS num;
~~~

### Eliminación de un cluster

* Si un cluster incluye tablas, no puede ser eliminado antes de que se eliminen las tablas o se incluya la cláusula `INCLUDING TABLES`
* `CASCADE CONSTRAINTS` permite eliminar las referencias externas que haya hacia el cluster
* Se elimina automáticamente el índice sobre el cluster

~~~sql
DROP CLUSTER nombre_del_cluster
[INCLUDING TABLES [CASCADE CONSTRAINTS]];
~~~

--------------------------------------------------------------------------------

# Comandos exclusivos SQLPlus

* `run <path_to_file>`: ejecuta un script guardado en un archivo
* `DESCRIBE <nombre_tabla>`: muestra el esquema de una tabla
* Para hacer `echo`: `SELECT '<comentario>' AS '';`

--------------------------------------------------------------------------------

# Comandos exclusivos de SQLite

* `.schema <tabla>`: muestra qué código ha creado la tabla
* `PRAGMA table_info(<tabla>)`: muestra el esquema de la tabla (equivalente al `DESCRIBE` de `MySQL`)
    * `.header on`: para que muestre los nombres de los campos
    * `.mode column`: para que lo muestre en columnas
* `.tables`: muestra las tablas creadas de la base de datos
* `.read <file>`: ejecuta un script
* El `SELECT1 MINUS SELECT2` en `SQLite` es `SELECT1 EXCEPT SELECT2`
* Para hacer `echo`: `SELECT '<comentario>' AS '';`

--------------------------------------------------------------------------------

# Dudas

* Ejercicio 2.4 apartado b
* Operaciones sobre fechas propias de `Oracle`
