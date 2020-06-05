---
title: Práctica 4
author: Sergio Quijano Rey
---

--------------------------------------------------------------------------------

# Primera lección - Benchmarking

## Teoría previa

* A la hora de configurar docker, tener cuidado al añadir un usario al grupo de docker
    * Si lo hacemos sin más, al añadir el usuario al grupo `docker` lo sacamos de administradores
    * Se hacer de la forma: `usermod -a -G docker david`

### Phoronix

* Comunidad que recopila tests y benchmarks
* Es una comunidad de código abierto, cualquiera puede subir un benchmark
* Se pueden subir benchmark en distintos lenguajes
    * Normalmente se suben sin compilar
    * En la compilación, hay que estudiar las dependencias de los benchmark
* **Suites**:
    * Agrupación de benchmarks a los que se le da un nombre
    * Suelen ser relevantes a alguna aplicación o caso de uso concreto
    * Por ejemplo, una *suite* para servidores web, para apache, para bases de Datos
* Vamos a lanzar tets individuales en vez de suites por tema de no estar esperando tanto tiempo
* Además, muchos tests, por no tener instalado el servidor gráfico `X`, no se van a poder lanzar
    * Tests como renderizaciones de escenas, demos de videojuegos...

### Cinebench-R20

* Sirve para una CPU
* El test consiste en renderizar una escena
* Evalúa la capacidad de una CPU para lanzar hebras en paralelo

### Apache Benchmark

* Desarrollado por Apache HTTP
* Sirve para cargar cualquier servidor HTTP, sea de Apache, `nginx`, microsoft...
* Viene incorporado en la instalación de `apache`, por esto es tan popular, por que es muy ubicuo
* Además, es muy fácil lanzar las pruebas, lo que promueve todavía más su popularidad
* De hecho, no dispone de una instalación individual, hay que instalar todo apache para tener apache benchmark
* Algunos problemas:
    * Carga muy simple, desde un único ordenador en vez de desde distintos ordenadores
    * Apache, Nginx, son los suficientemente listos como para no abrir muchas peticiones para un único usuario
    * Tampoco permite simular delays (tiempo de respueta de un cliente, que va navegando la página)

### Jmeter

* Desarrollado también por Apache
* Simula una carga mucho más real, distribuida
* Es un estándar
    * Sirve como ficheros de configuración para almacenar información de cargas en otros servicios
    * Por ejemplo, podemos usar ficheros de configuración `jmeter` en [Gatling](gatling.io)
* Lo vamos a usar en contexto HTTP, aunque se puede cargar para otros comportamientos
* Cargas sintéticas: simulamos usuarios de forma artificial
* Pueden ser cargas reales
    * Tomamos un fichero de logs de apache (con los que hemos trabajado en la anterior práctica)
    * Esta interacción guardada en el log, sirve para simular la misma carga
    * Así, podemos evaluar un sistema nuevo ante una carga habitual
    * Además, podemos usar un proxy para registrar nuetra navegación, y usar este registro para simular una carga real
    * Se puede mezclar carga real y carga sintética: lanzar 100 hebras que simulen la carga real
* Se necesita GUI para definir los test, por lo que para no tener que lanzar una `Xsession` por `ssh`, ejecutamos `JMeter` en nuestro ordenador
* Corre sobre Java, así que hay que instalarlo en la máquina desde la que queramos lanzar el test (en este caso, nuestro ordenador)
* Alternativas:
    * Gatling:
        * Programado en Scala que corre sobre Java (lenguaje funcional que corre sobre la JVM y es interoperable con Java)
        * Tiene un Domain Specific Language o DSL muy parecido a lenguaje de programacion
    * Locus:
        * El lenguaje que usa es muy legible
        * Usa python como lenguaje de configuración
* Permite realizar tests funcionales
    * Los test funcionales no simulan cargas de usuarios, comprueban la corrección de ciertas funcionalidades, a través de `assertions`
    * Buscan ver la validez del sistema, no para ver si soporta la carga
    * Aunque Jmeter haga esto, no es la mejor herramiento para esto, es mejor usar `Postman`

## Proceso

### Phoronix

* Ejecutamos los comandos en nuestra máquina virtual de Ubuntu
1. Instalamos phoronix
    * `apt-cache search phoronix`
    * `sudo apt install phoronix-test-suite`
2. Lanzamos algunos comandos que muetran información:
    * `phoronix-test-suite`: muestra ayuda y la lista de comandos que se pueden lanzar
    * `phoronix-test-suite system-info`: muestra información del sistema (hardware y software)
    * `phoronix-test-suite system-sensors`: muestra valores de los sensores físicos
        * No da mucha información al lanzarlo sobre una máquina virtual
3. Buscamos las suites disponibles
    * `apt install unzip`: necesario para la búsqueda
        * Si no se instala: `[PROBLEM] Failed to find ZIP support for extracting file: /home/sergio/.phoronix-test-suite/openbenchmarking.org/pts/av1-1.3.2.zip. Install PHP ZIP support or the unzip utility`
    * `phoronix-test-suite list-available-suites`: busca las **suites** disponibles para descargar
        * Parecido a un `apt search`
        * **Importante**: solo busca suites, no tests individuales
    * `phoronix-test-suite info pts/stream`: muestra informacion sobre un paquete, sin tener que descargarlo
4. Test Individuales
    * `phoronix-test-suite list-available-tests | grep -i processor`: buscamos **tests** sobre procesadores, que no impliquen la necesidad de tener un servidor gráfico corriendo
    * `phoronix-test-suite benchmark pts/compress-gzip`: ejecutamos un test sobre compresión usando gzip
        * Pregunta una serie de datos:
            * Pregunta si queremos guardar los resultados del test
            * Pregunta un nombre para el test
            * Pregunta un nombre para la configuracion dada
            * Pregunta por una descripcion para el resultado
        * Después de introducir estos datos, se corre el test
        * Al terminar muestra los valores en segundos. También, pregunta si queremos subir el benchmark a una base de datos abierta
            * No subir estos datos pues estamos corriendo una máquina virtual, pero de alguna forma guarda los parámetros del host
5. Vemos los resultados:
    * `phoronix-test-suite list-saved-results`: muestra la lista de resultados guardados
    * `phoronix-test-suite result-file-to-text <nombre>`: muestra la información de un resultado guardado

### Apache Benchmark: `ab`

1. En caso de no tener instalado el servidor Apache:
    * `sudo apt install apache2`
2. Lanzamos un comando de `ab` para comprobar el funcionamiento
    * `ab -n 10 -c 5 http://www.ugr.es/`
        * -n: número **total** de peticiones
        * -c: número de llamadas concurrentes
        * Así se hacen dos llamadas con cinco peticiones en paralelo
        * La salida muestra el servidor que se usa
            * En nuestro caso, `nginx`
            * Seguramente, esté actuando como proxy inverso
        * Muestra que responde con 162B: sospechoso porque pesa demasiado poco
            * `curl "http`://www.ugr.es": bajamos una página web
                * Estamos viendo una página de redirección, como se muestra
            * `curl -v "http`://www.ugr.es": lo ejecutamos en modo verbose
                * Codigo 301: nos da la página web a la que nos lleva
                * Hay que hacer https
            * `curl "https`://www.ugr.es": obtenemos la página que queremos
3. Lanzamos el mismo comando pero esta vez a la página sin redirecciones
    * `ab -n 10 -c 5 https://www.ugr.es/`
    * Podemos ver que ahora tarda bastante más
    * En la tabla que se muestra como salida:
        * `Connect` muestra el tiempo que tarda en crear un shocket tcp
        * `Waiting`: el tiempo que tarda en llegar el primer byte desde que iniciamos la conexión

--------------------------------------------------------------------------------

# Segunda Lección

## Teoria previa

### Estructura de la API REST que nos ha dado

* Se baja el código del repositorio en [github](https://github.com/davidPalomar-ugr/iseP4JMeter)
* `Docker Compose`:
    * Orquestación de servicios
    * Sirve para manejar varios servicios que forman un sistema
    * Buscamos ofertar un servicio global
    * `docker-compose.yml`: define el servicio global en base a los microservicios
* Bases de datos NoSQL
    * Son esquema-free
    * Muchas veces están optimizadas en lectura. Las aplicaciones suelen consumir muchas más lecturas que escrituras
        * Puedo replicar datos para optimizar lecturas evitando los típicos `sql join`
        * Por ejemplo, si tengo usuarios y sus tweets, en vez de juntar dos tablas, junto con cada tweet guardo su usuario (se puede hacer al revés también)
        * Haciendo esto, a la hora de actualizar un usuario, manualmente hay que actualizar los datos de los tweets, lo que es costoso
* Mongo
    * NoSQL
    * Corre en modo cluster, varios nodos
    * Hay unos nodos que son específicos para lectura, lo que optimiza las consultas
* Aplicación:
    * ` mongoinit` contiene una aplicación que introduce unos datos de juego a la base de datos
        * Ha usado `json-generator` para generar datos aleatorios
    * Las referencias tienen que ir con nombres simbólicos en vez de con IPs, porque docker asigna unas IPs en un rango privado de forma dinámica
        * `mongodb://mongodb:27017/etsii`
        * Estas referencias se especifican en el `docker-compose.yml`
    * Peticiones protegidas por `BasicAuth`: en las cabeceras pasamos las credenciales, en abierto, así que esto esta en desuso y está completamente desaconsejado
        * Aun así, BasicAuth, se suele utilizar para evitar cierto ruido en aplicaciones personales y privadas
    * `pruebaEntorno`: desde el localhost de la aplicación, prueba usando `curl` que todo va bien
        * Al lanzar bien curl, se obtiene un `jwt`
            * Se comprueba que el `jwt` sea valido y esté activo
        * Esto es muy común en interfaces del tipo API REST
    * `Oauth` es una mejor opción
        * Usa `JWT` como `basicAuth`
        * Permite conectarse a aplicaciones externas con la cuenta de google, que pasa a la aplicación externa un token con los credenciales
* `API REST`:
    * Verbos `HTTP` para acciones CRUD
    * Stateless, es decir, no se guarda sesión del usuario
    * Opciones que se presentan para mantener la sesión de un usuario:
        * Cookies, que no es API REST por romper con el stateless
        * TOKEN con la informacion que tiene que mantener durante toda la sesión
        * Después, en las peticiones nos identificamos con este token usando la cabecera `Authorization: Bearer $TOKEN`
* `JWT`:
    * Va codificado, que no significa que esté cifrado
    * La información, en el hash, va firmado
    * Se puede descodificar en [jwt.io](jwt.io)
    * Comprobaciones de forma automática en código de `node.js`:
        * Objeto `payload` con los campos:
            * `iat`: initialized at, indica el momento en el que se creó el token
            * `nbf`: not before, momento a partir del cual se puede usar el token
            * `exp`: expiration, tiempo en el que el token es válido
            * Estos campos se pueden encontrar en el [RFC](https://tools.ietf.org/html/rfc7519#section-4.1)

## Proceso

1. Instalamos la aplicación del profesor en nuestro Ubuntu:
    * `git clone https://github.com/davidPalomar-ugr/iseP4JMeter.git`: bajamos el código de la API REST
    * `sudo apt install docker-compose`
2. Construimos el entorno y lanzamos la aplicación
    * `sudo docker-compose up`: lanza la aplicación
        * Construye todas las imágenes docker a partir de los `Dockerfile`
        * Debería tardar algo en construir todo
        * Además lanza el comando `mongodbinit` para rellenar la base de datos de mongodb
    * `sudo docker-compose up -d`: lanza la aplicación en modo `dettached` para no ver los logs en la terminal
        * Para parar la aplicación usaríamos `sudo docker-compose down`
    * Si no queremos usar `sudo`, podemos añadir el usuario al grupo `docker`
    * En futuras ejecuciones todo el `build` no es necesario (excepto que modifiquemos código de la aplicación)
3. Comprobamos que la aplicación funciona
    * `curl -vv htpp://localhost:300`: a mano, usando `curl`
        * Se podría hacer lo mismo, pero de forma avanzada, con POSTMan
    * `./pruebaEntorno`: script que contiene un comando de `curl` para comprobar el funcionamiento
2. Instalamos `JMeter` en nuestro ordenador:
    * `yay -S jmeter`
    * Ahora podemos lanzar el programa en nuestro ordenador
    * Pasamos a configurar la prueba de carga

### Definición de la prueba de carga en JMeter

1. Creamos un nuevo test plan dándole un nombre y un comentario
2. Añadimos un `thread group`
    * `Test Plan -> Add -> Thread Group`
    * Simulan cargas de trabajo de un grupo de usuarios
    * Numero de threads: usuarios que usan simultáneamente el sistema
    * `Ramp Up`: no me he enterado
    * `Loop-Count`: el número de veces que se ejecuta
    * Parámetros:
        * `Name: Alumnos`
        * `Comments: Thread grupo que simula un grupo de alumnos que consumen nuestra REST API`
        * `Number of threads (users): 10`
3. Añadimos un `sampler` para acceder a la página index de la web
    * `Alumnos -> Add -> Sampler -> HTTP Request`
    * Parámetros:
        * `Name: Acceso a pagina principal`
        * `Protocol: no se toca nada, por defecto HTTP (cuidado con HTTPS)`
        * `IP:192.168.56.101`: la IP de la VM Ubuntu
        * `Port: 3000`: más tarde lo parametrizaremos
        * `Method: Get`
        * `Path: /`
4. Añadimos un visualizador de los resultados
    * `Test Plan -> Add -> Listener -> View Results Tree`
    * Parámetros:
        * `Name: Visualizador de resultados`
    * No usarlo en la carga del test final porque consume muchos recursos
5. Ya podemos ver un ejemplo de test
    * Guardamos la configuración en un archivo `.jmx`
    * Le damos al play y vamos al `Visualizador de resultados`
        * Muestra datos estadísticos de la petición
        * Vemos las peticiones enviadas
        * Vemos los headers de la petición
            * `User-Agent: Apache-HttpClient`: esto puede producir rechazos en ciertas páginas web, cambiar por la cabecera de un browser
        * También vemos que se han registrado las peticiones en la terminal
            * Es decir, en el prompt que aparece cuando, desde Ubuntu, hacemos `sudo docker-compose up`
6. Parametrizamos el entorno para no repetirnos
    * Petición HTTP REQUEST por defecto:
        * `Test Sample -> Add -> Config Element -> HTTP REQUEST DEFAULTS`
        * Parámetros:
            * `Name: Parametros servidor ETSIIT`
            * `Comments: Parametros por defecto para las HTTP REQUEST en nuestro servidor`
            * `IP: 192.168.56.101`
            * `Port: 3000`
        * Esto nos evita repetir datos en cada `HTTP REQUEST`
    * Definimos variables de usuario que nos van a venir bien:
        * Test Plan -> User Defined Variables
            * Podemos poner aquí las variables de la forma clave-valor que queramos
            * Se acceden usando ${nombre_variable} en los formularios del programa
        * Esto sirve para todo el ambiente de trabajo, no solo para el `HTTP REQUEST`
        * Añadimos:
            `SERVER_IP: 192.168.56.101`
            `SERVER_PORT: 3000`
        * **IMPORTANTE**: Estas variables se pueden especificar por línea de comandos
6. Añadimos un `sampler` para el login del alumno
    * `Grupo de Usuarios -> Add -> Sampler -> HTTP Request`
    * Parámetros:
        * `Name: Login Alumno`
        * `Method: POST`
        * `Path: /api/v1/auth/login`
        * El resto lo dejamos en blanco, aprovechando `Parametros servidor ETSIIT`
        * Parameters:
            * `login: hernandezmayer@tropoli.com`
            * `password: nisi`
7. Para podernos autenticar correctamente, añadimos un `HTTP AUTHORIZATION MANAGER`
    * `Test Sample -> Add -> HTTP Authorization Manager`
    * Parámetros:
        * `Name: Basic Auth`
        * `Comments: Autenticacion usando el BasicAuth indicado en el guion de practicas`
        * `Base URL: http://${SERVER_IP}:${SERVER_PORT}/`
        * `Username: etsiiApi`
        * `Password: laApiDeLaETSIIDaLache`
    * El usuario y contraseña viene documentado en el guión de prácticas, así como en la web principal de la API
8. Comprobamos que todo funciona
    * Ejecutamos el test y del `Visualizador de resultados` tomamos el token
    * Comprobamos la información del token en la web [jwt.io](jwt.io)
    * Este `jwt` nos sirve para hacer peticiones al usuario indicado en `Login Alumno`
9. Tomo el token que se me ha dado, porque me va a hacer falta
    * `Login Alumno -> Add -> Post Processor -> Regular Expression Extractor`
    * Parámetros:
        * Name: Extraer token jwt
        * Comments: Extrae el token jwt que nos autentica
        * Regular Expression: .*: nos quedamos con todo el token
        * Template: $0$: así no uso subgrupos y me quedo con todo el resultado
10. Consultamos el expediente del alumno
    * Alumnos -> Add -> Sampler -> HTTP Request
    * Parámetros:
        * Name: Consultar expediente
        * Comments: Consulta el expediente de un alumno que ha hecho login
        * Method: GET
        * Path: /api/v1/alumnos/alumno/hernandezmayer%40tropoli.com: `%40` sirve para indicar el `@` codificado
11. Añado un `HTTP HEAD manager`, para pasar el token de autenticación
    * `Consultar Expediente -> Add -> Config Element -> HTTP Header Manager`
    * Parámetros:
        * Name: Authorization Manager
        * Comments: Header Manager para pasar el token a la consulta del expediente
        * Parameters:
            * Name: Authorization
            * Value: Bearer ${TOKEN}
        * Así, en la consulta, paso el token para autenticarme
12. Ejecutamos el test plan
    * En el `Visualizador de resultados` podemos ver el `JSON` devuelto con la consulta
    * Podemos visualizarlo haciendo: `echo "<contenido_json> | jq"`
        * `jq` funciona como un JSON prettifier
    * Podemos también visitar una web que sea un *JSON prettifier*

### Ejecución de la prueba de carga

* Partimos de que hemos guardado la configuración de la prueba en un fichero `prueba_carga.jmx`
1. Lanzamos la prueba de carga
    * `jmeter -n -t p4_clase.jmx -l resultado.jtl`: lanzamos la prueba, guardamos los resultados en `resultado`
        * `-n`: no GUI
        * `-t`: fichero de configuracion, formato `.jmx`
        * `-l`: fichero donde guardamos los resultados, formato `.jtl`
        * Al terminar de lanzar el test, se muestra un pequeño `summary` con algunos datos
2. Mientras se lanza la prueba, vemos como impacta en el rendimiento
    * Podemos, desde ubuntu, lanzar `htop` para ver el impacto
    * También tenemos programada la página [status](192.168.56.101:3000/status) donde ver la monitorización
3. Comprobamos los resultados, por línea de comandos
    * `cat resultado.jtl`: vemos los resultados
        * Obtenemos un Summary Report
        * Podemos ver directamente los resultados porque un archivo `.jtl` es un archivo `.csv`
4. Comprobamos los resultados en la aplicación
    * `Test Plan -> Add -> Listener -> Summary Report`
    * Parámetros:
        * Name: Reporte de test
        * Comments: Muestra los resultados de un test de carga
        * Filename: lo indicamos con el browser, seleccionando el archivo `resultado.jtl`
            * Una vez que pone el `path` absoluto, lo cambiamos por un `path` relativo para que lo pueda usar nuestro profesor

### Prueba de carga para muchos alumnos

* Hasta ahora, este proceso lo hemos llevado a cabo para un único alumno. Buscamos tomar muchos alumnos automáticamente y hacer las consultas a la API
1. Añadimos los datos de los alumnos del archivo `./jMeter/alumnos.csv`
    * Alumnos -> Add -> Config Element -> CSV Data Set Config
    * Parámetros:
        * Name: Alumnos Login Data
        * Comments: Datos de autenticacion de los alumnos
        * Filename: hacemos `Browse` para seleccionar el archivo
            * Por desgracia, tenemos que clonar el repositorio en nuestro host, si queremos correr el test desde nuestro ordenador
            * Una opción es especificar la ruta desde la VM Ubuntu y correrlo desde ahí, pero es incómodo a la hora de debuggear el código
            * Además, cambiamos la ruta absoluta por una ruta relativa
            * Queda: `./jMeter/alumnos.csv`
    * Usamos los nombres de variable que nos da la cabecera del `.csv`
2. Editamos el `Login Alumno` para que use estos datos
    * En la tabla de parámetros
        * `login: ${login}`
        * `password: ${password}`
    * Si lo intentamos ejecutar, falla el consultar expediente, porque estamos usando constantes
3. Cambiamos el `Consultar Expediente`
    * Path: ${ENDPOINT_QUERY_ALUMNO}/${login}
    * Notar que he parametrizado los *end-points* en `User Defined Variables`
    * También está parametrizado el *end-point* de la consulta
    * Esto de cambiar el path de esta forma no es muy recomendable
        * Funciona, según el profesor, porque estamos usando `node`
    * Si lo ejecutamos vemos que ahora sí que funciona correctamente

### Consideraciones para la tarea

* Tenemos que repetir el mismo proceso para los administradores
* El `Visualizado de Resultados` es un `View Results Tree`
    * Consume mucho, así que para los test finales o lo borramos o lo desactivamos
    * Mejor hacer un `disable` por si lo necesitamos para debuggear
* Con esto ya hemos terminado el ejercicio entregable. La siguiente lección es extra
* Hay que añadir esperas aleaatorias con distribución gaussiana
* Tenemos que hacer ciertos cambios al thread group `Administradores`

### Cambios al administrador

* Partiendo de una estructura parecida a la de alumno
1. Borramos el `Consultar expediente`
    * De `Administradores -> Consultar expediente` nos quedamos el `Authorization Manager`
    * Borramos `Administradores -> Consultar expediente`
2. Tomamos los datos del `/iseP4JMeter/jMeter/apiAlumnos.log`
    * `Administradores -> Add -> Sampler -> Access log sampler`
    * Parámetros:
        * Name: Acceso de administradores
        * Comments: Fichero con datos reales de uso de usuarios de la API
        * Server: ${SERVER_IP}
        * Port: ${SERVER_PORT}
        * Log File: ./apiAlumnos.log

--------------------------------------------------------------------------------

# Tercera leccion

## Teoría

### Funcionalidad de navegación de usuario

* Instalamos un proxy entre la web y el navegador del ordenador
* Se registran todas las peticiones que realiza un usuario de verdad, desde nuestro ordenador
* Luego, ese registro se usa para generar carga
* Apache Log es más potente, porque no controlamos solo la navegación, sino toda la carga que ha sufrido el servidor

## Proceso

1. Abrimos Jmeter y creamos un Test Plan
    * Add -> Non-Test Element -> Http test script recorder
        * Name: Grabacion HTTP
        * Start
        * Vamos a la página [apache.org](apache.org)
    * Añadimos al plan -> THREAD GROUP
    * Al Thread Group añadimos recording controller
         * Name: Registro Apache.org
    * Volvemos Grabacion Http
        * Target Controller: Test Plan > Thread Group > Registro Apache.org
    * Al hacer start nos muestra el proxy que se va a usar en el test. Tenemos que configurarlo para que firefox use este proxy
* Firefox: preferencias: proxy: manual proxy
    * http proxy: localhost
    * port: 8888
* Abro [apache.org](apache.org)
    * Ya en el registro podemos ver todas las peticiones que estamos haciendo
    * Podemos ver que las cabeceras que se generan son todo el rato las mismas
    * Esto último es un incoveniente porque genera mucha basura, las cabeceras no aportan demasiada información nueva
* Prueba de carga con estos datos
    * thread group
        * Name: visitante de apache
        * Vamos llevando una a una las peticiones al visitante de apache
        * Cogemos las propiedades y las llevamos al thread group global
* Todo esto para HTTP, no para HTTPS. Tenemos el problema:
    * Hacemos una petición HTTPS, que pasa por el proxy
    * El proxy hace la petición, toma la web, la descifra
    * El proxy devuelve la página descifrada, pero la firma con su propia firma para falsear la conexión https
    * El navegador esperaba una página cifrada, avisa de un posible ataque man-in-the-middle
* Para ello añadimos el certificado de JMeter para firefox, y así no se queja
    * Firefox -> preferencias -> certificados -> view certificates -> Import -> bin > ApacheJMeterTemporaryRootCA.crt
* HTTPS es HTTP sobre TLS
    * TLS tiene varios standares de TLS, el moderno el 4
    * Para ciertos estandares modernos, este truco no vale, como para la universidad de granada
    * Si intentamos ir a la ugr.es, Error code: SSL_ERROR_RX_RECORD_TOO_LONG
    * Problema: firefox se tiene que identificar como firefox, pero la peticion la envia JMeter Proxy y no firefox
    * about:config -> security.tls.version.max -> Lo colocamos a 3
    * Ahora ya podemos entrar a la página de la ugr

--------------------------------------------------------------------------------

## Cosas que saber

* Lista de cosas que ha ido diciendo el profesor que hay que saber para el examen
* En el guión de prácticas aparecen más cosas que hay que saber
* curl
    * Verbose
    * POST
    * GET
    * Cómo sacar información de las cabeceras
* jmeter por comandos

# Tarea a realizar

* Hacer tutorial de JMeter que está en el guión
* **Enunciado de la tarea**:
    * Test de carga sobre una API rest
    * Lanzamos jmeter desde la máquina virtual que no corre docker con la API contra la máquina que corre docker con la API
* No se entrega memoria
* Se sube el plan `jmx`
* Se suben los resultados `jtl`
* En caso de problemas, un fichero de texto plano documentando o desarrollando los problemas que hemos tenido
* La prueba de carga se hace atacando a la VM de Ubuntu (según el auto-guión)
