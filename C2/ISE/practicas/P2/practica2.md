### Lección 1 - ssh

#### Pequeña explicación sobre como funciona ssh

---

#### Instalación de SSH en Ubuntu

- Crear máquinas virtuales CentOS y Ubuntu con la configuración estándar.
- Hay que volver a configurar la tarjeta de red (adaptador 2) como Host only (igual que en la práctica anterior).
- Instalar servidor ssh (en CentOS viene por defecto):
  - `apt-cache search sshd`
  - `sudo apt-get install openssh-server`
- Con `systemctl status ssh` podemos ver el estado del servicio y con `systemctl list-unit-files` podemos ver todos los servicios que hay.

#### Conexión mediante SSH a Ubuntu

- Necesitamos la IP de la máquina de Ubuntu. Para conseguirla, necesitamos configurar la red primero (como en la práctica 1). Una vez configurada, si ejecutamos `ifconfig -a` deberíamos ver nuestra IP.
- Para conectarnos usamos `ssh <usuario>@ip` desde la máquina anfitriona (nuestro ordenador).

#### Contenido carpeta .ssh (se encuentra en el cliente)

- `~/.ssh/id_rsa`: tu clave privada
- `~/.ssh/id_rsa.pub`: tu clave pública
- `~/.ssh/authorized_keys`: contiene una lista de claves públicas autorizadas para servidores. Cuando el cliente se conecta al servidor, esta autentica al cliente comprobando la firma de su clave pública guardada en este archivo.
- `~/.ssh/known_hosts`: contiene claves DSA públicas de los servidores SSH accedidos por el usuario. Este archivo es muy importante para asegurar que el cliente SSH se está conectando con el servidor SSH correcto.

#### Contenido carpeta /etc/ssh (en el servidor)

- `/etc/ssh/sshd_config`: archivo de configuración para el daemon de ssh. Campos que nos interesan:
  - `Port`: puerto en el que va a escuchar el daemon.
  - `PermitRootLogin`: para ver permitir acceso root o no, CentOS lo trae por defecto. 

#### Modificando la configuración del cliente

- Ejecutamos `vi /etc/ssh/sshd_config` y cambiamos el campo `Port 22` a `Port 22322`.
- Reiniciamos el servicio con `systemctl restart ssh`.
- Al hacer `ssh <usuario>ip` debería aparecernos conection refused.
- Nos conectamos desde el cliente con `ssh -p 22322 <usuario>ip`.
- Cambiamos el campo `PermitRootLogin prohibit-password` a `PermitRootLogin yes` para permitir el login al usuario root (normalmente esto se deshabilita).
- Para conectarnos sin usar la contraseña usamos: `ssh-copy-id <usuario>@ip`.
- Si copiamos el archivo `authorize_keys` en `/root` (que es el directorio home del usuario root) podremos acceder al usuario root mediante ssh.
- Cambiamos el campo `PermitRootLogin` a `PermitRootLogin no` para deshabilitar el login al usuario root (el profesor lo pedirá así).

#### Conectarnos mediante SSH sin contraseña

Nos vamos a la máquina desde la que nos queramos conectar, por ejemplo Ubuntu, y generamos nuestras llaves con `ssh-keygen` (le damos todo a enter) y luego ejecutamos `ssh-copy-id <usuariocentos>@ipcentos` y ya estaría. Ahora al hacer `ssh <usuariocentos>@ipcentos` se conectará directamente sin pedir contraseña.

##### Añadir contraseña SSH  al agente SSH
Cada vez que nos queramos queramos conectar a un servidor remoto se nos pedirá la contraseña, para evitarlo podemos dejar corriendo el agente ssh. Pasos a seguir:
-  ```eval `ssh-agent` ```  para activar el agente en background
- `ssh-add`  Nos pedirá la contraseña y después de esto ninguna vez más :D (durante esta sesión)

##### Crear alias para la conexión ssh

Para evitar tener que estar introduciendo siempre la ip y el usuario, bastará con añadir un alias al archivo de configuración de ssh

- Editamos el fichero `~\.ssh
Supongamos que para acceder a nuestro servidor ejecutábamos `ssh usuario@192.168.56.103`
Añadiendo las siguientes líneas

```
Host centos
     User usuario
     HostName 192.168.56.103
```

(Por supuesto también podriamos indicar otro parámetros como el puerto)

Ahora para acceder solo tendriamos que escribir ssh `centos`

Fuentes: https://www.howtogeek.com/75007/stupid-geek-tricks-use-your-ssh-config-file-to-create-aliases-for-hosts/


#### Configuración del firewall

##### Ubuntu

`ufw` es el encargado de gestionar el firewall en Ubuntu. Con `ufw status` podemos ver si está habilitado a deshabilitado y con `ufw enable` y `ufw disable` lo activamos o desactivamos. Esos cambios solo se guardan localmente por lo que cuando reiniciemos la máquina se perderán. Para hacerlos de forma fija usamos `systemctl enable ufw` y `systemctl status ufw`.

Si ahora nos intentamos conectar a la máquina usando ssh, veremos que no nos dejará. Tenemos que activar `OpenSSH` en el firewall: `ufw allow OpenSSH`.

Si borramos las reglas anteriores (con `ufw deny OpenSSH` o `ufw delete 1`) y hacemos `ufw allow 22/tcp` también nos dejará conectarnos.

##### CentOS

Si por comodidad, en lugar de usar ips, puedes crear un alias de la máquina usando `sudo hostname <alias>`.

Para poder hacerte root en CentOS necesitas modificar el archivo `/etc/sudoers` mediante el comando `usermod -G wheel <usuario>`.

En CentOS para configurar el firewall se usa el comando `firewall-cmd`. Para ver el estado del servicio usamos `systemctl status firewalld`. Los archivos de configuración se encuentran en `/etc/firewalld`.

- `firewall-cmd --remove service=ssh --permanent`
- `firewall-cmd --add-service=ssh --permanent`
- `firewall-cmd --add-port=22/tcp --permanent`
- En lugar de `--permanent` podemos usar `--runtime-to-permanent`.  
- `firewall-cmd --reload` para hacer ejectivos los cambios 

### Lección 2 - Backups y Sistemas de Control de Versiones

#### dd

Sistema de copias a nivel de dispositivos.

Esta utilidad lo copia todo en raw por lo que nos saltamos el buffer del sistema de ficheros (algunas cosas podrían perderse).

- `dd if=/dev/sd1 of=./sda1.raw bs=1024k`
- `dd if=/dev/zero of=./zeros.dat bs=1k count=5`
Estos comandos tienen como problema que si hay un error, el comando fallará, con esot lo podemos evista
`dd if=/dev/sda of=/dev/sdb conv=noerror,sync`  

Fuentes:
-  https://www.thegeekstuff.com/2010/10/dd-command-examples/
- https://docs.oracle.com/cd/E19683-01/816-5042/chap7rt-57/index.html (este es para la entrada y salida sincronizada, qué significa)

#### cpio

Sistema de copias a nivel de sistemas de ficheros.

Es un formato estándar, se usa para meter estructuras de directorios complejas en un solo archivo. (similar al comando tar). 

Los comandos básicos son: 
```
SYNOPSIS
PARA DESCOMPRIMIR
     cpio -i [options] [pattern ...] [< archive]
 PARA COMPRIMIR 
     cpio -o [options] < name-list [> archive]  
 PASS-THROUGH (copia de un directorio  a otro la lista de nombres a copiar) 
     cpio -p [options] dest-dir < name-list

```

Donde las opciones básicas son: 

- i input  
- o output  
- p pass-through  
- l crea link en la copia 
- v verbose copia el nombre del fichero que va copiando ( en la salids stderr es decir la terminal) 
- u unconditional: si no se pone, el archivo que ya existia no se copia, y si se ha borrado no se borra en el directorio a copiar  
- d  crea directorios si es necesario 



- `find /etc/ -iname '*.conf' | cpio -ov > ./etc/Conf.cpio`
- `cpio -iduv < ./etc/Conf.cpio` OJO TE LO GUARDA EN CARPETAS QUE ESTABAN INDICADAS, es decir, en este ejemplo otra vez todo para la carpeta etc xD 
- `cpio -iduv '/etc/fuse.conf' < ./etc/Conf.cpio`
- ` find /etc -iname '*conf' | cpio -pvd ~ ` Copiaría en home  una carpeta llamada etc con los archivos y estructura de directorios con los archivos de configuración.

Fuentes: man 

#### tar
<
Sistemas de copias a nivel de archivos. 

Ejemplo de sintaxis vamos a usar el estilo tradicional de sintaxis (estilo unix con -a, el estilo GNU es el verboso): 
```
Traditional usage
       tar {A|c|d|r|t|u|x}[GnSkUWOmpsMBiajJzZhPlRvwo] [ARG...]
```	   

Opciones básicas:   
- f nombre del archivo al que se quiere comprimir 
- z usar gzip   
- c create nuevo archivo a crear 
- x extrat para descomprimir 
- u update, append los ficheros que son más nuevos 
- O --to-stdout: estra los ficheros a la salida estandar 
- `tar czf /tmp/etc.tgz`
- `tar xzf /tmp/etc.tgz`


Fuentes: 
- man 
- https://help.ubuntu.com/community/BackupYourSystem/TAR  

#### rsync
Rsync es una herramienta ampliamente utilizada para realizar copias de seguridad y mantener copias exactas de estructuras de ficheros (sincronizadas). En las prácticas nos centraremos en esta segunda funcionalidad. El alumno debe ser capaz de sincronizar una estructura compleja de directorios y subdirectorios dentro del mismo servidor  y a un servidor remoto empleando SSH como canal de comunicación.


- `rsync -a ./dev antonio@ip ip:/home/antonio`  
- `rsync -aviz --delete ise ana@192.168.56.10:/home/ana/ `


- borramos algo
- ejecutamos otra vez el comando anterior y vemos que reaparece.  

## Git   

- Software Change Management (SCM) / Version Control System (VCS) SON LO MISMOS CONCEPTOS

### Estrategias a la hora de utilizar un control de versiones  

#### Feature Branches  

- Cada característica de desarrollo de la aplicación tienen lugar en una rama dedicada a ello  

- Esto permite que varios desarrolladores trabajen simultaneamente y la rama principal sea siempre funcional   

#### Trunk based development  
- Todos trabajan en la rama master, no se subirá ningún cambio que vuelva no funcional la rama master. 
- máxima transparencia y trabajar en equipo
- problemas
(Este es el que utiliza google, a David le parece una barbaridad xD )

#### Git workflow   

Se parte de un puntu, pueden haber desarrollos paralelos a ese punto que se van integrando con merge . 

### Repositorio centralizado   

### Repositorio distribuido

### Fuentes:
- Sobre ramas, registros y trazas: https://git-scm.com/book/es/v2/Ramificaciones-en-Git-%C2%BFQu%C3%A9-es-una-rama%3F

- Feature branches: https://www.atlassian.com/git/tutorials/comparing-workflows/feature-branch-workflow
#### Guias de git  

Comandos: 

- git init: create a new repository  
- git clone: 

```
create a working copy of a local repository by running the command
git clone /path/to/repository
when using a remote server, your command will be
git clone username@host:/path/to/repository3`
create a working copy of a local repository by running the command
git clone /path/to/repository
when using a remote server, your command will be
git clone username@host:/path/to/repository

```


add & commit

You can propose changes (add it to the Index) using
git add <filename>
git add *
This is the first step in the basic git workflow. To actually commit these changes use
git commit -m "Commit message"
Now the file is committed to the HEAD, but not in your remote repository yet.


pushing changes

Your changes are now in the HEAD of your local working copy. To send those changes to your remote repository, execute
git push origin master
Change master to whatever branch you want to push your changes to.

If you have not cloned an existing repository and want to connect your repository to a remote server, you need to add it with
git remote add origin <server>
Now you are able to push your changes to the selected remote server


reate a new branch named "feature_x" and switch to it using
git checkout -b feature_x
switch back to master
git checkout master
and delete the branch again
git branch -d feature_x
a branch is not available to others unless you push the branch to your remote repository
git push origin <branch>  


update & merge

to update your local repository to the newest commit, execute
git pull
in your working directory to fetch and merge remote changes.
to merge another branch into your active branch (e.g. master), use
git merge <branch>
in both cases git tries to auto-merge changes. Unfortunately, this is not always possible and results in conflicts. You are responsible to merge those conflicts manually by editing the files shown by git. After changing, you need to mark them as merged with
git add <filename>
before merging changes, you can also preview them by using
git diff <source_branch> <target_branch>


 In case you did something wrong, which for sure never happens ;), you can replace local changes using the command
git checkout -- <filename>
this replaces the changes in your working tree with the last content in HEAD. Changes already added to the index, as well as new files, will be kept.

If you instead want to drop all your local changes and commits, fetch the latest history from the server and point your local master branch at it like this
git fetch origin
git reset --hard origin/master 

- Básica: http://rogerdudler.github.io/git-guide/  
- completa: https://www.atlassian.com/git/tutorials

### Lección 3: LAMP Stack (Linux, Apache, MySQL, PHP)

#### Ubuntu
Nos vamos a Ubuntu y ejecutamos `tasksel`, buscamos la opción `LAMP server`, la seleccionamos con espacio y pulsamos enter.

Con `systemctl status apache2` podemos ver que Apache está correctamente instalado. En `/etc/apache2` está la configuración. El archivo más importante es `apache2.conf`. Podemos comprobar que funciona correctamente lanzando un navegador y escribiendo `http://tuipdeubuntu`.

Si no funciona puede ser porque el firewall esté activo: desactívalo o añade el puerto 80 con `ufw allow 80/tcp`. También podemos usar `lynx localhost` o `curl http://localhost` (con -v podemos ver las cabeceras de la petición).

Ahora tenemos que comprobar que funciona MySQL ejecutando `mysql`. Con php igual: `php -a` (en `/etc/php/7.0` está la configuración)

Añadimos a `var/www/html/index.html` en el body:

```
<?php
phpinfo();
?>
```

Y renombramos el archivo con: `mv index.html index.php`.

#### CentOS

Queremos instalar nuestra stack (pila de software) LAMP (Linux, apache, mysql, php) para tener nuestro servidor listo. 

Para ello empezaremos instalando nuestro servidor de apache.
Como no conocemos el nombre concreto del paquete con `yum search httpd` podemos buscar los existentes. 

`httpd.x86_64 : Apache HTTP Server` esta línea es la que nos interesa. (el punto de después es información extra y no nos interesa.

- `yum install httpd`
- `systemctl status httpd` Nos permite ver el estado del servidor de apache. 
Tras ejecutarlo nos deberá salir algo parecido a esto: 

```
● httpd.service - The Apache HTTP Server
   Loaded: loaded (/usr/lib/systemd/system/httpd.service; disabled; vendor preset: disabled)
   Active: inactive (dead)
     Docs: man:httpd(8)
           man:apachectl(8)
```  
Que como vemos esta desactivado. Procedamos a activarlo 
- `systemctl start httpd` inicializamos apache (tras esto le hacemos status para cer si funciona


```
[root@localhost ~]# systemctl start httpd
[root@localhost ~]# systemctl status httpd
● httpd.service - The Apache HTTP Server
   Loaded: loaded (/usr/lib/systemd/system/httpd.service; disabled; vendor preset: disabled)
   Active: active (running) since vie 2020-03-27 20:26:56 CET; 2s ago
     Docs: man:httpd(8)
           man:apachectl(8)
 Main PID: 2330 (httpd)
   Status: "Processing requests..."
   CGroup: /system.slice/httpd.service
           ├─2330 /usr/sbin/httpd -DFOREGROUND
           ├─2331 /usr/sbin/httpd -DFOREGROUND
           ├─2332 /usr/sbin/httpd -DFOREGROUND
           ├─2333 /usr/sbin/httpd -DFOREGROUND
           ├─2334 /usr/sbin/httpd -DFOREGROUND
           └─2335 /usr/sbin/httpd -DFOREGROUND

mar 27 20:26:56 localhost.localdomain systemd[1]: Starting The Apache HTTP Server...
mar 27 20:26:56 localhost.localdomain httpd[2330]: AH00558: httpd: Could not reliably det...ge
mar 27 20:26:56 localhost.localdomain systemd[1]: Started The Apache HTTP Server.
Hint: Some lines were ellipsized, use -l to show in full.

```
Como vemos ya está corriendo, además apache ha lanzado varios procesos "pesados"  

Y tras eso hacemos hacemos permanente el servicion 
- `systemctl enable httpd` 

Todo lo relacionado a configuración de apache lo encontraremos el directorio `/etc/httpd/`

Si ahora probamos si funciona:
Desde el anfitrión abrimos un navegador y escribimos la dirección
`192.168.56.103` (en mi caso)  

vemos que no funciona x'D (tranqui, esto forma parte de la práctica y las pedagogías modernas)

Comprobamos que tenemos conexión en local, es decir desde el servidor (al que estaremos conectados por ssh) nos conectamos usando

o por telnet puerto 80 : `telnet localhost 80` 

o por curl `curl http://localhost` 

Esto nos devolverá código html e incluso para más información usar modo verboso `curl -vv http://localhost`  
Esta información es muy interesante porque entre otras cosas se nos dirá información sobre el servidor sobre el que se está corriendo la información y aprovechar sus vulnerabilidades 

Ejemplo línea que aporta información : `< Server: Apache/2.4.6 (CentOS)`

Si ahora repetimos el curl en nuestro ordenador anfitrión: 
(curl es un herramienta para transferir datos de o para un servidor utilizando protocolos soportados)

`curl http://192.168.56.103` recibiremos el siguiente mensaje: `curl: (7) Failed to connect to 192.168.56.103 port 80: No existe ninguna ruta hasta el `host'
` el cual nos interesa ua que os da más información del motivo del fallo que es... (redobles por favor...)  

QUE NO HEMOS ABIERTO EL PUERTO 80 DE HTTP jejjejejej 

Si hacemos `firewall-cmd --state` veremos que el demonio está corriendo. 

Nota informativa para poder seguir con las prácticas: 
Zonas de un cortafuegos  
Las zonas son un grupo de interfaces físicas o virtuales a las que controlamos su acceso  

Vamos a ver la configuración por defecto: `firewall-cmd --list-all` (para ver la de todas las zonas `firewall-cmd --list-all-zones`)

` services: dhcpv6-client ssh` en esta línea vemos los servicios que admitimos (falta http)
o también podríamos configurarlos por puertos. 
Para añadir el servicio bastaría con: 
- `firewall-cmd --add-service=http`

Pero esto no lo haría de forma permanente y al reiniciar el servidor lo perderiamos, para ello ejecutamos: `
- firewall-cmd --zone=public --permanent --add-service=http`  (debería de aparecer success)


- `firewall-cmd --reload`  

Si ahora volvemos a intentar entrar: desde el navegador aparecerá el contenido de la página de pruebas de apache.

##### Registro de accesos a nuestro servidor  

Como toda la información variable durante que la máquina esté corriendo, podemos acceder a ella en la carpeta var
`/var/log/httpd` en el encontraremos dos archivos `access_log` y `error_logo` que nos muestran información sobre interacciones con nuestro servidor. 

En la práctica estos archivos pesan demasiado así que cada poco se borran (aunque se pueden realizar el pertinente análisis de datos antes de borrarlos. 

#### Contenido de la página web

El contendo de una página web se divide 
- Estático (html files, css,images...)
- Dinámico (en esta práctica no se pide nada relacionado con él)  

##### Estático  

El path del documento que lo implementa lo podremos configurar con `DocumentRoot "/path/entrecomillas"`
Por defecto lo podremos encontrar en `/var/www`  . 

La página que se carga por defecto al entrar al servidor es `/var/www/html/index.html`  
Pero como todo lo de apache es configurable en el fichero `/etc/httpd/conf/httpd.conf` más en concreto buscar `DirectoryIndex` en ese fichero. 


hagamos una prueba: 
Escribimos lo siguiente en el fichero `/var/www/html/index.html`  
```htm
<html>
<head><title> Buenos y positivos días </title></head>
<body>
  <h1> ¡Buenos y positivos días! </h1>
  <h2> Estoy probando apachecillo</h2>
</body>
</html>

```

Si ahora recargamos la página en el navegador veremso el cambio. 
También podremos acceder a otras web que no sean el index escribiendo como ruta `192.168.56.103/onombreFichero.ext`
Donde `ext` es la extensión del fichero que podría ser html, php,txt...
 (si el archivo tiene una extensión que no reconoce te permite descgártelo si entras con el navegador)
 
 

##### Contenido dinámico  
Se genera en tiempo de respuesta del servidor, se puede generar de varias formas: handlers y gci programs.   
más información: https://httpd.apache.org/docs/trunk/howto/cgi.html  
(muy interesante para echarlo un vistacillo con perl o python)   



#### Configuración de php

- `yum install php`
- `apachectl restart` (Si ya lo hemos comenzado alguna  vez en lugar de utlizar `restart` podemos usar `reload`)
- o prodríamos haber utilizado también `systemctl`



También contamos con un fichero de configuración de php en etc. 
Para comprobar si el fichero es correcto existe el comando `apachctl configtest`

Creamos `index.php` en `/var/www/html` conteniendo:

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<?php
phpinfo();
?>
</body>
</html>
```

####  Configuración de MySql    

Instalamos y configuramos MariaDB:

(Si no nos acordamos  `yum search mysql`  )  
- `yum install mariadb-server`
- `systemctl enable mariadb`
- `systemctl start mariadb` ( va a tardar un poquitillo, no os austéis xD)  

Una orden interesante de `systemctl` es `systemctl list-unit-files` que permite ver el estado de todos los servicios disponibles 


Es recomnedable ejecutar `mysql_secure_installation`.

Creamos un usuario:

- `mysql -uroot -pantonio`
- `create database ise2019;`
- `grant all privileges on ise2019.\* to 'isedev'@'localhost' identified by 'isedev';`
- ejemplo más concreto que escribió el profesos:  
`grant all privileges on ise2020.* to 'alumno'@'localhost' identified by 'ise2020'; `  

- `flush privileges;`
- `exit`
- Para comprobar que funciona: `mysql -uisedev -pisedev ise2019`.
- `yum install php-mysql` y reiniciamos apache `apachectl restart`. (ahora en phpinfo() aparecerá en paquetes iniciales php-mysql)

(Si nos da pereza hacer lo anterior podemos trabajar con las bases de datos que tienen por defecto mysql: 

- `mysql mysql` (tabla por defecto)  
- `show databases;`  
)

Y sustituimos el body de `/var/www/index.php`, quedándonos así:

Nota: antes de que funcione hay que crear la base de datos, las tablas y blablbalabal
(Código aportado por el profesor)
```php
<?php 
	$mysql = new mysqli('localhost', alumno', ise2020',password');
	if ($mysql->connect_error) { 
		die('No pudo conectarse: ' . $mysql->connected_error); 
	}
	echo '<h1>Conectado satisfactoriamente</h1>';
	?> 
	<h1>Contenido de la tabla Alumnos</h1>
	<table border="1">
	<tr><td>Id</td><td>Nombre</td><td>Email</td></tr> 
	
	<?php $sql = "SELECT id, nombre, apelidos, email FROM alumnos";
	$result = $mysql->query($sql); 
	if ($result->num_rows > 0) { 
		while($row = $result->fetch_assoc()) { 
			echo "<tr><td>".$row["id"]."</td><td>".$row["nombre"]." ". $row["apellidos"]."</td>";
			echo "<td>".$row["email"]."</td></tr>\n"; 
		} 
	} 
	mysql_close($enlace); 
	phpinfo(); 
?>
```

(hay algo que no funciona y que hay que corregir) 

(Código de Antonio)
```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<?php
<?php
$link = mysql_connect('localhost', 'mysql_user', 'mysql_password');
if (!$link) {
    die('Could not connect: ' . mysql_error());
}
echo 'Connected successfully';
mysql_close($link);
?>
?>
</body>
</html>
```

Si no se ha instalado la versión php para mysql o hemos escrito mal el porgrama e intentamos acceder podemos ver el error en `/var/log/httpd/error_log`


## Mysqli guia rápida  <3  

El paradigma de trabajo es orientado a objetos ( :O Blanca del pasado se sorprendió mucho)  

Todos los comando que aquí escriba se entenderán dentro del tag de php, es decir dentro de html en una estructura 
```php
<?php 

//cuestro codiguillo php 

?>
```

- la variables creadas comienzan por $  
`$SQL = new mysqli;`  // create copy class    
y despúes iniciamos nuestra conexión en la base de datos 
`$SQL -> real_connect('127.0.0.1', 'root', 'pass', 'database');` (estos son ejemplo de php)

o bien directamente con el constructor nos conectamos (ahora ya si con nuestro contenido):

`$mysql = new mysqli('localhost', 'alumno', 'secreto','ise2020');`

Para comprobar si el acceso ha sido correcto: 

```php
$SQL_Err = $SQL->connect_errno;
   
if($SQL_Err) // if error
    exit('Error');

```  

Para trabajr con la base de datos como hariamos desde la línea de comandos: 

- creamos una variable con el texto de la búsqueda  y llamamos al método query ejemplo: 

```
$sql = "INSERT INTO MyGuests (firstname, lastname, email)
VALUES ('John', 'Doe', 'john@example.com')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

```


Para cerrar la conexión: `$SQL->close(); `
#### Fuentes: 

- Configuración httpd https://www.linode.com/docs/web-servers/apache-tips-and-tricks/apache-configuration-basics/
- apache documentación oficial: https://httpd.apache.org/docs/2.4/
- zones in firewall:
  https://www.kwtrain.com/blog/network-security-zones
 https://docs.paloaltonetworks.com/pan-os/7-1/pan-os-admin/getting-started/segment-your-network-using-interfaces-and-zones  
 - curl man
 - systemctl: https://www.linode.com/docs/quick-answers/linux/start-service-at-boot/
- php y mysql manuales:
Introducción sencillica pa' todos los públicos: https://www.w3schools.com/php/php_mysql_connect.asp  
Lo bueno: https://www.php.net/manual/es/book.mysqli.php

#### Notas gráciles  

Para estas prácticas vamos a tirar mucho de editores y de manual, si no estamos acostumbrados a nano o vi podemos buscar alternativas con las que estemos acostumbrados. 

Yo he utilizado  `emacs` en su versión `emacs-nox` que es la versión de terminal, más que nada porque me siento muy a gusto con sus atajos, coloreo de sintaxis y para consultar el man de manera más cómoda o ejecuar comandos terminal sin salir del editor. ¿por qué no le das una oportunidad a emacs? ;)

