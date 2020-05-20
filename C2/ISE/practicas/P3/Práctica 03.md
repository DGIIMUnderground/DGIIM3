---
title: Práctica 3 - Ingeniería de Servidores
author: Sergio Quijano Rey
---

--------------------------------------------------------------------------------

# Lección 1 -- Monitorización y Profiling

## Teoría Previa

* **Monitorización**: evaluamos sistemas ya en producción, o como mucho en preproducción
* Tres sistemas de monitorización diferenciados:
    * **Telemetría**: evaluar sistema operativo (llamado plataforma): accesos a disco, redes, llamadas al sistema, cambios de contexto...
    * **Loggin**: mirar los logs que generan las aplicaciones, como hemos visto con Apache
        * Estos logs se suelen procesar porque son muy pesados
        * Arquitectura ELK Stack
            * Toma los logs y los trocea para enviarlos a una base de datos
            * Base de datos optimizada para lectura y no escritura (NoSQL como MongoDB)
            * Tiene un dashboard *Kibana* para mostar datos
        * Otras soluciones te dan información sobre quiénes llegan desde home, desde una red social, cuanta gente llega a hacer un pedido... Analytics se llaman
* **Profiling**: evaluación de prestaciones antes de llegar a producción
    * Medir tiempos de ejecución de un `C++`, por ejemplo
    * Podemos localizar los métodos que consumen más tiempo dentro de un programa, así sabemos dónde tenemos que centrar los esfuerzos de mejoras en el código
* En la práctica vamos a ver:
    * Monitorización básica del sistema
    * Monitorización avanzada con *Zabbix*
        * Podríamos usar *Munin* y *Nagios* que se usan mucho en la industria
        * Por motivos académicos elegimos *Zabbix*
    * Automatización con *Ansible* 

## Utilidades que vamos a usar

### dmesg

* Comando para consultar el buffer circular de logs del kernel de linux
* Los logs del kernel van directamente a memoria principal sobre buffer circular
    * Si el buffer se llena, se sobrescribe lo más antiguo
    * El tamaño del buffer se puede cambiar recompilando el kernel y piensa que con `sysctl`, aunque no lo consigue
    * Se usa para depurar aspectos críticos del sistema operativo
    * Por ejemplo, al lanzar el sistema operativo, muchos comandos no se muestran en consola, se puede consultar con dmseg
    * Los mensajes salen directamente del código fuente del kernel de linux

### Log de servicios y aplicaciones

* `/var` contiene información que varía a lo largo de la vida de un sistema linux
* Mirar *linux hierach file system*
* Dentro de var está log donde se guardan todos los logs: `/var/log`
* Los logs pueden ocupar un tamaño enorme
    * Para gestionar eso se suele usar `logrotate`
        * Cada cierto periodo que se especifica, se rota un archivo
        * Se copia el log, `<logname>.1` o `<logname>.<date>`, `<logname>.2` y así... El original se pone a cero (mas que una copia es un `move`)
        * Asi para abrir estos archivos y trabajar con ellos 
        * Se puede rotar con un tamaño umbral
        * Se pueden rotar y comprimir: `syslog.2.gz`
        * Se puede especificar el numero de archivos a guardar antes de ir borrando
        * Permite lanzar scripts antes o despues de un rotate

### who

* Muestra los usuarios que se han conectado en el sistema
* `last` es mas completo, se muestran los comandos lanzados por los usuarios

### Grep

* Se busca informacion en los archivos
* Admite recursividad
* Admite expresiones regulares
* Admite recursividad sobre directorios

### proc

* Sistema de ficheros virtual, montado en `/proc`
* Creado por el kernel
* Vive en memoria principal: por tanto es volátil
* El propósito de este sistema de archivos virtual es analizar información del kernel
* Otras utilidades como `ps`, `top` consultan el sistema proc
    * De este modo, proc es el origen de la informacion para scripts y utilidades
* Subdirectorio `/proc/sys`:
    * Se pueden modificar sus archivos, teniendo impacto sobre parámetros del sistema
    * Es más fiable usar `systemctl` para modificar estos parámetros

### Cron 

* Programación de tareas en una fecha concreta
* Standard de facto para programación de tareas
* Servicios modernos se basan en esto, y en su formato (`ansible`, *aws*)
* `CentOS` esta remplazandolo con `systemctl timers` que son más avanzados
    * Más complejos de configurar pero más finos
    * Granularidad a nivel de microsegundo
    * Si hay una tarea perdida (sistema apagado en su momento de lanzarlo), se puede configurar una política de resolución para esto
* `crontab-guru` para mirar formatos de cron en una página web
* Se puede hacer que las tareas sean logeadas en el syslog
* Toma variables de entorno de `/etc/profile`:
    * Se pueden definir variables de entorno directamente sobre crontab
    * Suele dar problemas en los scripts que usan variables de entorno

## Utilidades que están en el guión pero que no vemos en la práctica

* `hddtemp`: mide la temperatura de los discos duros
* `lm-sensors`: control de temperaturas, voltajes y ventiladores
* Comandos para mostrar el hardware del equipo:
    * `lscpu`
    * `lsusb`
    * `lshw`

## Tarea

* Se lleva a cabo solo sobre una máquina
* Crear un RAID1 con dos discos
    * A través de `mdadm`
    * Montamos el RAID en `/var`
* Con `cron` y con la información de `/proc/mdadm` o `/proc/mdstat` (depende del sistema)
    * Escribir un script que muestre mensajes según la información de estos ficheros
    * Los mensajes son de la forma *RAID KO* o *RAID1 OK*
    * Lanzar el script en cron cada minuto
    * El mensaje se lanza al `syslog`, con el comando `logger`
* Corromponemos uno de los discos
* Subir el script a swad. Dentro del script, comentar la línea que añadimos al cron

## Proceso

* El profesor hace todo el proceso en Ubuntu
1. Mostramos el funcionamiento de `dmesg`
    * `dmesg -Hw`: muestra todo el contenido 
    * `-h`: human 
    * `-w`: wait, modo paginador
    * Muestra información de raid y del LVM
2. Mostramos información que se genera al conectar un USB en `dmesg`
    * En la máquina de VirtualBox, Configuración, USB: añadimos el USB para que sea visible a VirtualBox
        * Para ello, necesitamos tener el USB pinchado en el ordenador
    * Pincha un USB en el ordenador
    * Ahora el dispositivo es visible en la VM, se muestran mensajes en `dmesg`
    * Expulsa el usb con `dmesg` abierto
    * Se muestran mensajes en `dmesg` relacionado con el desconecte del usb
3. Comprobamos el sistema de ficheros `/var/log`
    * `cd /var/log & ls`
    * `wtmp`: binario con información de seguridad
    * `syslog`: log del kernel de linux y otros servicios
        * `rsyslogd`: si va con r es para aceptar logs remotos de otros ordenadores
            * Actúa como un servicio
            * Permite que otros procesos envien lineas de log en el fichero syslogd
    * `cd apache`: para entrar al log que usa apache
        * Apache no usa el syslog de todo el sistema
        * Sus logs los gestiona el propio servidor
        * Mysql tambien gestiona por si mismo el log
        * Otros servicios dejan esta responsabilidad sobre syslog o rsyslog, como ya hemos comentado
    * `cd /var/log; grep -i usb syslog`: busca usb en `syslog` usando la utilidad `grep`
4. Comprobamos el servicio syslog
    * `ps ax | grep -i syslog`: vemos que es un proceso en memoria con el que nos podemos comunicar para volcar logs
    * `syslog` se esta remplazando por servicios más modernos, como es el caso de `CentOS`
5. Miramos cómo funciona la rotación de logs
    * `less /etc/log/logrotate.conf`: configura aspectos de las rotaciones
    * `less /etc/log/logrotate.d/apache2`: configura la rotación propia de los logs de Apache
6. Miramos el funcionamiento de `who` lanzando este comando
7. Trabajamos sobre el sistema de ficheros `/proc`
    * `ls proc`: los números son los procesos que se estan ejecutando
    * `less proc/cpuinfo`: muestra información de la CPU
        * En este fichero se apoya la utilidad `lscpu`
    * `less loadvg`: carga media del sistema
    * `less stat`: estadisticas de cpu
    * `less /proc/version`: información del sistema operativo
        * En esto se basa `uname -a`
8. Directorio `/proc/sys`
    * En este directorio podemos modificar parámetros del sistema
    * `cd proc/sys/net/ipv4`: vamos a cambiar parámetros del networking 
    * `less ip_forward`: configurando esto podemos configurar un sistema como router, es una práctica bastante común
        * Coge un paquete de una tarjeta de red y lo saca por otra tarjeta de red
        * Puedo modificar en caliente : `echo 1 > ip_forward`
9. Usamos el directorio `/proc/sys` para cambiar el *hostname*
    * `hostname`: muestra el nombre de la máquina
    * `cd proc/sys/kernel ; echo prueba > hostname`: cambio el nombre usando el sistema de ficheros
    * `hostname`: veo que he cambiado el nombre del sistema, a nivel de kernel
* Esta forma de trabajar es útil a nivel de depuración
    * Cambiamos parámetros del kernel de forma temporal y de forma dinámica
    * No hay que cambiar configuraciones
    * Antiguamente había que recompilar todo el kernel para hacer visibles los cambios
    * Modificamos en caliente y vemos que pasa
    * `sysctl` hace cambios en caliente tambien. Ademas muetra informacion de las variables del kernel
10. Usamos `systctl` para hacer cambios en caliente, sin modificar `/proc/sys` manualmente
    * `systctl -w kernel.hostname='adios'`: cambiamos el hostname
    * `hostname`: vemos que se ha cambiado el nombre
    * `systemctl -a`: nos muestra todos los parámetros que podemos modificar
    * `systemctl -a | grep -i kernel`: buscamos todos los parámetros del kernel que podemos cambiar
    * Si quiero el cambio de forma permanente, sin recompilar el kernel:
        * vi /etc/systctl.conf: ponemos el comando formato systcl
            * Habría que reiniciar la máquina
            * En el arranque del systema, pasa todos los comandos a sysctl y los lanza
11. Comando `watch`
    * `watch -n 2 cat /proc/sys/stat`: lanza cada dos segundos el comando especificado
    * `watch -n 2 cat /proc/sys/meminfo`: lanza cada dos segundos el comando especificado
    * Se usa mucho para monitorizar de forma rápida
12. Trabajamos con `cron`
    * `ps ax | grep -i cron`: comprobamos que el servicio está corriendo
    * Tenemos los directorios `/etc/cron.{daily, hourly, ...}`: para configurar cron a un determinado periodo
        * Por ejemplo, tenemos el script `/etc/cron.daily/logrotate`
    * `/etc/crontab`: el crontab principal a nivel de sistema
        * Podemos editarlo con un editor
    * `crontab -e`:
        * Edita el crontab, pero a nivel de usuario
        * Además, así se permite editar el crontab sin tener privilegios
        * Aquí no se admite parametro usuario, a diferencia que en `/etc/crontab`
13. Prueba de que la tarea funciona
    * Usaremos el comando `logger`, que añade mensajes al `syslog`
    * `vi /etc/crontab`: `* * * * * * root echo "HOLA" 2>&1 | logger -t isep3l1`
    * Esto esta muy mal hecho, no hacer scripts directamente sobre cron, no guardo lo anterior
    * `vi ~/monitor`: sobre este script haríamos la tarea

        ~~~bash
        #!/usr/bin/env bash 

        # logger -t : marca con una etiqueta el mensaje
        # La etiqueta por defecto es el usuario que invoca al comando logger
        echo "HOLA" 2>&1 | logger -t isep3l1
        ~~~

    * `which env`: truco si se me olvida el path de `env`
    * En dos terminales ejecutamos:
        * `tail -f /etc/syslog ||| ~/monitor`: vemos que funciona correctamente
    * Editamos el crontab para añadir nuestro script
        * Como usuario normal: `crontab -e` : `* * * * * /home/david/monitor`
    * Con esto hemos ganado segregacion de responsabilidades
        * No lleno de basura `cron`
        * Es muy fácil de depurar el script (lo puedo ejecutar a mano)
    * Para simular la caida del disco en caliente: `mdadm --set-faulty`: para marcar el raid como fallido

--------------------------------------------------------------------------------

# Lección 2 -- Monitorización: top y Zabbix

## Teoría previa

### TOP

* **Información que muestra**:
    * Primer bloque:
        * Primera linea:
            * Hora
            * Uptime (cuánto tiempo lleva el sistema corriendo)
            * Numero de usuarios activos
            * Carga media o load average: muy importante en la evaluación de prestaciones de servidores
                * Estándar de facto
                * Es muy importante entender el funcionamiento
                * Media exponencial decayente en el último minuto, últimos 5 minutos y últimos 15 minutos
                * Para un cpu que solo puede lanzar un proceso, solo se puede llegar a un valor de 1.00
                * Si se supera este valor, significa que tenemos procesos en cola, y sabemos la longitud de la cola
                * En realidad, se mide la longitud de la cola
                * Una CPU saturada permanentemente por encima del 75% es peligroso por sobrecargas eléctricas, calentamiento excesivo
                * Al trabajar con medias, podemos saber si hay estrés continuado y obviar picos puntuales
        * Segunda línea: información de hebras y procesos
            * `sleeping`: muestra las hebras bloqueadas, normalmente por entrada/salida, por cambios de contexto
            * `stopped`: los procesos parados por control-z
            * `zombie`: proceso padre spawnea hijos, acaban la tarea para morir dando el resultado (exito o codigo de error). Cuando no se elimina por algún motivo, se queda zombie. (zombie se queda entre comunicar la finalización y el eliminado del sistema). Un valor alto suele indicar que no se hace, tras un `fork`, un `join` bien hecho
        * Tercera linea: estadísticas del cpu
            * `us`: porcentaje en codigo usuario
            * `sys`: porcentaje en llamadas al sistema
            * `ni`: procesos cambiados el niceness
            * `id`: carga ociosa de la cpu
            * `wa`: tiempo bloqueado
            * `hi, si`: interrupciones hardware, software
            * `st`: stolen time, ciclos que se pierden por el hipervisor (solo funciona para hipervisores nativos de linux como KVM)
        * Cuarta linea: estadísticas de memoria ram
            * Lo último, buffer/cache es lo reservado para cachear entradas salidas. Su tamaño es dinamico, depende de la historia que lleve el sistema. Se puede considerar espacio libre porque se puede reclamar
        * Quinta linea: memoria swap
            * La última cifra es especial. Da idea sobre el maximo de memoria que puede reclamar un proceso puede reservar ahora
    * Segundo Bloque (tabla interactiva):
        * Procesos:
            * Ordenados por PID (Process ID), no significativo
* Teclas útiles:
    * Interrogación muestra la ayuda
    * h: muestra ayuda
    * tecla desconocida: cambia entre threads y tasks (creo que tasks muestra procesos y no hebras)
    * e: cambia las unidades
* **Características**
    * Es un agregador de información. Usa otras utilidades o el sistema en memoria `/proc` para localizar los valores. Cosume utilidades como:
        * `uptime`: toda la primera línea
        * `vmstat`: información sobre memoria virtual
        * `free`: estadísticas sobre memroia principal y swap (cuarta y quinta línea)
    * Herramienta interactiva para monitorizar un sistema, en la consola sin interfaz gráfica
        * Por tanto, es útil para conexiones remotas
* Alternativas:
    * `htop`: versión mejorada, con mejor interacción con el usuario y colores
    * `gtop`: programada en `go`, tiene dependencias con `node.js`

### Monitores avanzados

* **Motivación**:
    * Buscamos una herramienta más potente que los comandos básicos antes vistos
    * `top` no nos da un histórico, por ejemplo
    * Los monitores que vamos a ver permiten realizar previsiones de necesidades, para que no se caiga nuestro sistema en puntos picos que podemos preveer

### Zabbix

* **Características**:
    * Código abierto
    * Arquitectura no trivial como *Munin*
    * Usamos la versión 3.4, la última es la 5.0, que sigue en desarrollo
    * Hace llamadas a los plugins de los distintos elementos que queremos medir (tamaño de bases de datos, temperaturas, cargas...)
    * Puede trabajar con estrategia pull o push
        * Por defecto estrategia pull
    * Componentes:
        * Servidor: núcleo al que mandamos los datos de telemetría. Mete los datos en una base de datos relacional (en nuestro caso, `mysql`)
        * Agentes: procesos que corren en cada uno de los sistemas que queremos monitorizar (apache, mysql). 
            * Esta en background, lo llama el server para que le mandemos las métricas. 
            * Está constantemente tomando medidas, tiene capacidad para algun tiempo
        * Proxy: cuando tenemos muchos equipos, un servidor central no puede con todo. 
            * Un proxy se pone en medio y actua como un servidor local que muestrea grupos
            * Así el servidor central, puede hacer pull al proxy directamente
        * Get: lo que necesitamos para el ejercicio final de hoy. 
            * Es un GET REST
            * Muy útil para depurar problemas de comunicación agente-servidor, es básicamente para lo que se usa
            * Sirve también para crear un cliente propio que use una API REST
* **Estrategias de monitorización**:
    * Pull
        * Muestreo clásico
        * Primera estrategias en desarrollarse
        * Voy sistema a sistema preguntando por los valores que queremos saber
        * Se hacen los pull en secuencia
    * Push
        * Más novedosa
        * Los sistemas mandan la información al monitor de forma autónoma
        * El monitor retoma los datos cuando quiere
        * El servidor monitor está menos cargado, se comunican los eventos (analogo a las interrupciones), cuando pasa algo interesante
        * No se tiene control sobre el momento de llegada de la información
            * Se puede saturar el servicio central de monitorización
* **Alternativas a zabbix**:
    * Munin
    * Prometheus 
        * Estrategia pull (muestreo) en vez de push
        * Programado en `go`, por lo que corre muy rápido
    * PIG: prometheus + InfluxDB + Grafana
        * InfluxDB es una base de datos programada en `go`, especializada en ser *Time Series Database*
        * Graphana para mostrar los resultados

### Interfaz de Zabbix (página web)

* Pestaña `Monitoring`
    * `HOSTS`: 
        * Muestra los equipos que estamos monitorizando
        * Los agentes usan el puerto `10050`
    * `LATEST DATA`: 
        * Podemos hacer filtros, grupos de máquinas.
    * `GRAPH`
        * Muestra los gráficos de los filtros que hayamos creado
        * La gráfica no se refresca en tiempo real, para evitar sobrecargas en el sistema. 
        * Hay un cierto tiempo entre muestreo, que se puede configurar desde zabix server CONFIGURATION, desde el fronted, variable por variable, máquina por máquina
        * Un cambio en un servidor puede provocar que haya un punto muerto en el que no sea visible el cambio desde zabbix, motivado por el tiempo entre muestreos
* En entornso de producción, siempre se tiene una pantalla o proyección con el *dashboard* para vigilar el correcto funcionamiento del sistema

## Proceso

### TOP

1. Nos conectamos por SSH a nuestra máquina Ubuntu, en dos terminales distintas
2. Jugamos con top
    1. `apt install top`
    2. `apt install stress`: para estresar algo el sistema y ver cómo varía en top
    2. `top`: para lanzar el programa
    4. `stress --cpu 8 --io 4 --vm 2 --v-bytes 128M --timeout 10s`: lo obtenemos de la primera línea del man de `stress`

### Zabbix

1. Instalamos el agente Zabbix en CentOS
    * Siguiendo el tutorial de [](https://www.zabbix.com/documentation/3.4/manual/installation/install_from_packages/rhel_centos)
    1. `sudo rpm -ivh https://repo.zabbix.com/zabbix/3.4/rhel/7/x86_64/zabbix-release-3.4-2.el7.noarch.rpm`: para añadir el repositorio de zabbix
    2. `sudo yum install zabbix-agent`: instalación del agente
    3. Hay un BUG en esta versión que tenemos que solucionar:
        * La solución al BUG está documentada en [](https://bugzilla.redhat.com/show_bug.cgi?id=1349998)
        * `sudo yum install policycoreutils-python`: instala `audit2allow`, necesario para corregir el BUG
        * `ausearch -c 'zabbix_agentd' --raw | audit2allow -M my-zabbixagentd`: cambia el tamaño de algo que limita a Zabbix
        * `semodule -X 300 -i my-zabbixagentd.pp`: creo que hace el cambio permanente
    4. Iniciamos el servicio y comprobamos que funcione correctamente
        * `sudo systemctl enable zabbix-agent`
        * `sudo systemctl status zabbix-agent`
    5. Configuramos el agente para que se comunique con el servidor Ubuntu
        * `sudo vim /etc/zabbix/zabbix_agentd.conf`
        * `Server=192.168.56.101`: estamos indicando el servidor con el que se tiene que comunicar el agente
        * `sudo systemctl restart zabbix-agent`: para que los cambios se hagan efectivos
2. Instalamos el servidor Zabbix en Ubuntu
    * Siguiendo el tutorial de [](https://www.zabbix.com/documentation/3.4/manual/installation/install_from_packages/debian_ubuntu)
    1. Añadimos el respositorio de Zabbix al sistema
        * `wget https://repo.zabbix.com/zabbix/3.4/ubuntu/pool/main/z/zabbix-release/zabbix-release_3.4-1+xenial_all.deb`
        * `sudo dpkg -i zabbix-release_3.4-1+xenial_all.deb`
        * `sudo apt update`
    2. Instalamos los componentes necesarios en la máquina
        * `sudo apt install zabbix-server-mysql`
        * `sudo apt install zabbix-frontend-php`
        * **Observación**: no instalamos `zabbix-proxy-mysql` porque al tener solo una máquina monotorizada, no hay suficiente carga de muestreo de datos como para que merezca la pena el `overhead` de correr el proxy sobre la misma máquina que el servidor
3. Creamos la base de datos para el servidor como se indica en [](https://www.zabbix.com/documentation/3.4/manual/appendix/install/db_scripts#mysql)
    * `sudo mysql_secure_installation`: damos contraseña al usuario `root` de la base de datos
    * `sudo mysql -u root -p`: para acceder a la base de datos como root
    * `mysql> create database zabbix character set utf8 collate utf8_bin;`: se crea la base de datos
    * `mysql> grant all privileges on zabbix.* to zabbix@localhost identified by 'ise2020';`: se dan los permisos necesarios, creando a la vez el usuario `zabbix` con contraseña `ise2020`
    * `mysql> quit;`: salimos de la base de datos
    * `zcat /usr/share/doc/zabbix-server-mysql/create.sql.gz | mysql -uzabbix -p zabbix`: importamos los datos (datos y esquemas de tablas)
4. Configuramos Zabbix Server en Ubuntu
    * `vi /etc/zabbix/zabbix_server.conf`

~~~conf
DBHost=localhost
DBName=zabbix
DBUser=zabbix

# El unico cambio que hacemos
# Descomentamos e introducimos la contraseña
DBPassword=ise2020
~~~

5. Iniciamos el servicio
    * `sudo systemctl enable zabbix-server`
    * `sudo update-rc.d zabbix-server enable`
    * `sudo systemctl status zabbix-server`: comprobamos el estado del servicio
6. Editamos la configuración de apache para indicar la zona horaria
    * `sudo vim /etc/apache2/conf-enabled/zabbix.conf`
    * Descomentamos las líneas `php_value date.timezone Europe/Riga`
    * `sudo systemctl restart apache2`: para que se cargue la página web de Zabbix
7. Accedemos a `192.168.56.101/zabbix`
    * Completamos el `installation wizard`
    * Accedemos al dashboard con:
        * User: `Admin`
        * Password: `zabbix`
8. Abrimos los puertos de CentOS
    * El agente usa el puerto 10050
    * `CentOS> firewall-cmd --zone=public --add-port=10050/tcp`: para abrir el puerto
    * `CentOS> firewall-cmd --runtime-to-permanent`: para que el cambio hecho en `runtime` sea permamente
    * `Ubuntu> telnet 192.168.56.102 10050 `: muestra que el destino es alcanzable
9. Para ver, de forma instantánea, que los cambios se han realizado correctamente, usamos `zabbix_get`
    * El profesor saca estos comandos de los ejemplos del `man`
    * `Ubuntu> apt install zabbix_get`
    * `Ubuntu> man zabbix_get`: para ver el ejemplo y usarlo
    * `Ubuntu> zabbix_get -s 192.168.56.102 -p 10050 -k "system.cpu.load[all, avg1]"`: resuelto sin mensaje de error, está bien, aunque en el dashboard se pueda ver que sigue caido
    * En [](https://www.zabbix.com/documentation/3.4/manual/config/items/itemtypes/zabbix_agent) se pueden consultar los items que podemos monitorizar con `zabbix_get`
10. En dashboard:
    * Add Widget -> Simple Graphic -> {HTTP SERVICE, SSH SERVICE}
    * Con `systemctl` tiramos y levantamos el servicio
    * Vemos cómo varía la gráfica

## Tarea

* Instalar `zabbix` en Ubuntu y `CentOS`
* Monitorizar `http` y `ssh` sobre `CentOS`
* Se puede usar la grafica para indicar la caída, o el dashboard, con eso le es suficiente
* Documento entregable:
    * Un par de páginas mostrando capturas del dashboard
    * Comando para matar el servicio
    * Captura en la que zabbix muestra que ha caido el servicio
    * Comando subiendo el servicio
    * Captura dashboard viendo que se ha levantado
* Hasta final de curso que para hacer el ejercicio
* Para hacer la instalación, mirar tutorial de la instalación de zabbix, versión XENIAL XERUS LTS

--------------------------------------------------------------------------------

# Lección 3 -- Ansible

## Teoría previa

* Opciones antes de los **gestores de configuración**
    1. Configuración totalmente manual
    2. Modificar el instalador para que se haga una instalación dada automáticamente
    3. Imagenes virtuale
        * Un ejemplo es *Vagrant*, que es independiente de la tecnología de virtualización
        * Por debajo puede usar VirtualBox, VMware, AWS...
        * A partir de este punto, se usa otra tecnología como scripts, Chef, Puppet...
* **Ansible**:
    * Proyecto desarrollado por *RedHat*
    * Es un producto de **gestión de configuración**
        * Tomamos un equipo y queremos llevarlo a una configuración determinada
            * Configuración de red
            * Configuración de usuarios
            * Configuración de privilegios
            * Configuración de paquetes
            * Configuración de servicios
        * Definimos la configuración en ficheros de código
            * No dependemos de un fabricante concreto
            * Podemos almacenar configuraciones en un gestor de versiones como *Git*
        * Formatos:
            * XML -> en desuso
            * Yaml
            * JSON
    * Se lanza contra un servidor y lanza comandos que configuran el equipo
    * Se pueden paralelizar
    * Usa *playbooks*
        * Ficheros de texto que definen 
    * Solo lo vemos en el contexto de lanzar comandos, no configuramos todos los servidores
    * Ansible usa `ssh`, así reducimos las vulnerabilidades a `ssh`, como las aplicaciones modernas
    * Ansible necesita que los servidores corran `python` para poder lanzar los comandos
    * Ansible está programado directamente sobre `python`
    * Competidores
        * Puppet
        * Cheff
        * Terraform: está más orientado a la nube
            * Permite definir clouds completos
            * Define cómo se hacen los despliegues
    * **Inventario**: lista de ordenadores con los que vamos a trabajar
    * Se puede configurar nuestros propios módulos para hacer llamadas
* **Configuración de servicios**:
    * Programación y configuración declarativa: decimos cómo queremos acabar pero no cómo se llega al estado final
    * En el caso de `systemctl`no se usan verbos `systemctl`, se usan adjetivos como `started`:
        * Programación declarativa
        * Indicamos el estado al que queremos llegar
        * Si decimos started, y ya esta arrancado, ansible no hace nada
        * Si fuese procedural, indicaríamos los comandos que queremos que se lance

## Proceso

### Primeros pasos con Ansible

* Partimos de dos máquinas Ubuntu y de una máquina CentOS
1. Instalamos ansible en nuestras máquinas
    * `Ubuntu1> apt install ansible`
    * `Ubuntu2> apt install ansible`
    * `CentOS> yum install centos-release-ansible-29`
    * `Ubuntu1> vi /etc/ansible/hosts`: vemos ejemplos de los inventarios
    * `Ubuntu1> vi /etc/ansible/ansible.cfg`: configuraciín de ansible
2. Añadimos hosts a nuestro sistema para luego poder usarlos en inventarios
    * `sudo vim /etc/ansible/hosts`
        * `UbuntuServer ansiblehost=192.168.56.101`
        * `CentOSServer ansiblehost=192.168.56.102`
        * Esto no haría falta si los servidores fuesen alcanzables desde nuestra DNS
        * `ansible all -m ping`: para comprobar la configuración
            * `-m`: indica un módulo a usar
3. Algunos ejemplos
    * `ansible all -a "uname -a"`: se ejecuta un comando concreto
        * `-a`: indica argumentos, en este caso, sin usar módulo, el comando a lanzar
    * `ansible -m ping UbuntuServer -u sergio`
        * `-u`: para indicar el usuario
    * `ansible all -m ping `: lanza el ping a todos los host definidos en `/etc/ansible/hosts`
    * `ansible all -m ping -a "data='buenos dias'"`:
        - `-a`: parámetros que se pasan al comando
        * `data`: define la respuesta que da el ping. Puede ser un comando
    * `all`: hace las llamadas en paralelo, con hebras de `python`
4. Podemos añadir los usuarios por defecto en `/etc/ansible/hosts`
        * `UbuntuServer ansiblehost=192.168.56.101 ansible_user="sergio"`
        * `CentOSServer ansiblehost=192.168.56.102 ansible_user="sergio"`
5. Podemos hacer la configuración a nivel de usuario
    * Editando el archivo `~/.ansible.cfg`: sobrescribe la configuración del sistema
    * Aquí no podemos definir un inventario, solo configuración de Ansible
    * En esta configuración, definimos un archivo local para los hosts y el inventario:

**~/.ansible.cfg**
~~~
[defaults]
inventory = ~/.ansible/hosts
~~~

**~/.ansible/hosts**
~~~
UbuntuServer ansiblehost=192.168.56.101 ansible_user="sergio"
CentOSServer ansiblehost=192.168.56.102 ansible_user="sergio"

[lamp]
UbuntuServer
CentOSServer
~~~

6. Hacemos pruebas con esta configuración local
    * `ansible lamp -m ping`: lanzamos un ping contra el grupo `lamp`
    * `ansible all -m shell -a 'echo $PATH'`
        * Muestra el `path` de cada uno de los equipos
        * Toma la configuración de acceso remoto: `env vars` de `/etc/bash_profile`
    * `ansible all -a 'echo $PATH'`: hace lo mismo, porque ansible usa por defecto el modulo shell
        * Mala idea porque VirtualBox toma el $PATH del host, puede dar lugar a confusiones
    * `ansible all -a "uname -a"`: muestra que versión de Linux estamos corriendo

### Escalada de privilegios

* Nos conectamos a un usuario distinto de `root` pero que puede ejecutar comandos `sudo` sin introducir contraseña
    * De otra forma, `ansible --become` falla por no poder introducir la contraseña
* Modificamos el usuario `sergio` tanto en `Ubuntu` como en `CentOS` para que pueda hacer esto
1. Configuración en CentOS
    * Editamos `/etc/sudoers` para que el grupo `wheel` no use contraseña en comandos con `sudo`
        * `sudo su`: para ser root
        * `visudo -f sudoers`: única forma de editar este archivo
        * Comentamos la línea de `wheel` y descomentamos la siguiente línea:
            ~~~
            #%wheel ALL=(ALL)       ALL
            %wheel  ALL=(ALL)       NOPASSWD: ALL
            ~~~
    * Añadimos el usuario al grupo `wheel`, en el que hemos configurado que no se pida contraseña
    * `usermod -a -G wheel sergio`:
        * `-G`: indica que vamos a operar sobre los grupos de un usuario
        * `-a`: **importante**, para añadir grupos, y no para cambiar
            * De otro modo, sacaríamos al usuario de sus grupos previos
2. Configuración en Ubuntu: es repetir el proceso de `CentOS`
    * Hacemos la misma configuración de `/etc/sudoers`
    * `groupadd wheel`: creamos el grupo `wheel` porque no existe
    * `usermod -a -G wheel sergio`
3. Comprobamos la escalada
    * `ansible all -m ping --become`

### Configuración de servicios

1. Entramos en las páginas web para ver como cambian: 
     * Ubuntu: `192.168.56.101/index.php` 
     * CentOS: `192.168.56.102/index.php` 
2. Antes de hacer la escalada:
    * `ansible cenise -m service -a "name=httpd state=stopped"`: falla por acceder como usuario no privilegiado
    * `ansible cenise -m service -a "name=httpd state=started"`: no falla, porque como ya está `started`, no se ejecuta ningún comando
        * Esto es lo importante del estilo declarativo
        * Para un millón de servidores, saltarse la ejecución de comandos tiene mucho impacto en tiempo
3. Configuración básica de servicios
    * `ansible cenise -m service -a "name=httpd state=stopped" --become`
        * Vemos que la página web, que antes estaba operativa, ahora está caida
        * Usamos el módulo `service` para no preocuparnos del gestor de servicios que use el sistema
    * `ansible all -m service -a "name=httpd state=stopped" --become`
        * Falla en Ubuntu, no se llama httpd, se llama apache
        * Solucionamos las diferencias con variables en nuestra configuración local de los hosts
        * Simplemente añadimos una **variable**, su **nombre lo elegimos** nosostros


**~/.ansible/hosts**
~~~
UbuntuServer ansiblehost=192.168.56.101 ansible_user="sergio"
CentOSServer ansiblehost=192.168.56.102 ansible_user="sergio"

[lamp]
UbuntuServer apache_service="httpd"
CentOSServer apache_service="apache2"
~~~

4. Configuración a través de variables de configuración:
* `ansible lamp -m service -a "name={{apache_service}} state=stopped --become"`
    * Vemos que las dos páginas web han caído

## Tarea

* No se entrega, es para prácticar
* Reiniciar todas las máquinas que estamos configurando
* Con el comando reboot
