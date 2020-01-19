#### 6. Se tiene una BD sobre la información de un congreso con un esquema formado por las siguientes tablas:


INSCRIPCIONES(__IN_S#__,NOMBRE,PROCEDENCIA) 
SESIONES(__SES#__,TITULO,COORD#) 
ARTICULOS(__ART #__,TITULO) 
PROGRAMA(__SES#, ART#__,FECHA,HORA_INICIO,HORA_FIN,SALA,PONENTE#)


Teniendo en cuenta que tanto los coordinadores como los ponentes han de estar inscritos en el congreso, resolver las siguientes consultas mediante Álgebra Relacional y mediante Cálculo Relacional Orientado a Tuplas:   

#### a) Mostrar los nombres de los ponentes que coordinan su propia sesión.

##### Álgebra  

$$\pi_{ponente}( Programa\bowtie_{p.ses=s.ses \wedge s.coord = p.ponente} Sesiones )$$


##### Cálculo  

$$\{p.ponente | Ponente(p) \wedge \exists s \wedge Sesiones(s) \wedge p.ses=s.ses \wedge s.coord = p.ponente \}$$

##### Sql  

```Sql
SELECT p.ponente from Programa p JOIN Sesion s ON 
     p.ses=s.ses and s.coord = p.ponente
```

#### b) Seleccionar los coordinadores que coordinan una única sesión. 

##### Álgebra
$$\pi_{coord}(sesiones) - \pi_{coord}(sesiones \bowtie _{s1.ses \neq s2.ses \wedge s1.coord = s2.coord} sesiones) $$

##### Cálculo 
$$\{ p.coord | Sesion(p) \wedge  \neg   \exists s \wedge Sesion(s) \wedge p.ses \neq s.ses \wedge p.coord = s.coord \}$$

##### Sql  
Estilo álgebra relacional 
```sql
SELECT coord from Sesion
MINUS
SELECT p.coord from Sesion p JOIN Sesion s ON 
    p.ses <> s.ses  and p.coord = s.coord
```

Estilo cálculo   
```sql
SELECT p.coord from sesion p WHERE 
    NOT EXISTS ( SELECT s.coord FROM sesion s where 
        s.coord = p.coord and s.ses <> p.ses)
```
¿Cuál sería más eficiente?



#### c) Mostrar el tı́tulo de los artı́culos que se exponen en primer y último lugar.

**Nota interpretación**: 
- ¿Exponerse significa empezar,terminar o empezar para la hora de inicio y acabar para el último lugar?
Supongo la última opción. 

##### Álgebra    

- **Programas candidatos**: 
$espacio = \pi_{ses, art}programa$    


- **Saco el primero artículo** : 
$pp = programa \bowtie_{(p1.fecha <p2.fecha) \vee (p1.fecha =  p2.fecha  \wedge p1.hora\_inicio < p2.hora\_inicio )} programa$  
Selecciono p2, que es son los programas para los que existe otro antes
$primero= \pi_{art} (espacio - \sigma_{p2.ses, p2.art} (pp))$    

- **Saco el último artículo** (solo cambia la hora): 
$pp2 = programa \bowtie_{(p1.fecha <p2.fecha) \vee (p1.fecha =  p2.fecha  \wedge p1.hora\_fin < p2.hora\_fin )} programa$  
Selecciono p1, que es son los programas para los que existe otro después
$último= \pi_{art} (espacio - \sigma_{p1.ses, p1.art} (pp2))$  
- **Asociamos artículos con su título**
$$títulos = \pi_{título}( articulo \bowtie(primero \cup último) )$$

