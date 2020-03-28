---
title: Práctica 2 - Ingeniería de Servidores
author: Sergio Quijano Rey
---

--------------------------------------------------------------------------------

# Lección 1 -- SSH y Firewall

## Conceptos Previos

* `ssh` remplaza el protocolo de `telnet`
    * `telnet` realiza la comunicación en abierto, es decir, sin cifrar
    * Por tanto no es un protocolo seguro
* Características de `ssh`
    * Corre sobre el protocolo `TLS`: `TCP/IP` cifrado
    * Toda la comunicación es cifrada
* Ambos son protocolos de *shell* remota, nos conectamos a la shell de un ordenador a través de red
* Mirar `fail2ban` en casa porque entra en el examen pero no lo va a explicar

### Cifrado y autenticación de comunicaciones

* Cifrado simétrico:
    * Misma llave para cifrado y descifrado
    * Ejemplos de protocolos: `DES` y `TDES` o `triple-DES`
* Cifrado asimétrico:
    * Una llave pública y otra llave privada
    * El contenido cifrado con una llave, solo se puede descifrar con la otra llave
        * Clave pública y privada pueden tomar cualquier rol en estos pares
    * La idea es tener la llave privada como tal, privada, y la pública la hacemos conocer a cualquier ordenador
    * Jugando con el rol privado-público podemos obtener distintas funcionalidades
    * Para enviar un mensaje a A, encriptamos el mensaje con la pública de A. Solo A con su clave privada puede descifrar este mensaje
    * Para autenticar que un archivo es de A, A cifra su documento con la privada, y el resto de ordenadores pueden comprobar la autenticidad descifrando con la llave pública de A
    * Para identificar, como es el caso de `ssh` realizamos lo siguiente: A quiere identificar a B como tal:
        1. A envia un mensaje que quiere que B cifre con su llave privada
        2. A cifra el mensaje con su llave privada y lo envía a B
        3. B descifra con la clave pública de B y autentica a B
    * El algoritmo más importante es el de `RSA`
    * El cifrado es muy costoso, por lo que se usa como paso inicial previo a comunicación a través de cifrado simétrico
        * Se comparte la llave simétrica con el cifrado asimétrico
* Problemas del cifrado simétrico
    * Una llave para una organización es inseguro
    * Un par de llaves para cada pareja de la organización es más seguro, pero más complicado de manejar
* **Hash**:
    * Tomamos un tamaño cualquiera de información, y generamos un valor aleatorio de tamaño fijo, sea cual sea el tamaño de entrada
    * Propiedades:
        * Para la misma información, se genera el mismo hash
        * Cambios ínfimos en la información original genera un hash distinto. Si es bueno, el hash nuevo cambia drásticamente
        * Dado el hash, no podemos recuperar la información original
    * `SHA` o *Standard Hash Algorithm*
    * Se usan para cifrar contraseñas o secretos
    * `BCRYPT`
        * Tomamos la contraseña o secreto
        * Le añadimos un prefijo conocido
        * Generamos un hash
        * Almacenamos en la base de datos de contraseñas, el valor del hash, junto al prefijo que hemos añadido
        * Además, `BCRYPT` hace unas cuantas pasadas para ser más robusto
* El mecanismo de criptomonedas es buscar la información original, con fuerza bruta, que genera un hash dado
* **Firma digital** usando *DSA*
    * Calculamos el hash del documento
    * Se añade el hash al documento
    * Se añade también al documento el hash del documento cifrado, que lo firma
* **Certificado digital** a través de **Autoridades de Certificación**
    * Fábrica de Moneda y Timbre, DNI electrónico
    * Un certificado digital se compone de:
        * Datos adicionales
        * Llave pública
        * Firmado electrónicamente por una autoridad (el hash cifrado)
* Las autoridades de certificación se agrupan con cadenas de certificación

### SSH

* Error REMOTE HOST IDENTIFICATION CHANGED
    * Me conecto a la VM, el servidor nos devuelve su llave pública
    * Puede que haya un maninthemiddle
    * Las claves públicas de los servidores a los que nos conectamos se guardan en `~/.ssh/known_hosts`: guardan pares servidor-clave pública
    * Borramos la parte de la antigua ip publica para poder acceder
    * Nos solicita por la confirmación. En entorno profesional se guarda la raíz de certificación
    * El cliente propone con cifrado asimétrico la futura contraseña para la comunicación por cifrado simétrico

## Proceso

1. Creamos una máquina virtual con Ubuntu Server y otra máquina virtual con CentOS
    * Particiones estándar (las que vienen por defecto en los instaladores) con 10GB de base
2. Crear y configurar, como en la práctica anterior, dos tarjetas de red: NAT y HOST-ONLY
3. Configurar ssh en Ubuntu:
    1. Vemos que sshd no está instalado en el sistema:
        * `systemctl status sshd`: con esto vemos que el servidor `ssh` no está instalado
        * `systemctl list-unit-files`: muestra todos los servicios, para comprobar que de verdad no está instalado (podría estar desactivado)
    2. Instalar sshd
        * `tasksel`: para instalar ssh, el profesor no lo va a hacer así
            * Ofrece tareas de administración en modo texto que simula GUI
            * Para hacer ssh y configurar usuarios, servicios, paquetes, sin sufrir la terminal (GUI a través de terminal)
            * Nosotros vamos a hacerlo con la terminal
        * `sudo apt update`: lee los repositorios de paquetes, actualiza las descripciones únicamente de los paquetes
        * `apt search sshd`: para hacer búsqueda de nombres de paquetes
        * `apt install open-ssh-server`: instalamos el servidor
            * Esta instalación configura ssh como servicio
    3. Comprobamos qué tenemos disponible
        * `systemctl status sshd` para ver que el serivicio ya funciona
        * `ping 192.168.56.1`: probamos tenemos conectividad con el HOST (gracias a que hemos configurado la tarjeta HOST-ONLY)
4. Nos conectamos a la VM desde el HOST
    * Desde el host: `ping 192.168.56.10`, prueba conectividad con la VM
    * Desde le host `ssh sergio@192.168.56.10`: nos conectamos a la VM a través de ssh
        * Nos solicita si confiamos en la clave pública
        * Como se explica en los conceptos previos, esto escribe en `known_hosts` de nuestro portátil
5. Configuración de ssh en Ubuntu Server (trabajamos con la consola de nuestro ordenador pero en Ubuntu)
    * `cd /etc/ssh`: 
        * `ssh_config`: configura el cliente
        * `sshd_config`: configura el servidor
    * `vi sshd_config`
        * Cambiamos del puerto 22 al 22232: `Port 22232`
        * `systemctl restart sshd`: mataría el servicio y lo inicia. No hacemos esto para no matar nuestra conexion
        * `systemctl reload sshd`: deja las conexiones vivas, pero solo escucha en el puerto nuevo
6. Comprobamos en el host las consecuencias del cambio de puerto
    * `ssh sergio@192.168.56.10`: no se conecta, 
        * La otra conexion sigue viva gracias a que hemos hecho `reload` en vez de `restart`
    * `ssh -p 22232 sergio@192.168.56.10`: sí que se conecta
7. Deshacemos el cambio de puertos en Ubuntu, pasándolo otra vez al puerto 22
8. Ejecución de comandos a través de ssh
    * `ssh sergio@192.168.56.10 "ls -l"`: ejecuta un comando
    * Más que utilidad para conectarse en remoto, se usa para lanzar comandos contra servidores. En esto se basa en parte Ansible
    * `ssh` juega un papel fundamental para la automatización de sistemas
    * Normalmente, se reduce todo un servicio a `ssh` junto al servicio normal, para reducir los puntos débiles
9. Instalación del servidor ssh en CentOS
    1. `systemctl status sshd`: vemos que viene instalado por defecto
    2. `ping 192.168.56.1`: comprobamos la conexión con el host, a través de la tarjeta HOST-ONLY
10. Conexión ssh con CentOS
    1. En el host, `ping 192.168.56.20`: comprueba la conexión a través de HOST-ONLY
    2. En el host, `ssh root@192.168.56.20`: accedemos directamente a root, esto en Ubuntu no se permite
11. Quitamos RootLogin
    1. `vi /etc/ssh/sshd_config`: cambiamos a `PermitRootLogin=no` para no permitir conexión directa a ssh
    2. `systemctl reload sshd`: para cambiar el servicio sin matar la conexión actual
12. Compruebo en el HOST los efectos del cambio
    1. `ssh root@192.168.56.20`: ahora no podemos acceder al root
    2. `ssh sergiot@192.168.56.20` y desde aquí escalamos privilegios a root
    * Este esquema es muy común en servidores, cerrar el acceso a un usuario no privilegiado desde el que escalar privilegios
13. Cambiamos el nombre de la máquina CentOS, para más claridad a la hora de trabajar con ssh
    * `vi /etc/hostname` y ponemos el nombre que se quiera
14. Creamos una clave ssh en Ubuntu: ==> A partir de aqui no he hecho nada
    * `ssh-keygen`: crea la clave en `.ssh/id_rsa` y en `.ssh/id_rsa.pub`
        * No ponemos phassphrase por comodidad, en producción si que lo usaríamos
        * Con `-c` podemos poner comentarios en la clave
    * Si perdemos la clave pública, la podemos recuperar con la privada
15. Copiamos la clave pública de Ubuntu en CentOS
    * `ssh-copy-id sergio@192.168.56.102`
        * Tenemos que introducir una clave para que CentOS nos autentifique
        * Este comando hace el proceso automáticamente, pero podríamos hacerlo manualmente copiando la clave pública y colocándola en `.ssh/authorized_keys`
    * En CentOS, comprobamos la clave haciendo `less .ssh/authorized_keys`
16. Vemos los mensajes que se transfieren en el protocolo ssh
    * Esta parte es opcional, es por curiosidad del profesor
    * Desde Ubuntu: `ssh sergio@192.168.56.102 -vv`
        * `-vv` para ver los mensajes del protocolo
17. Añadimos un hostname
    * Haciendo esto, en vez de lanzar un comando `ssh username@ipaddr`, podemos hacer `ssh username@hostname`
    * Los hostnames se añaden editando `/etc/hosts`
        * De la forma `ipaddr hostname`
    * Editamos el archivo y nos conectamos haciendo `ssh sergio@ubuntuserver`
    * Esta configuración también se puede hacer editando, en nuestro host, `~/.ssh/config`, como se muestra en el siguiente código
        * Esta opción es mejor, porque la configuración la hacemos a nivel de usuario, no a nivel de todo el sistema
        * Además podemos especificar muchos más parámetros

~~~
Host UbuntuServer
    Hostname 192.168.56.101
    user sergio
~~~

18. Copiamos la clave de CentOS en Ubuntu a mano
    * Copiamos la clave de CentOS en el `clipboard` de nuestro sistema
    * Editamos, en Ubuntu, `.ssh/authorized_keys` y pegamos lo que tenemos en el `clipboard` del sistema
19. Usamos `ssh-agent` para trabajar con las claves de nuestro host
    * Descripción del problema:
        * Imaginemos que necistamos usar, desde nuestro servidor, un servicio configurado con las claves de nuestro host
        * Puede ser el caso de un repositorio git
        * Podemos configurar el servicio para que use las claves del servidor, pero esto puede hacer que los administradores del servidor nos roben la identidad en el servicio
        * Podemos copiar las claves del host en el servidor, pero esto puede hacer que los administradores del servidor nos roben la identidad en cualquier servicio
        * Solución: usamos `ssh-agent` que copia, en la sesión, las claves del host en el servidor
    * Desde nuestro host:
        * Iniciamos con `eval $(ssh-agent)`
        * `ssh add .ssh/id_rsa`: copia en la sesión la clave privada 
        * `ssh-add -l`: para mostrar las claves privadas que tenemos almacenadas en `ssh-agent`
        * `ssh -A sergio@ubuntuserver`: nos conectamos a Ubuntu con las claves de `ssh-agent`
    * Dentro de ubuntu: `ssh-add -l`: vemos que la clave ha llegado al servidor de Ubuntu
    * Si editamos en nuestro host `~/.ssh/config`, y añadimos `ForwardAgent yes`, no tenemos que hacer `ssh -A ...`
20. Lanzamos comandos con ssh
    * `ssh sergio@centosserver "<comando>"`: lanza un comando en el servidor de CentOS
    * Como ya se ha comentado, este es el verdadero potencial de ssh, lanzar comandos
    * Consituye la base de herramientas como `Ansible`

--------------------------------------------------------------------------------

# Lección 2 -- Sistemas de Backup y Git

## Teoría

### Copias de Seguridad

#### DD - Disk Dump | Disk Destroyer

* `dd` permite copias `raw`, nos saltamos completamente el sistema de ficheros
* Borrar un archivo con `rm` es simplemente borrar una entrada de la tabla de inodos
* `copy` baja al device a través del sistema de ficheros de archivos
* `dd` se salta la parte del sistema de ficheros. Se usa para generar archivos artificiales
* Por ejemplo: `dd if=/dev/zeros of=~/pruebas.dat bs=1024MB count=2`
    * `if`: indica el archivo de entrada
    * `of`: indica el archivo de salida
    * `bs`: indica el tamaño de bloque
    * `count`: indica el numero de bloques de tamaño `bs` se copian
    * Se puede poner `status=progress` para ver de forma interactiva como avanza el comando
* Hay que tener mucho cuidado con este comando, la escritura en `raw` puede destruir todo un disco, una partición...
* Se suele usar bastante para hacer un `LiveUSB` sin necesitar una `GUI`

#### CPIO

* Es una utilidad para hacer backups en archivadores
* Un archivador es un fichero que contiene varios ficheros a su vez
* Funcionamiento:
    * Copia una archivo tras otro
    * Pone marcas en medio para distinguirlos
    * No usa, por defecto, compresión
* Lo que buscamos es hacer un backup de varios archivos que se guarda en un solo fichero. Análogo a comprimirlos en un `.zip` pero con diferencias:
    * No estamos comprimiendo los archivos (aunque se puede indicar)
    * Podemos darle información a `cpio` para que al extraer los archivos, recree la estructura de directorios original
* En centos, por ejemplo: `/boot/initrd-plymouth.img` está en formato cpio
* En la sección de proceso tenemos un ejemplo de cómo usar cpio
* Es muy antiguo, pero ciertas partes del sistema (como ya se ha comentado), lo siguen usando

#### Rsync

* `rsync` protocolo para sincronizar dos directorios, aunque estén en dos computadores distintos
* Como protocolo de comunicaciones, se basa en `ssh` para reducir las debilidades de seguridad a un punto
* Es útil para hacer sincronización de dos directorios, normalmente entre un host y un servidor
* No se puede hacer `rsync` sobre dos máquinas remotas (ssh en una máquina y lanzamos `rsync`)
* Las sincronizaciones no se pueden programar temporalmente directamente usando `rsync`
    * Se recurren a herramientas externas como `crontab`
* En la sección de proceso tenemos un ejemplo de cómo usar cpio

### Sistema de Control de Versiones

* Software Change Management SMC es lo mismo que Version Control System VSC
    * El primero es el término oficial
* Funciones:
    * Almacenamiento centralizado
    * Origen de verdad
    * Varios desarrolladores a la vez
    * Copias de seguridad del proyecto
    * Dispone de changelog o registro de cambios de versiones
    * Manejan las diferencias sobre archivos de texto, no sobre archivos binarios
        * Existe gestión de versiones para binarios (por ejemplo, video)
* GIT
    * Parte de un directorio que es la raiz del proyecto: `PROJECT_ROOT`
    * Todas las carpetas que cuelguen de ahím se guarda como unidad de versiones. Cualquier cambio debajo de ese punto, se considera un cambio en el proyecto, y se genera una nueva versión completa
    * Los cambios son a nivel de proyecto global, no de archivo
    * Cada cambio en git se considera un commit
    * Cada commit es una fotografia entera del sistema y se almacena. Se guardan solo las diferencias respecto a la versión anterior para ser eficientes a nivel de almacenamiento
        * Esto considerando los tres niveles: workdir, stage y repositorio
    * Además se documenta el cambio
        * Automático: quien y cuando se hizo
        * Para qué se hizo: con una descripción que hace el desarrollador
    * Se puede hacer `blame` a nivel de fichero o de linea para ver quién hizo el cambio y en que commit
        * Se puede hacer una trazabilidad completa de todos los cambios
* GIT permite el desarrollo en ramas
    * Los equipos de desarrollo trabajan en paralelo sobre la misma base de código
    * Se pueden hacer bifurcaciones de las versiones: son las ramas o *branches*
    * Se suele tener un `main branch`
    * A parte del nombre, nada diferencia de `main branch` que de otras `branches`
    * Da problemas a la hora de hacer un `merge` porque se generan conflictos
    * Este coste de `merge` puede no merecer la pena. Se dan tres aproximaciones:
        * Feature branches
            * Equipos independientes en ramas diferentes
            * Una rama para cada funcionalidad nueva
            * No puede durar mas de tres dias una rama sin hacer `merge`
        * Git Workflow
        * Trunk Based Development
            * Solo una rama
            * Es fundamental que los programadores esten todo el rato subiendo los cambios
                * Los cambios no deben romper el codigo, porque hace que los otros desarrolladores no puedan seguir trabajando
    * Por este coste nace la integración continua y continuous delivery 
* El branching es fundamental en todas las herramientas de desarrollo
* Subversion gestiona las versiones a nivel de directorio, lo que introduce muchos problemas
* Es una buena práctica tener ciertas reglas y directrices a la hora de hacer commits, como lo que se conoce por *Meaningful commits*
* Tipos de servidores
    * Centralizados
        * La copia del software siempre está en un servidor compartido
        * Favorece la integración continua
        * No se puede trabajar sin estar conectado al servidor (falta de internet)
        * El commit automáticamente sube el cambio al servidor
        * Se tienen que bloquear los archivos directorios antes de trabajar con ellos
    * Distribuido (los modernos) -> bazaar, mercurial, git
        * No existe un servidor central
        * Clono el servidor en local -> puedo seguir trabajando con todas las funcionalidades
        * No bloqueas a otros desarrolladores
        * Permite forks, hago una copia y sigo desarrollando otro proyecto partiendo de la base, ya no de código, sino de historia del proyecto
        * El opensource se beneficia de esto
        * Desde un repositorio se pueden registrar cambios contra el repositorio "original" 

## Proceso - Copias de Seguridad

1. Probamos el funcionamiento de `cpio`: desde Ubuntu
    * `sudo ls /etc/*.conf | cpio -vo > ~/sergio/conf_backup.cpio`: hace una copia de seguridad de los archivos de configuración
        * `-v`: verbose
        * `-o`: output, toma una lista de nombres de fichero y los almacena en un archivador
    * `mv /etc/yum.conf ~/yum.conf`: para simular que se rompe el archivo de configuración
    * `cpio -iudv yum.conf < conf_backup.cpio`: descomprime los archivos guardados, en el directorio original
        * `-v`: verbose
        * `-i`: input: lee una lista de nombres de archivos y los recupera del archivador
        * `-u`: unconditional: sobreescribe ficheros existentes
        * `-d`: directories: crea la estructura de directorios
    * `less /etc/yum.conf`: vemos que el backup ha funcionado
    * Para hacer `cpio` sobre toda una estructura de directorios, se puede hacer:
        * `find <base_dir> -t f | cpio -vo > <backup_file>`: `find` va colocando los path de los archivos, podemos usar el comando ya visto para recuperar todo
2. Probamos el funcionamiento de `rsync`: desde Ubuntu
    * Creamos una estructura de directorios de juguete:
        * `mkdir ise`
        * `touch archivo1`
        * `mkdir ise/p1`
        * `mkdir ise/p2`
        * `touch ise/p2/pruebas`
    * Copiamos la estrucutra en CentOS: `rsync -a --delete sergio@192.168.56.102:/home/sergio`
        * `-a`: para mantener los metadatos de los archivos
        * `--delete`: para borrar, en otro caso se acumulan archivos
    * Personalmente yo uso `-zaP`
        * `-a`: para mantener los metadatos de los archivos
        * `-z`: para comprimir el tráfico
        * `-P`: para ver el progreso de la copia
## Proceso -- Control de Versiones
        
### Alguna teoría previa

* En cada `git commit`:
    * Se calcula el SHA de todo el proyecto -> id del commit
    * Dentro de `.git` se se guardan las diferencias con el directorio anterior
        * Base de datos de la forma: Clave primaria, diferencias, commit padre
            * Con clave primaria el SHA del proyecto
        * Al hacer un branch, generamos un commit que tiene mas de dos hijos
    * Cada cierto tiempo se hace un snapshot completo por temas de eficiencia
* En verdad, cada rama es un tag que se le pone a un commit
* Es util hacer ramas locales y mergearlas contra master en local. Push de los cambios solo sobre master. No se tiene constancia en el servidor de las ramas locales con las que hemos trabajado

### Comandos en Ubuntu

1. Creamos una estructura basica para el futuro repositorio
    * `mkdir isegim`
    * `cd isegim`
    * `git init`: inicializa un repositorio de git, crea un `.git`
        * En `.git` se gestiona todo la historia y contenido. Toda la base de datos de cambios de git
    * `vi README.md`: archivo estándar que muestra la información del proyecto
2. Vemos el estado del proyecto
    * `git status`: para ver el estado del proyecto
    * `git add README.md`: pasamos el archivo al `stage`
        * No se suele hacer stage, ya tiene poca utilidad, directamente se pasa al commit
        * `git log`: no tenemos cambios, el staging no sirve para demasiado
3. Configuramos git
    * `git config --global user.name "david.palomar"`: quiere nombres y apellidos
    * `git config --global user.email "sergiquijano@gmail.com"`: quiere nombres y apellidos: USAR CUENTA DE LA UGR
    * Usar local para que no me toque toda la configuracion de la maquina
        * Así se guarda en `<repo>/.git/config`, en vez de `~/.git/config`
        * Se guarda en así con local en `<repo>/.git/config`, si no en `~/.git/config`
    * Copiamos la clave pública en github
4. Hacemos el commit
    * `git commit`: para guardar el cambio hecho
5. Añadimos el repositorio remoto
    * `git remote add github git@github.......`
        * Necesitamos copiar nuestra clave ssh en github
        * Al principal se suele poner origin
6. Hacemos push
    * `git push github --set-uptstream github master`: esto no lo entiendo demasiado
        * Nosotros `origin`: con origin se puede omitir

### Comandos en CentOS

1. Instalamos y configuramos git
    * `yum install git`
    * Copiamos la clave pública en github
    * git confing local user.name="chungo.quijano"
    * git confing local user.email="nian.nombrechungo@gmail.com"
2. Bajamos el repositorio
    * `git clone git@github.com/<nombre_repo>`
3. Creamos una rama
    * `git branch sergioQuijanoRey`
        * git checkout -b sergioQuijanoRey hace lo mismo pero además se mueve a la nueva rama

### Tarea -- Ubuntu y CentOS

1. En Ubuntu y master:
    * Ponemos nuestro nombre en fichero `.csv` ubicado en la rama `master`
    * `git commit -a` : registramos el cambio
2. En CentOS, tras haber creado la rama nueva `sergioQuijanoRey`
    * Edito el archivo `.csv` añadiendo un nombre raro
    * `git commit -a`: para hacer `git add . ; git commit` automáticamente
    * `git log`: veo que no se ha hecho el cambio
3. En Ubuntu y en la rama master
    * `git log`: para ver el cambio en paralelo
    * `git push --set-upstream github master`: para colocar el upstream
4. Vemos los resultados:
    * En github: vemos que en master esta todo como en el principio
5. En CentOS y branch `sergioQuijanoRey`
    * `git push origin sergioQuijanoRey`: hacemos push sobre la rama alternativa
6. Volvemos a ver los resultados
    * En github vemos que hemos creado una nueva rama
    * Los cambios están sobre la rama nueva, la rama `master` sigue intacta, con lo hecho desde Ubuntu
7. En CentOS y branch `sergioQuijanoRey`
    * `git checkout master`: cambiamos a la rama master
    * `git pull`: para tomar los cambios hechos desde Ubuntu
    * `git merge sergioQuijanoRey`: fusionamos la rama `sergioQuijanoRey` sobre la rama `master`
        * Nos muestra el conflicto
    * Editamos el fichero quitando los comentarios del `merge conflict` 
    * `git commit`: para registrar que hemos resuelto el conflicto
    * `git push`: para hacer efectivo la resolución del conflicto

--------------------------------------------------------------------------------

# Lección 3

## Introducción y teoría

* Parte del firewall lo tenemos que hacer nosotros de forma autónoma
* El profesor instalará el framework o stack `LAMP`
    * Linux
    * Apache
    * MySQL o MariaDB (a veces MongoDB)
    * PHP o Python
* MariaDB es el Fork open source de MySQL
    * Compatible a nivel de API (mismas llamadas)
    * Aurora aws tambien es base de datos compatible con mysql, desarrollada por amazon
* Apache HTTP
    * Uno de los más antiguos y más robustos
    * Es el servidor web más utilizado del mundo
    * Un servidor web alternativo es `nginx`
        * Se utiliza mucho como proxy inverso (servidor frontend que tiene detrás servidores que son las que toman la carga que el frontend va distribuyendo)
        * Muy potente ejecutando reescrituras (distribuye bien la carga entre otros servidores)
        * Muy bueno accediendo a dispositivos (paralelismo en colas, que no sobrecargan la CPU)
        * Entradas / Salidas asíncronas
* EL .x86_64 siempre sobra en el yum install!!!
* Node.js no es concurrente, por lo que para ciertas tareas no es buena herramienta

## Proceso:

* Desde CentOS (previa conexion por ssh):
    * `sudo su`: para hacer instalacion de paquetes
    * Instalación de Apache como servidor HTTPD
        * `yum search httpd`
        * `yum install httpd-x86_64`
        * `systemctl status httpd`: cargado pero inactivo
        * Se puede usar `apachectl` para administrar el servicio, vamos a usar `systemctl`
        * `systemctl enable httpd`
        * `systemctl start httpd`
        * `systemctl status httpd`
        * Vemos que apache lanza bastantes hijos, que depende del sistema se lanzan en hebras o en procesos pesados
            * En el caso del profesor, se lanzan en procesos
            * Esto lo ve haciendo `ps ax | grep httpd`
        * Su configuracion está en `/etc/httpd`
        * `cd /etc/httpd`
        * `cd conf`: miramos la configuración de HTTP
        * `cd ../conf.d`: configuración modularizada
    * Probamos que Apache funciona solamente:
        * Desde nuestro ordenador: `192.168.56.20`: no puedo
        * Desde centos: `curl http://localhost`: devuelve la pagina html
        * Desde centos: `curl -vv http://localhost`: devuelve las llamadas del protocolo más la página html
        * Desde nuestro ordenador: `curl http://192.168.56.20`: conexión rechazada => nos está bloqueando el firewall
    * Firewall centos : desde centos
        * `firewall-cmd --state`: esta running, queremos ver que reglas tenemos
        * `firewall-cmd --list-all`: veo las reglas definidas
        * firewall-cmd --zones: no es asi pero no lo he visto, TODO preguntar a lucia o a quien sea
            * Dice que `firewall-cmd --list-all-zones`
    * Para ver servicios: `less /etc/services`
    * Abrimos el puerto 80 en Centos
        * `firewall-cmd --zone=public --permanent --add-service=http`: 
            * Lo hacemos por nombre pero se puede hacer por puerto (hay que saber cómo se hace)
            * `--permanent`: para que sea persistente
        * `firewall-cmd --reload` para que el cambio sea visible
    * Ahora podemos acceder, desde CentOS y desde fuera de CentOS, desde nuestro ordenador
* Instalamos php
    * `yum search php`
    * `yum install php`
    * `php` y accedemos al intérprete
    * php -v: para ver que ya esta
    * less /etc/php.ini: para ver la configuración
* Versión larga: `<?php ... >`: se indica el nombre del lenguaje
    * `short open tag Off`
* Logging:
    * El log de errores de php se manda al mismo sitio que APache porque se generan con HTTP: en `/var/log`
    * `cd /etc/httpd`
    * `less access_log`
        * Archivo muy largo, generan gigas, por lo que no suelen ser persistentes en el tiempo
        * Se tienen media hora, una hora... Depende del tráfico gestionado
        * Se pueden usar para tener estadísticas de usuarios, de uso por países...
    * `less error_log`
        * Errores de Apache
        * Php los manda aqui tambien porque php los lanza con HTTP
* Document Root ==> se encuentra en var
    * cd /var/www
        * cgi-bin: protocolo antiguo
    * cd html
        * Aqui se colocan los archivos
        * index.html: fichero por defecto que apache busca
        * vi index.html: creamos un archivo básico html para mostrar algun mensaje
* Accedemso a la página con el navegador de nuestro portátil
* Creamos un php
    * `vi hola.php`
    * cat index.html > hola.php

~~~php
<html>
<head>
    <title>Pruebas de pagina web</title>
</head>
<body>
<h1>Probando PHP</h1>
<?php
phpinfo();
>
</body>
</html>
~~~

* Si volvemos a acceder a la pagina, no se interpreta php, http://192.168.56.20/hola
* Lo tenemos instalado pero no activo
* cd /etc/http/conf.d
* Vi httpd.conf
    * `IncludeOptional conf.d/*`: hace que se modularice la configuracion
    * DirectoryIndex: muestra el archivo por defecto
* vi php.conf: por mirarlo, no tocamos nada
* Ahora reiniciamos apache
    * `apachectl reload`: no funciona
    * `apachectl restart`: si funciona
        * No tocamos la configuracion, pero tenemos que traer el interprete de php a memoria. Por eso este comando funciona
* Para verificar las configuraciones: `apachectl configtest`
* Instalamos mysql
    * yum search mysql ==> no opensource, no se distribuye
    * yum search mariadb
    * `yum install mariadb-server.x86_64`: cuidado con no instalar el cliente, queremos el servidor
    * systemctl status mariadb
        * Cargado pero no habilitado
    * systemctl start mariadb
    * systemctl enable mariadb
* Configuramos mysql
    * En produccion hariamos `mysql_secure_installation`
    * mysql mysql: la segunda es la base de datos de metainformacion. Con esto demostramos que funciona
        * > show databases: muestra esquemas
        * > show tables: muestra las tablas
* Para consultar todos los servicios a la vez (por si no se el nombre) `systemctl --list-unit-files`
    * static: no son servicios, son configuraciones
* Conectamos php con mysql, abrimos en dos consolas:
    * vi /var/www/hmtl/hola.php
    * vi /var/log/httpd

~~~php
<html>
<head>
    <title>Pruebas de pagina web</title>
</head>
<body>
<h1>Probando PHP</h1>
<?php
    phpinfo();
>
<h2>Probando conexion con MySQL</h2>
<?php
COPIAMOS EL COMANDO DEL GUON $mysql = new mysqli('localhost...')
>
</body>
</html>
~~~

* En el archivo dado de ejemplo, cambiamos usuario por alumno y clave por ise2020
* Creamos una configuracion de pruebas
    * `mysql`
    * `Create database ise2020;`
    * `grant all privileges on ise2020.* to 'alumno'@'localhost' identified by 'ise2020';`
        * Este comando también crea el usuario y le asigna el usuario sin mas engorro
    * `exit`
    * mysql -u alumno -p ise2020 ise2020: se conecta bien
* Accedemos a la pagina y vemos que todo va bien
    * tail -f /var/log/httpd/error_log: para que se muestre en tiempo real
    * Vemos si aparece el mensaje *conectado satisfactoriamente*
        * Da un error porque no tenemos la clase mysqli cargada por defecto, como se muestra en el log
        * Hay que instalarlo
    * yum search php | grep mysql
    * yum install php-mysq : EL .x86_64 siempre sobra
* apachectl restart para que se cargue el mysql
    * Esto se puede mostrar en las tablas que muestra el phpinfo en la pagina web
    * Ahora vemos que pone conectado satisfactoriamente

## Tarea

* Creamos una tabla con los campos de la segunda parte que no hace
* Insertamos el código que falta
* Repetimos todo el proceso en Ubuntu
