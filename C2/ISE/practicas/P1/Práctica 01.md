---
title: Práctica 1 - Ingeniería de Servidores
author: Sergio Quijano Rey
date: 13/02/2020
---

# Presentación

* Profesor:
    * David Palomar
    * `dpalomar@ugr.es`
    * No hace tutorías fuera de los jueves de 17.30h a 19.30h. Enviarle un correo para pedir tutorías porque es profesor asociado y no se queda en la facultad
* Seguir la clase con nuestros ordenadores, después en casa repetir la práctica
* Se hacen exámenes de tipo test de cuatro opciones para evaluar las prácticas: 
    * Hay que hacer las prácticas más que estudiar.
    * Se hacen en 10 minutos
    * 20 preguntas de tipo test
    * Cada final de las prácticas
    * Están marcadas
    * En la semana del seminario, se hacen dos exámenes en el mismo día, aunque el examen del seminario es una tontería
* Cada falta no justificada es un punto de penalización sobre 10
* Hay que enseñar la práctica, de forma aleatoria
* Si se piensa que no se va a aprobar, se puede pedir realizar un trabajo adicional que suma hasta un punto

# Práctica 1

## Distribuciones Linux

* Linux es el kernel
* Se monta una colección de paquetes que conforma la distribución
* RedHat vive de vender licencias y soporte de su `RedHat Enterprise`

## Sistema de virtualización

* El software de virtualización corre sobre el sistema operativo, lo vamos a llamar **hipervisor**
    * Crea una abstracción, creando un hardware ficticio
    * Sobre el sistema virtual se instala otro sistema operativo
    * Sobre el último sistema operativo, corren las aplicaciones que vamos a usar
* Hay otras formas de virtualizar, la anterior es la más clásica
* Por ejemplo, correr el hipervisor directamente sobre el hardware, no sobre el sistema operativo de primer nivel
* VPS o Virtual Private Server
* Esto da elasticidad a la hora de escalar los servicios
    * Pasamos de coste fijo a coste variable
    * El proveedor optimiza los recursos que tiene
* Existen hipervisores que restrigen el software que se puede correr sobre ellos, con el fin de mejorar las prestaciones
* Llamamos anfitrión o *host* al hardware y a su sistema operativo. A todas las capas por encima, invitado o *guest*
* En virtualización se tira de línea de comandos para consumir los menos recursos posibles
* Ejemplo: Amazon Web Services (*AWS*) y *Elastic Cloud Compluting* o *EC2*
    * 1 vCPU = medio core i7 (un hiperthreading)
    * *AMI* o *Amazon Image*: por ejemplo Ubuntu Server
* Vamos a intentar emular lo que hacen estas compañías

### Máquina virtual vs Contenedor

* Máquina Virtual significa virtualizar hardware con un hipervisor
* Los contenedores no virtualizan el hardware
    * Corre directamente sobre el sistema operativo subyacente
    * Gracias a los `namespace` de Linux, ocultan los:
        * Espacio de memoria
        * Espacio de ficheros
        * Espacio de CPU
        * Espacio de red
    * Dan una solución de encapsulado, un sandbox
* Lo mejor de los contenedores es el encapsulado, no la virtualización que no la realiza. Todas las dependencias de un programa están autocontenidas, y esto evita muchos problemas
* Los programas que tienen que correr sobre contenedores tienen que estar diseñados para correr sobre contenedores, por ejemplo la máquina virtual de Java
* No se pierde eficiencia en memoria ni en CPU, algo en red (porque sí que se virtualiza)
* En MAC, se corre Linux virtualizado y sobre ello `Docker`

## Cloud Computing

* Tipos de servicios:
    * IaaS: infraestructure as a service: virtualizar red, almacenamiento, servidor y sistema operativo
    * PaaS: platform as a service: LAMP, por ejemplo. Se ofrece sistema operativo y plataforma de desarrollo. Nos olvidamos de la administración de los servicios
    * SaaS: Software as a service
    * FaaS: Function as a service: se ofrecen APIs a las que llamamos. *Serverless*
* `OpenStack` y `ProxMox` para clouds privados

# Lección 1

## Conceptos iniciales

* Ubuntu Server, Raid1, LVM, Cifrado LUKS
* **RAID**: redundance array of independent disks
    * RAID0: Stripping
        * Coges un disco de 1GB, otro de 1GB, y se simula un disco de 2GB
        * Sectores distribuidos
        * Permite leer en paralelo para discos consecutivos
        * Problema: si se rompe uno de los discos, perdemos todo el almacenamiento
    * RAID1: Mirror
        * La información se guarda redundantemente
        * La capacidad de almacenamiento es el del disco de menor capacidad
        * Son sistemas muy robustos
        * Se suele usar para arranques del sistema operativo
    * RAID5: 
        * Se usa un código de redundancia cíclica ó *CRC*
        * El disco duro que vemos es el de los $n-1$ discos que tenemos
        * El último disco se usa para que si un disco cae, se pueda recuperar su información
        * En realidad la información se distribuye entre todos los discos del RAID, pero en números es lo ya comentado
        * Da cierta robustez a un coste no muy alto
        * Usados en bases de datos, repositorios de archivos, empresas convencionales...
    * Dos implementaciones:
        * Hardware
            * El ordenador tiene una tarjeta que pincha a los varios disco para administrarlos
            * El gestor de RAIDS está en la BIOS, desde hardware
            * Por tanto, es más eficiente
            * El sistema operativo no se entera del RAID, no sabe los discos o la arquitectura que haya por debajo. Sirve para cualquier sistema operativo, por ejemplo Windows. Esta es la principal ventaja
            * Más caro, aunque hoy en día esta opción es batante barato
        * Software
            * Se consume CPU para las operaciones sobre el disco
            * Es visible a nivel de sistema operativo, se puede romper el RAID
            * Más barato
* Se crea un dispositivo `/dev/md0` compuesto por
    * `/dev/sda`
    * `/dev/sdb`
* Si escribo sobre `/dev/sd?` me cargo el RAID, hay que escribir sobre `/dev/md0`, **MUY IMPORTANTE**

## LVM

* Logic Volume Management o Sistema de Volúmenes Lógicos
* *Physcal Volume* o *PV*:
    * Puede ser una partición de un disco duro, un RAID, un volúmen virtual...
* Agrupamos nuestros *PV* sobre un *Grupo de Volumen*
    * El almacenamiento se va acumulando
    * Definimos volúmenes lógicos sobre el grupo
        * Si tenemos un RAID y un disco duro, no sabemos que información va al RAID y cuál no
        * Con volúmenes lógicos podemos controlar esto
        * También para discos mecánicos o díscos sólidos
* Nuestro volúmenes lógicos serán
    * nombre -> punto de montaje: descripción, formato
    * root = /: todo el espacio que quede, EXT4
    * boot = /boot: para arrancar el sistema, 200-400 MB, EXT4, que es transaccional
    * swap = intercambio, no se monta: asignar tanto como memoria principal RAM se tenga, área de intercambio
    * home = /home: elegimos una cantidad considerable, EXT4

## Cifrado

* Cifraremos con LUKS root, home, swap
    * Ciframos root porque en var tenemos todos los archivos variables que pueden tener información sensible
    * Home podríamos no cifrarlo
    * En RAM no se cifra todo, si se intercambia con SWAP, va cifrado
    * Boot no se cifra porque si no no se arrancaría Linux
    * Uso como contraseño en los tres volúmenes `ser`
    * Implica estar físicamente en la terminal del ordenador para arrancar el ordenador, aunque hay unos `initrd` que permiten conectarse por red (telnet)
* Hay legislación sobre el cifrado de información personal de los usuarios con los que trabajemos

## **Proceso**

* Lo primero es hacer unas pequeñas tonterías sobre el idioma, configuración de teclado, región horaria...
* Llegamos al particionado del disco en el que pasamos a modo manual
    1. Crear RAID
    2. Crear el grupo LVM
    3. Crear vólumenes lógicos LVM sobre el grupo LVM
        3.1. Boot 400MB
        3.2. Swap 1024MB
        3.3. Home 1GB
        3.4. Root
    4. Cifrar los volúmenes lógicos
    5. Formatear los vólumenes lógicos
        5.1. Elegir el formato de archivos de cada partición
        5.2. Elegir el punto de montaje de la partición
    6. Elegir tipo de actualizaciones: sin actualizaciones del sistema
    7. Elegir los programas a instalar: sistema básico
* Hago la elección de instalar GRUB en `/dev/sda`: **PREGUNTAR AL PROFESOR SI ESTO ESTA BIEN**
* **Configuración**:
    * Definir lo que rodea el sistema
    * Propiedades con las que funciona
    * Librerías
    * Dependencias
    * Usuarios y permisos
* La configuración y el sistema se degrada el sistema temporalmente
* Instalación del GRUB
    * `/dev/sd<letra>`: sd significa SATA Disk
    * Se instala el GRUB sobre `/dev/sda`: en la BIOS se busca el primer disco, en la partición primera el sector de arranque para arrancar el sistema
    * Al tener un RAID software, si se rompe el primer disco no podríamos arrancar el sistema aunque tengamos RAID1
* **Arranque de un ordenador** o arranque IBM-PC:
    * IBM abarató costes gracias al sistema de arranque
    * En la BIOS tenemos una memoria ROM que almacena las primeras instrucciones que se cargan en memoria RAM
    * Esas instrucciones van a `/dev/sda` y se van al sector 0 o *Master Boot Record* o *MBR*. Este sector, que es uno solo, tiene instrucciones en x86 que es el código de arranque
    * Si copiamos ese sector sobre un USB en la *MBR*, tenemos un USB de arranque o LiveUSB, como curiosidad
    * El código de arranque busca la partición primaria del disco y carga el kernel del sistema operativo, que se encuentra en `/boot`
    * Por tanto, si ciframos `boot`, el código de arranque no tiene forma de descrifrar `/boot` y no arranca el sistema
* Ficheros:
    * `vmlinuz<version>`: contiene el kernel 
    * `init.rd`: contiene archivos que se monta como un sistema de ficheros cargado en memoria principal. Sirve para drivers
* Aquí aparece una técnica muy conocida que es montar como sistema de archivos una zona de memoria principal. Los dispositivos en linux son archivos, así como recursos de red...
    * `/proc` no existe, está en memoria principal
* **Networking**
    * En la máquina disponemos de una tarjeta de red que es virtual, es creada por Virtual Box
    * Por defecto VirtualBox te da una tarjeta de red configurada en NAT
    * NAT te deja acceder a internet de forma directa. NAT te conecta al HOST que deja acceder a la red a la que tiene acceso el HOST.
    * El HOST está corriendo un servicio de NAT al que se conecta Virtual Box: Network Address Translation
    * NAT sirve para resolver problemas de agotamiento de direcciones IP
    * Algunos rangos de IPs derivadas
        * 10.0.0.0/24
        * 192.168.0.0/16
    * NAT corre directamente sobre el router
    * Por tanto, podemos sacar paquetes, pero no podemos meter paquetes al sistema, porque no hay forma de direccionar la máquina virtual. Tenemos la máquina virtual completamente protegida, no podemos ser atacados. Pero no podemos ofrecer servicios
    * NAT usa por tanto un esquema de *Whitelist*
    * El problema, no puedo conectar máquinas virtuales entre sí, ni conectar la máquina virtual con el host
* **Tipos de red que se usan en virtualización**:
    * **BRIDGE**: 
        * Se crea una tarjeta de red virtual, con MAC 
        * La parte de capas 1-3 de OSI se implementa en la tarjeta de red del HOST
        * Por tanto lecturas y escrituras de red virtual se hacen directamente sobre la real
        * Podemos sniffear el host desde el guest y viceversa
        * Así, ofrece directamente acceso a internet
        * Tenemos mismos recursos de red (se usa la tarjeta física) que el host
        * No tenemos la capa de seguridad de NAT
        * Máquinas en BRIDGE se ven entre ellas
        * Se usa cuando se quiere tener un acceso físico y real a las máquinas virtuales o por temas de eficiencia
    * **Host-Only**
        * A la máquina virtual le creamos una tarjeta virtual, y en el HOST creamos otra tarjeta virtual que es distinta a su tarjeta real
        * La tarjeta virtual del HOST se usa para otros Host-Only de otras máquinas virtuales
        * Por tanto, la tarjeta virtual solo se usa para comunicar la red virtual
        * No tenemos acceso a internet. Hace falta configurar en el router las entradas de la tabla de enrutamiento para dar acceso a las máquinas virtuales hacia afuera o viceversa
        * Lo frecuente en los operadores de cloud
        * Por tanto, usa un esquema de *white-listing*
    * **NAT**:
        * Ya lo hemos comentado algo por encima
        * Opción por defecto para VirtualBox
        * NAT te deja acceder a internet de forma directa. NAT te conecta al HOST que deja acceder a la red a la que tiene acceso el HOST.
        * El HOST está corriendo un servicio de NAT al que se conecta Virtual Box: Network Address Translation
        * NAT sirve para resolver problemas de agotamiento de direcciones IP
        * Algunos rangos de IPs derivadas
            * 10.0.0.0/24
            * 192.168.0.0/16
        * NAT corre directamente sobre el router
        * Por tanto, podemos sacar paquetes, pero no podemos meter paquetes al sistema, porque no hay forma de direccionar la máquina virtual. Tenemos la máquina virtual completamente protegida, no podemos ser atacados. Pero no podemos ofrecer servicios
        * NAT usa por tanto un esquema de *Whitelist*
        * El problema, no puedo conectar máquinas virtuales entre sí, ni conectar la máquina virtual con el host
    * Ahora vamos a definir una tarjeta *Host-Only* y la *NAT* que viene por defecto, así como la tarjeta virtual del *Host-Only* del HOST
        * Vamos a usar Host-Only para acceder por SSH a nuestra máquina virtual (tiene que funcionar el ping)
        * Tiene que salir la tarjeta de red al hacer `ifconfig -a` en el HOST
        * En la máquina añadimos una nueva tarjeta de red HOSTONLY
    * `/etc/network/interfaces`
    * A Host-Only no damos `DHCP`, asignamos nosotros a mano la IP
    * `ifup enp0s8` o restart con `/etc/init.d/networking restart`
    * Testeo host->virtual con `ping 192.168.56.10`
    * Testeo virtual->host con `ping 192.168.56.1`
* Para enseñar las practicas: lsblk o los pings de antes

--------------------------------------------------------------------------------

# Segunda Lección -- CentOS con particionado automático

* Motivación:
    * Gestionaremos LVM por línea de comandos
    * Suponemos que `/var` se nos ha llenado, y `/var` no tiene un volumen lógico solo para él
    * Hay que apagar el sistema, añadir un disco y asignarlo a `/var`
    * Suponemos que se vuelve a llenar, pero como tenemos `/var` en un volumen lógico lo vamos a poder redimensionar
* Le asignamos la misma tarjeta *Host-Only* de la primera lección, así nos podemos comunicar con los servidores de Ubuntu
* Podemos acceder a CentOS como root directamente
* `ip link show`
* Partimos sin acceso a internet porque CentOS no lo configura por defecto
* `cd /etc/sysconfig/network-scripts`
* `vi ifcfg-enp0s3`: ponemos `ONBOOT=yes`
* `ifup enp0s3`
* `ip link show`
* `ip addr` para mostrar el networking
* Copiamos el archivo ifcfg-enp0s3 y ponemos el nombre de la interfaz localhost
    * Borramos hasta NAME=enp0s3 y lo ponemos a 8
    * Nos quedamos con TYPE, NAME, DEVICE, ONBOOT=yes, IPADDR = 192.168.56.20, NETMASK
* Levantamos con ifup enp08
* Hacemos ping para comprobar
* `lsblk`
    * Una partición para `/boot` en `/sda1`
    * En `/sda2`: volumen para root y para swap
* `pvdisplay`: pv de physical volume
* `vgdisplay`: vg de volume group
    * Free a 0: tenemos todo el grupo asignado a volumenes logicos
* Volumenes logicos hace `/dev/<grupo_volumenes>/<volumen_logico>`
* `/dev/mapper/<grupo>-<volumen>`
* `dd if=/dev/zero of=/var/log/zeros.raw bs=1024k count=1024`: para ocupar todo el 
    * `/dev/zero` devuelve ceros todo el rato
    * `/dev/random`: devuelve aleatorios, pero es mejor usar `/dev/zero`. En máquinas virtuales puede no funcionar porque la aleatoriedad depende de un dispositivo físico
* `df -h`: para ver como van los discos duros
* `du -s`: para ver que directories se comen el espacio
* Ahora apagamos para añadir un disco
    * Esto por virtualbox, si tuviésemos hotplug no haría falta el apagado
    * Particionado
        * Caso 1:
            * `fdisk` para particionar el nuevo disco
            * Creamos una sola partición
    * `pvcreate /dev/sdb1`
        * No hace nada, etiqueta la partición para ser usada por LVM
* Añadimos el volumen fisico al grupo de volumenes
    * `vgextend cl /dev/sdb1`
    * Ahora tenemos free en `vgdisplay`
    * `lvcreate -l 4G nvar cl`
* Ahora busco llevar `var` desde `/` a `nvar`
    * Tengo que formatear el nuevo volumen
    * `mkfs -t ext4 /dev/cl/nvar`
    * `mkdir nvar`
    * `mount /dev/cl/nvar /mnt/nvar`
    * `systemctl runlevel1.target`: pasamos a mono procesador o con `init1` para modo mantenimiento
        * No quiero que cuando esté copiando var se me escriban nuevas cosas en var
        * cp -a /var/* /mnt/nvar
    * `mv /var /oldvar`: copia de seguridad
    * `umount /mnt/nvar`
    * `vi /etc/fstab`: para configurar montajes automáticamente
    * `mount -a`: monta según el `fstab`
    * `reboot`
