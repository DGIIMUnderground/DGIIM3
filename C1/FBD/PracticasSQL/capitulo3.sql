-- ALGUNOS EJERCICIOS RELACIONADOS CON EL  CAPÍTULO 3
-- Con el objetico de enteder mejor la división, el group by y el having
-- Espero que os lo paséis muy bien jugando a SQL <3
-- Enero 2020

-- 3.24 Encontra los códigos de las pieza suministradas a todos los proyectos loaclizados en Londres

-- versión cálculo

SELECT v.codpie
FROM ventas v
WHERE NOT EXISTS (
      SELECT j.codpj
      FROM  proyectos j
      WHERE NOT EXISTS (
      	    SELECT w.codpie
	    FROM ventas w
	    WHERE
		j.cidudad LIKE 'Londres' AND
		w.codpie = v.codpie AND
		j.codpj = w.codpj
	)
);

-- versión mixta cálculo y álgebra

SELECT v.codpie
FROM ventas v
WHERE NOT EXISTS (
      (SELECT DISTINCT codpj FROM proyectos WHERE ciudad LIKE 'Londres')
      MINUS
      (SELECT DISTINCT codpj FROM ventas WHERE codpie = v.codpie)
);


-- 3.25 Proveedores que envían piezas procedents de todas las ciudades donde hay un proyecto

-- creo vista para la relación proveedor ciudades donde recibe 

CREATE VIEW proveedorCiudad (codpro, ciudad) AS
SELECT codpro, ciudad
FROM ventas NATURAL JOIN piezas;

-- versión mixta
   -- idea subyaciente COGER  proveedores CANDIDATOS para los cuales el conjunto de TODAS las ciudades MENOS LAS SUYAS sea VACÍO 

SELECT v.codpro
FROM prooveedorCiudad v
WHERE NOT EXISTS (
      (SELECT DISTINCT ciudades FROM proyectos)
      MINUS
      (SELECT DISTINCT w.ciudades
      FROM proveedorCiudad w
      WHERE w.codpro = v.codpie      
      )
);

-- versión de cálculo
 -- idea subyaciente: no existe CONDICIÓN para la cual NO EXISTE OTRO elemento del candidato que la satifaga

SELECT v.codpro
FROM proveedorCiudad v
WHERE NOT EXISTS (
      SELECT *
      FROM proyectos p
      WHERE NOT EXISTS (
      	    SELECT *
	    FROM proveedorCiudad w
	    WHERE w.codpro = v.codpro AND
	    	  w.ciudad = p.ciudad 
      )
);

-- 3.26 envios con más de 1000 unidades

SELECT count(*) FROM ventas WHERE cantidad > 10000;

-- 3.27 Mostar el máximo peso

SELECT max(peso) FROM ventas;

-- 3.27 Mostrar código de pieza de máximo peso

SELECT codpie
FROM (SELECT codpie,  max(peso) FROM ventas)
;


-- 3.30 Muestra el código de los proveedores que han hecho más de 3 envíos direntes

SELECT codpro
FROM ventas v
WHERE 3 <  SELECT COUNT(*) FROM ventas v where v.codpro = w.codpro;



--Ejercicio 3.31 Mostrar la media de las cantidades vendidas por cada código de pieza junto con su nombre.


SELECT codpro, avg(vantidad) FROM VENTAS GROUP BY ( codpro);

--Ejercicio 3.32 Encontrar la cantidad media de ventas de la pieza ’P1’ realizadas por cada proveedor.
SELECT codpro , avg( cantidad) FROM ventas where codpie Like 'P1' GROUP BY codpro;

-- Ejercicio 3.33 Encontrar la cantidad total de cada pieza enviada a cada proyecto.

SELECT codpj, count(*) FROM VENTAS GROUP BY codpj;

-- Ejemplo 3.22 Mostrar la media de unidades vendidas de la pieza ’P1’ realizadas por cada proveedor, indicando solamente la información de aquellos proveedores que han hecho entre 2 y 10 ventas.

SELECT codpro, codpie,  AVG(cantidad)
FROM ventas
WHERE codpi LIKE 'P1'
GROUP BY (codpro, codpie)
HAVING count(*)  BETWEEN 2 and 10

-- Ejemplo 3.23 Encuentra los nombres de proyectos y cantidad media de piezas que recibe por proveedor

SELECT v.codpro, j.nompj, AVG(v.cantidad)
FROM ventas v, proyecto j
WHERE v.codpj = j.codpj
GROUP BY(v.codpro, j.proyecto)

-- Ejercicio 3.35 Mostrar los nombres de proveedores tales que el total de sus ventas superen la cantidad de 1000 unidades.

SELECT s.nompro
FROM ventas v, proveedores s
WHERE v.codpro = s.codpro
GROUP BY (v.codpro)
HAVING sum(v.cantidad) > 1000;


-- Ejemplo 3.24 Mostrar el proveedor que más ha vendido en total.

SELECT codpro, sum(cantidad)
FROM ventas
GROUP BY codpro
HAVING  count(cantidad) = (
	SELECT  max( sum(v.cantidad))
	FROM ventas v
	GROUP BY v.codpro
	); 

-- Ejercicio 3.36 Mostrar la pieza que más se ha vendido en total.


SELECT codpie
FROM ventas
GROUP BY codpie
HAVING sum(cantidad) = (
       SELECT max(sum(v.cantidad))
       FROM ventas v
       GROUP BY v.codpie
);


-- Otros ejercicios
-- apoyados en advy99
-- enlace al repo: ttps://github.com/advy99/FBD/blob/master/Practicas/agregacion_15_05_2019.sql
-- contar los tipos de piezas diferentes que ha vendido S1

SELECT count( DISTINCT codpi )
FROM ventas
WHERE codpro = 'S1'

-- Pedido con mayor númer de unidades

SELECT codpie, codpro, codpj, cantidad
FROM ventas
WHERE cantidad = (SELECT max(cantidad) FROM ventas);

-- Número pedidos para cada proveedor

SELECT codpro, count(*)
FROM ventas
GROUP BY codpro

-- Número pedidos para cada proveedor cuyo status sea mayor que 2
-- REVISAR ESTA FORMA 
SELECT p.codpro, count(*)
FROM ventas v, proveedor s
WHERE v.codpro = s.codpro
GROUP BY codpro
HAVING s.status > 2


       -- v2
SELECT codpro, count(*)
FROM ventas
WHERE codpro IN
      (SELECT codpro
      FROM proveedor 
      WHERE status > 2
      ); 

-- Número de piezas P1 que se han vendido, que sean de  madrid

-- v1
SELECT count( v.cantidad)
FROM ventas v, pieza p
WHERE v.codpie = p.codpie and codpie = 'P1' and p.ciudad LIKE 'Madrid'
GROUP BY codpie

-- v2

SELECT count(cantidad)
FROM ventas
WHERE codpie IN
      (SELECT codpie
      FROM pieza
      WHERE ciudad LIKE 'Madrid' and
      	    codpie = 'P1'
)

-- Número de piezas de Madrid ordenadas de mayor cantidad a menos

SELECT codpie , sum (cantidad)
FROM ventas
WHERE codpie IN
      ( SELECT codpie
      FROM pieza
      WHERE ciudad = 'Madrid'
      )
GROUP BY codpie
ORDER BY sum(cantidad), codpie ASC


-- Piezas que se hayan vendido más o igual que la P1

SELECT codpie, sum (cantidad)
FROM ventas
GROUP BY codpie
HAVING sum(cantidad) >= (
       SELECT sum(v.cantidad)
       FROM ventas v
       WHERE v.codpie = 'P1' ); 
      -- GROUP BY v.codpie) innecesario


-- Total de piezas vendidas por año desde el 2016 y que el pedido supera las 100 unidades

SELECT to_char(fecha, 'yyyy'), sum(cantidad)
FROM ventas
GROUP BY to_char(fecha, 'yyyy')
HAVING (cantidad > 100) and (to_char(fecha, 'yyyy')>'2016'; 


-- Encontrar al proveedor que más piezas a vendido
--- v1
SELECT codpro
FROM venta
GROUP BY codpro
HAVING sum(cantidad) =
       (SELECT max(sum(v.cantidad))
       FROM venta
       GROUP BY v.codpro
       );

--- v2

SELECT codpro
FROM venta
GROUP BY codpro
HAVING sum(cantidad) <= ALL
       (SELECT sum(cantidad)
       FROM ventas
       GROUP BY codpro); 

---------------------------------


-- EJERCICIOS ADICIONELES

-- Ejercicio 3.44 Mostrar los proveedores que venden piezas a todas las ciudades de los proyectos a los que suministra ’S3’, sin incluirlo.

SELECT a.codpro
FROM venta a
WHERE a.codpro <> 'S3' and
      NOT EXISTS (
         (SELECT DISTINCT  p.ciudad FROM venta v, proyecto p
      	      WHERE v.codpj =p.codpj and v.codpro = 'J1')
          MINUS
          (SELECT DISTINCT  p2.ciudad FROM venta v2, proyecto p2
       	       WHERE v2.codpj = p2.codpj and v.codpro = a.codpro) 

);



-- Ejercicio 3.58 Coloca el status igual a 1 a aquellos proveedores que sólo suministran la pieza P1.

-- COMPROBAR QUE FUNCIONA

UPDATE  proveedor
SET status = 1
WHERE codpro in (

       SELECT s.codpro
       FROM proveedor s
       WHERE s.codpie = ALL (SELECT p.codpie
       	     	      	    FROM proveedor p
			    WHERE p.codpro = s.codpro) 
); 



-- 3.46
-- No necesita el distinct, le hecha una mano al optimizador pero no sería necesario 
SELECT codpro
FROM proveedor pro
WHERE NOT EXISTS(
      (SELECT DISTINCT codpie FROM ventas WHERE ventas.codpie='S1')
      MINUS
      select DISTINCT codpie FROM ventas where ventas.codpie=pro.codpie)
);


-- 3.47

SELECT codpro, SUM(cantidad)
FROM ventas pro
WHERE NOT EXISTS(
(SELECT DISTINCT codpie FROM ventas WHERE ventas.codpie='S1')
MINUS
(SELECT DISTINCT codpie FROM ventas WHERE ventas.codpro=pro.codpie)
)
GROUP BY(codpro); 


-- 3.48
SELECT *
FROM proyecto
WHERE not exists(
      select v.codpro from ventas v where v.codpie='P3'
      MINUS 
      select v1.codpro from ventas v1 where v1.codpj=proyecto.codpj
);


-- 3.49
-- MIRAR EL DISTINCT, EL IN TE LO AHORRA
-- v1 con subconsulta 
SELECT avg(cantidad)
FROM  (select distinct v.codpro from ventas v  WHERE  v.codpie='P3') p, ventas v1
      where p.codpro=v1.codpro;

-- v2 sin subconsulta (aquí hay el mismo problema de arriba no poner el disting
SELECT avg(cantidad)
WHERE v1.codpie='P3' and v1.codpro


-- 3.50

select index_name, table_name, table_owner from user_indexes; 

-- 3.51

describe ventas; -- la idea es sacar esta información

select column_name NOMBRE, nullable nulo,  data_type Tipo
from user_tab_columns
where table_name='VENTAS';

-- 3.52

select codpro, avg(cantidad), to_char(fecha, 'yyyy')
from ventas
group by codpro, to_char(fecha, 'yyyy')
order by codpro, to_char(fecha, 'yyyy')
