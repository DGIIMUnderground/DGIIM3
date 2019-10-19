# Cómo utilizar máxima para resolver ecuaciones diferenciales  

Esta información está basada en: 
http://maxima.sourceforge.net/docs/manual/es/maxima_21.html 


Declación de una ecuación de segundo orden


```
(%i5) 'diff(x,t)=x^2+x;
                                  dx    2
(%o5)                             -- = x  + x
                                  dt
								     ``` 


para resolver la ecuación 
```
 ode2(%i5,x,t);
(%o7)                    log(x) - log(x + 1) = t + %c
```

si sabemos que una solución de nuesta ecuación es  `x(t=1)=1`
podemos calcular la ecuación particular

```
(%i12) ic1(%o10,t=1,x=1);
(%o12)               log(x) - log(x + 1) = t - log(2) - 1
(%i13) 

```

El comando `ratsimp(%o12)` simplificaría la ecuación
`solve( %o12, x);` despejaría la x 

`bc2(%o12, x=1,y=1);` también valdría como ic1, además valdría para ecuaciones de segundo orden.

Podemos pintar un campo vectoria con la siguiente instrucción: 
` plotdf( x^2+1, [trajectory_at, 0,0]);` trajectory_at te pinta la solución que pase por el punto (0,0)

Para pintar una ecuacion, que hayamos quitado la constante

```
(%o23)                            atan(x) = y
 x1:map('rhs,solve(%o23,x));
(%o24)                             [tan(y)]
(%i25) plot2d( x1,[y,0,1]);
```
