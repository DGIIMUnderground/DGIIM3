# Lección 1  

## Conceptos previos  

- Concepto sistema operativo: kernel + colección de paquetes  
- SO anfitrión (host) y SO invitado (guess) <----???
- Virtual: abstracción capa física implementanto sobre el SO: hipervisor, hw virtual, (Ubuntu) OS, App
- Concepto de contenedor: sobre so anfitrión monta contenedor (que tiene mismo SO sobre el que se monta) y app
- Raid 0 (Striping): Unificación discos duros como si fuera uno, la suma de los respectivos tamaños es el tamaño total. Si se rompe uno se pierde toda la información.
- Raid 1 (Mirror) : Cada uno de los discos unidos almacena la misma información, se toma como tamaño total el del mínimo.  
- Raid 5: si hay discos n discos de el alamacenamiento será el de (n-1) y uno de esoos se utiliza para almacenar los códigos de recuperación (una especie de hash con el resto de los discos) Si solo se va un disco se puede recuperar. 



## Requisitos previos  

- Disponer de virtual box 
- Disponer de la distribución de ubuntu server.


## Lección 1 t


### Creación de discos en el Virtual Box

Deberá alojar SO tipo linux y ubuntu 
Se deben de crear dos discos de las mismas caracterísiticas: 
- tamaño 1G
- Crear disco duro virtual 
- VDI 
- Reservar memoria dinámica 
- 10 GB

Una vez creados vamos la configuración de almacenamiento y en controlador sata añadimos el otro disco (cuadrado con el más)
y añadimos un nuevo disco duro 

Una vez hecho esto arrancamos el disco. 

## Inicio de instalación   

Indicar que deseamos arrancar desde un disco inicial, en ese caso el de ubuntu server. 
- Install ubuntu server
- Idioma español  
- Introducir nombre usuario: usuario
- Contraseña: ise (los nombres y la contraseña me los estoy inventando)
- Cifrar NO
- Confirmamos la hora   

### Creación de las particiones 
- Partición de disco manual 
- Creamos partición den los dos discos disponibles ( para decir que existen)
- Seleccionamos sda y le damos que sí 
- Selecciona sdb y le damos que sí 

### RAID 
- Configuar RAID por software -> sí 
- Crear un dispositivo MD
- Raid 1 como indican las práctica
- Número de dispositivos 2
- Número de dispositivos libres 0 (supuestamente solo hemos creado 2)
- Seleccionamos sda y sdb con tecla espacio como partición de los dispositivos
- Sobreescribimos los datos con un sí 
- Terminar 

### Configurar gestor de volúmenes  LVM 
- Crear grupo de volúmenes, llamémosla "main" por ejemplo. 
(espacio para poner con asteriscon los dispositivos)
- el nombre del dispositivo para el nuevo grupo de volúmenes será (en mi caso )
`/dev/md0` 
Cuando se escriba en él se escribiran en los dos discos creados /dev/sda y /dev/dba
- continuar


#### Crear los volúmnes lógicos

- Seleccionamos el grupo de volumen creado con anterioridad, main en nuestro caso. 
Crear los siguientes volúmnes lógicos, dejar root para el final y con los siguienes tamaños : 

- `swap` 1024 MB = 1G
- `boot` gestor de arranque 400 MB
- `home` 1024 MB = 1G o 2 G
- `root` lo que quede de almacenamiento 

- Terminar  

### Configurar los volúmenes cifrados  

Cifrar particiones   

- Configuar los volúmenes de cifrados
- Create encrypted volumes

Se deben de cifrar obligatoriamente: 
- `root` Puesto que contiene entre otros las direcciones var   
- `swap` y que tiene la memoria principal 

Sin obliagatoriedad de cifrar
- `home` Si no se hace cada usuario debería buscarse la vida, recomendable 

NO CIFRAR: 

- `boot` Si no no arrancaría  

Tras indicar las particiones a cifrar aparece un mensaje de partición de discos: de que se está editando la partición a nivel home, swap, root, dejar por defecto dándole a `Se ha terminado de definir la partición`

- Aparece mensaje de si deseamos mantener la distribución de las particiones existentes u configuarar los volúmnes, damos que sí

- finish 

- Ahora nos pedirá las contraseñas  (ise para todas en mi caso)
3 veces y con confirmación 

- empezará a particionar  y volverá al menú  


### Paso de montar las particiones   

Ahora debermos montar las particiones en el disco, para ello pulsaremos enter en cada partición

y las direcciones son las siguientes ( pulsar en la parte con #)
 a montar | partición | formato a utilizar   
 --- | ---  | ---
 root | \    | ext 4 transicional 
 boot | \boot   | ext 4 transicional 
 home | \home  | ext 4 transicional 
 
 ¡El swap no se monta! pero sí de debe indicar que es de tipo `área de intercambio ` ( aparece en formato a utilizar)

- Finalizar particionamiento y escribir en disco 
- Sí deseamos escribir en disco 


### Configurar el grup
 - Dejar en blanco  lo del proxy http 
 - Sin actualizaciones automáticas  
 - Instalar cargador de arranque de manera manual 
 - Instalar cargador de arranque GRUB en disco duro /dev/sda  
 
 
### Comprobación de si ha funcionado
- Si arranca, pedirá todas las contraseñas (Mensaje para la Blanca del futuro olvidadiza: todos tus usuarios y contraseñas son usuario usuario)

-  una vez arrancado utiliza `lsblk` list block devices 
deberá de salir algo como: 

![./img/resultadoFinalP1L1.jpg](Resultado correcto de la práctica 1) 
 
- Puede apagarse el equipo con poweroff


