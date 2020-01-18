-- EJERCICIOS RELACIÓN CÁLCULO Y ÁLGEBRA RELACIONA 
select table_name from user_tables; 
describe VENTAS;
DESCRIBE opc.VENTAS;

-- 1A
(SELECT  distinct codpro, codpj from opc.VENTAS) order by (codpj) ; -- mostramos todos, debería de salir S1,S2,s4,S3

select DISTINCT  codpro from opc.VENTAS spj WHERE spj.codpj like 'J1';

--1C nombre piezas, proveedores y proyectos que se encuentran en la misma ciudad 

-- versión natural join 
select DISTINCT s.nompro, p.nompie, j.nompj from 
OPC.PROVEEDOR s NATURAL JOIN 
( OPC.PIEZA p natural join OPC.PROYECTO j); 

-- version join on 

select distinct s.nompro, p.nompie, j.nompj from 
OPC.PROVEEDOR s join 
    ( OPC.PIEZA p join OPC.PROYECTO j
    on p.ciudad = j.ciudad )
ON p.ciudad = s.ciudad ;


--versión from 
SELECT distinct s.nompro, j.nompj, p.nompie
FROM OPC.proveedor s, OPC.proyecto j, OPC.pieza p
WHERE s.ciudad = j.ciudad and j.ciudad = p.ciudad;

-- versión from más eficiente 
select DiSTINCT subconjunto.nompro, subconjunto.nompj, p.nompie 
from OPC.PIEZA p, 
    (select DISTINCT s.ciudad, s.nompro, j.nompj from OPC.PROVEEDOR s, OPC.PROYECTO j where s.ciudad=j.ciudad) subconjunto
WHERE p.ciudad = subconjunto.ciudad;

-- EJERCICIO D: nombre de las piezas suministradas por proveedores de París

-- versión con selección natural 
SELECT DISTINCT p.nompie
FROM OPC.pieza p NATURAL JOIN (
    OPC.ventas NATURAL JOIN (
        SELECT * 
        FROM OPC.proveedor 
        WHERE ciudad LIKE 'Londres'
    )
); 
-- versión con in (próxima cálculo relacional)
SELECT DISTINCT codpie
FROM ventas where codpro IN
(Select DISTINCT s.codpro FROM proveedor s where s.ciudad LIKE 'Londres');

-- versión con el exist,  
SELECT DISTINCT v.codpie 
    from ventas v where EXISTS (
    select * from proveedor p 
        where ( v.codpro = p.codpro and p.cidudad LIKE 'l¡'
    )
);

-- EJERCICIO 3.24 (CUADERNILLO)
--Encontra los códigos de las pieza suministradas a todos los proyectos loaclizados en Londres  

-- (entendiendo que la pieza tiene que estar en todos los proyectos)
-- versión utilizando divisor (conjunto Minus) 



-- (entendiendo que esa pieza está en algún de los proyectos)
-- versión de cálculo ( con exists)  
SELECT DISTINCT v.codpie from VENTAS v WHERE EXISTS (
    SELECT * FROM proyecto p
        where p.codpj = v.codpj and p.ciudad LIKE 'Londres'
    
)
order by v.codpie DESC;

-- versión por álgebra  

SELECT DISTINCT codpie from VENTAS NATURAL JOIN ( 
    SELECT DISTINCT codpj from proyecto where ciudad LIKE 'Londres'
    )
    ORDER BY codpie ASC;
        



-- EJERCICIO F. piezas suministradas a algún proyecto por un vendedor que se encuentre en la misma ciudad que el proyecto  

SELECT DISTINCT v.codpie 
FROM OPC.ventas v NATURAL JOIN (
    select j.codpj from  OPC.proyecto j NATURAL JOIN OPC.proveedor   
);
        

