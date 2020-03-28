---
title: Práctica 2 - Ingeniería de Servidores - Parte 3
author: Miguel Ángel
---

--------------------------------------------------------------------------------

# Lección 2 - Firewall y pila LAMP

### Apache Server

###### Instalar apache server

```
yum install httpd
```

Está cargado pero no activo (`systemctl status httpd`).

Activamos el servicio, crea enlace simbólico para que ejecute servicio cada vez que arranca la máquina: `systemctl enable httpd`.

Ejecutamos httpd: `systemctl start httpd`. Engendra varios procesos con varias hebras cada uno (pero no tiene muy claro el profesor cómo va). Se puede controlar cuántos procesos se lanzan, cuántas peticiones puede obtener, límite de peticiones antes de acabar el proceso.

**Nginx** se utiliza como proxy inverso, línea de entrada a cloud. Potente ejecutando reglas de reescritura. Es muy bueno accediendo a dispositivos (paralelismo basado en colas similar a Javascript: no lanza hebras sino que usa una cola y E/S asíncrona de SO).

###### Configuración de Apache

```
cd /etc/httpd
```

La configuración está en `httpd.conf`. Pero está modularizado en `conf.d`.

###### Probar que Apache funciona

La forma más bruta sería usar un `telnet`: `telnet localhost 80`. Usamos una herramienta más común.

Usando `curl`, nos devuelve resultado HTML (con `-vv` nos da las llamadas del protocolo y la web). Esto nos dice que apache está funcionando.

```
curl http://192.168.56.20
```

Desde dentro si funciona, pero no desde fuera: quiere decir que el firewall está activado.

```
firewall-cmd --state
```

Vemos todas las reglas:

```
firewall-cmd --list-all
```

Podemos definir grupos de reglas del firewall: **zonas de firewall**. La zona por defecto es la pública.

Los distintos servicios están en `/etc/services`.

###### Abrimos puerto 80

```
firewall-cmd --zone=public --add-service=http (o poner puerto)
```

Para que la regla se guarde de forma permanente añadimos la opción `--permanent`. Esto nos lo escribe en el archivo de configuración de firewall.

Tenemos que decirle que recargue para tener estos cambios:

```
firewall-cmd --reload
```

### PHP

```
yum search php
```

```
yum install php
```

Este es el modo largo: `<?php ... >`.

El archivo de configuración de PHP es:

```
less /etc/php.ini
```

Ver `short_open_tag` que hace las descripciones de errores largos, porque por defecto está `Off`.

Todos los logs están en `/var/log`.

La raíz de documentos (`DOC_ROOT` o `DOCUMENT_ROOT`) está en `/var/www/html`. Aquí ponemos los ficheros de nuestras páginas web. Crearemos una página web (ver al final `index.html`). Por defecto, Apache busca `index.html`.

Creamos un archivo con código PHP, lo llamamos `index.php` (ver al final `index.php`). Colocando `<?php phpinfo(); ?>` no me lo interpreta: tengo PHP instalado pero no activo para Apache.

Hacemos `apachectl restart` porque no teníamos el intérprete de PHP en memoria.

### MariaDB

```
yum install mariadb-server
```

Todos los servicios configurados en CentOS:

```
systemctl list-unit-files
```

Habilitamos y empezamos servicio `mariadb` con `systemctl`.

Entramos en la tabla `mysql` de mariadb que contiene metainformación. Si funciona, funciona MariaDB.

```
show databases;
show tables; // ta vacía
```

Creamos datos mySQL:

```
create database ise2020;
grant all privileges on ise2020.* to 'alumno'@'localhost' identified by 'ise2020';
```

Con `tail -f` se actualiza un archivo (perfecto para ver logs).

Si ejecutamos el código PHP con

```php
$mysql = new mysqli('localhost', 'usuario', 'secreto','ise2020');
if ($mysql->connect_error) {
	die('No pudo conectarse: ' . $mysql->connected_error);
}
echo '<h1>Conectado satisfactoriamente</h1>';
```

En el log vemos `class 'mysqli' not found`. Vemos que no está `mysql` en los datos de la configuración del entorno PHP. Necesitamos un código intermedio entre el código que actúa como cliente y la BD.

###### Instalar driver MySQL para PHP

```
yum search php | grep -i mysql
```

Instalaremos `php-mysql`. Recargamos apache porque el intérprete PHP está corriendo en el espacio de memoria de Apache. Si corriésemos algo fuera de Apache, accedería a MySQL sin problema.

Queda crear una tabla `alumnos` en la base de datos `ise2020` e insertar datos.



#### Cosas que ha dicho por encima

Ver Hazelcast Vault para almacenamiento de secretos (almacenar contraseñas).

Repetir proceso en Ubuntu.



### Archivos asociados a la práctica

###### `index.html`

~~~html
<html>
    <head>
        <title>Página de prueba</title>
    </head>
    <body>
        <h1>¡Hola, esto es una página de prueba!</h1>
    </body>
</html>
~~~

###### `index.php`

~~~html
<html>
    <head>
        <title>Página de prueba</title>
    </head>
    <body>
        <h1>Página de prueba</h1>
        <h2>Conexión MySQL</h2>
        <!-- Esto es para cuando hayamos configurado MySQL y hayamos creado un usuario en la BD -->
        <?php
			$mysql = new mysqli('localhost', 'alumno', 'ise2020', 'ise2020');
			if ( $mysql->connect_error ) {
				die('No pudo conectarse: ' . $mysql->connected_error);
			}
			echo '<p>Conectado satisfactoriamente</p>';
		?>
        <h2>Configuración PHP</h2>
        <?php
			phpinfo();
		?>
    </body>
</html>
~~~
