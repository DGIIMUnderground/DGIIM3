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
        * Cambiamos del puerto 22 al 22232: `Port 2232`
        * `systemctl restart sshd`: mataría el servicio y lo inicia. No hacemos esto para no matar nuestra conexion
        * `systemctl reload sshd`: deja las conexiones vivas, pero solo escucha en el puerto nuevo
6. Comprobamos en el host las consecuencias del cambio de puerto
    * `ssh sergio@192.168.56.10`: no se conecta, 
        * La otra conexion sigue viva gracias a que hemos hecho `reload` en vez de `restart`
    * `ssh -p 22232 sergio@192.168.56.10`: sí que se conecta
7. Deshacemos el cambio de puertos en Ubuntu, pasándolo otra vez al puerto 22
8. Ejecución de comandos a traǘes de ssh
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
