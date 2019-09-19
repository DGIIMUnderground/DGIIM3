# Tema 1: Introducción a los fundamentos de redes

* **Sistema de comunicación**: Conjunto de elementos que posibilita el intercambio y transmisión de información entre dos puntos remotos.
* **Información**: Conjunto de datos al que se dota de significado.
* **Red**: Conjunto de equipos terminales de datos autónomos, interconectados y dotados de capacidades de intercambio de información.
* **Comunicaciones salto a salto**: Las que tienen lugar de forma directa entre cada par de nodos involucrados en la comunicación de dos nodos terminales.
* **Comunicaciones extremo a extremo**: Comunicaciones entre los nodos terminales y que habitualmente implican la participación de nodos que actúan de intermediarios.
* **Host**: Equipo terminal de datos.
* **Subred**: Conjunto de elementos que posibilitan la conexión de los host. Se componen de:
  * **Nodos de conmutación**: dispositivos cuya misión es posibilitar el transporte de los datos entre un origen y un destino dados.
  * **Líneas de transmisión**: Canales o enlaces de comunicación. 

## Modelo de referencia OSI y TCP/IP

El objetivo es conseguir una comunicación eficaz y transparente entre las estaciones o equipos finales involucrados. Para ello se adopta un modelo en capas en el que cada capa resuelve uno o varios problemas   involucrados en la comunicación e independientemente del funcionamiento de las demás capas.

* **Modelo de referencia**: Conjunto de capas definido y sus funciones asociadas.

### Modelo OSI:

De abajo a arriba:

* *Capa física*: Funciones relacionadas con la transmisión de datos desde el punto de vista de características eléctricas, mecánicas o funcionales. Su finalidad es el intercambio de bits entre dos terminales.
* *Capa de control del enlaces de datos*: A partir de los bits agrupados en tramas, se encarga de la delimitación de tramas, control de errores y control de flujo. Su finalidad es la entrega de datos de forma fiable al receptor.
* *Red*: Encaminamiento (establecimiento de la ruta a seguir entre el origen y el destino), control de congestión, interconexión de redes y evaluación del consumo generado. Su finalidad es garantizar que los paquetes son enviados de la forma más eficiente posible a través de la subred.
* *Transporte*: Control de errores y de flujo extremo a extremo, posibilitar varias comunicaciones entre los distintos hosts. Su finalidad es la entrega fiable de los segmentos entre el origen y el destino.
* *Sesión*: Gestionar el diálogo entre los host.
* *Presentación*: Representación de los datos que provienen de la capa superior.
* Aplicación: Servicios finales que se ofrecen al usuario.

### Modelo TCP/IP:

De arriba a abajo:

* *Aplicación*: Servicios de usuario como *ssh* (acceso remoto), *ftp* (transferencia de ficheros), *smtp* (correo electrónico), *http* (web)...
* *Transporte*: Control de flujo, errores, congestión y conexión (extremo a extremo).
* *Red*: Encaminamiento y fragmentación.

Estas tres capas deben sustentarse necesariamente sobre otras inferiores que permitan la comunicación entre los sistemas finales. TCP/IP es una red software, es decir, puede implementarse sobre cualquier red sin ser dependiente de ella.

## Terminología

* **Comunicación real (vertical)**: Flujo que sigue la información entre el emisor y el receptor: intercambio de datos entre capas adycentes, en sentido descendente en el emisor y en sentido ascendente en el receptor.
* **Comunicación virtual (horizontal)**: Comunicación desde el punto de vista de las entidades paritarias. En cada capa se añade una serie de información suplementaria (cabecera) destinada a establecer una comunicación coherente entre entidades paritarias. La cabecera solo es relevante para las entidades paritarias involucradas y se irán eliminando al pasar los datos a capas superiores.
* **Entidades del nivel N**: Elementos hardware y software que componen una capa N.
* **Entidades pares**: Entidades de nivel N en el emisor y en el receptor.
* **Protocolo**: Conjunto de reglas y convenciones a aplicar en la comunicación entre dos entidades paritarias con el objetivo de llevar a cabo una cierta función o servicio.
* **Interfaz**: "Superficie" de separación entre dos capas adyacentes a traves de la cual se realiza la comunicación vertical.
* **Servicio**: Funciones o prestaciones que una capa ofrece de forma transparente a su superior.
* **Capa proveedora/usuaria del servicio**: Dadas dos capas adyacentes, N y N+1, la inferior se denomina proveedora de servicios y la superior usuaria de servicios.
* **Pila de protocolos**: Especificación en capas que de los protocolos que componen una arquitectura de red.
* **Arquitectura de Red**: Conjunto de capas junto a sus protocolos asociados.
* **SAP (Service Access Point)**: Punto de acceso a un servicio. En el SAP se produce la comunicación entre dos capas adyacentes (sinónimo de interfaz pero más concreto).
* **SDU (Service Data Unit)**: Conjunto de datos que una entidad maneja que procede de su superior.
* **PDU (Protocol Data Unit)**: SDU recibida de la capa superior más la cabecera añadida para la comunicación con otras entidades paritarias.
  * PDU=cabecera+SDU

## Retardos en la comunicación

* Acceso al medio.
* Procesamiento nodal (interno).
* Tiempo en cola: Tiempo esperando para transmisión.

+

* Tiempo de Transmisión (Tt) = 	Longitud (bits) / Velocidad de transmisión (bits/segundo)

+

* Tiempo de Propagación (Tp) = Distancia (m) / Velocidad de propagación (m/s)

## Servicios

* *Orientado a Conexión (SOC):* Se establece una conexión como paso previo a la transmisión de datos entre emisor y receptor.
* *No Orientado a  Conexión (NOC):* No precisa la existencia de una conexión previa a la transmisión de la información.

A partir de esta característica:

* *Confirmado*: El emisor tiene alguna constancia de la recepción en el destino.
* *No confirmado*: No se produce realimentación sobre este hecho.

### Primitivas de servicio

Cada uno de los procesos elementales en que se desarolla un servicio. Son cuatro:

* *Solicitud (Request)*: Requerimiento de comunicación por parte del solicitante.
* *Indicación (Indication)*: Recepción de la solicitud en el destino.
* *Respuesta (Response)*: Si el destino acepta el servicio, responde a la solicitud.
* *Confirmación (Confirm)*: De forma análoga al caso de la indicación, se corresponde con la recepción de la respuesta del emisor.

# Tema 2: Servicios y protocolos de aplicación en internet

Un **servidor** es un proceso que permite el acceso remoto a ciertos recursos existentes en un host. Una aplicación servidora se caracteriza por encontrarse inicialmente en un estado de escucha, no realizando función alguna hasta que recibe una solicitud remota por parte del **cliente**.

Un servicio que adopte el modelo cliente-servidor se desarrolla en los siguientes pasos:

1. Se ejecuta la aplicación servidora, pasando a modo escucha y esperando solicitudes de servicio (puesta en marcha *pasiva*).
2. La ejecución del servicio cliente da lugar al envío de una solicitud de servicio hacia el servidor  (puesta en marcha *activa*).
3. El servidor responde a dicha solicitud de acuerdo con la aplicación desarrollada.
4. Tras el desarrollo del servicio, el servidor vuelve al estado de escucha a la espera de nuevas solicitudes y el cliente finaliza su ejecución.

Los servidores están siempre en funcionamiento, la IP es permanente y pública y están agrupados en "granjas".

Los clientes funcionan intermitentemente, pueden tener IP dinámica y privada, se comunican con el servidor pero no se comunican entre sí.

Mientras el servidor se asocia con un puerto conocido a priori, los clientes suelen usar puertos diferentes.

Las aplicaciones cliente-servidor se pueden implementar en forma de cascada o recursiva: Una entidad servidora puede actuar como cliente de otra servidora.

## La interfaz *socket*

Es la interfaz entre las aplicaciones y la pila TCP/IP.

*  ***SOCKET***: **Descriptor** de una transmisión a través del cual la aplicación puede enviar y/o recibir información hacia y/o desde otro proceso de aplicación.
* **Puerta** de acceso entre la **aplicación** y los servicios de **transporte**.

Para interaccionar con una aplicación remota mediante la interfaz *socket*, la aplicación debe abrir la comunicación invocando a la correspondiente llamada al sistema, la cual devuelve un descriptor (denominado *socket*), luego se escribe en él y por último se cierra.

El *socket* realmente consiste en una variable puntero a una estructura que tiene los siguientes campos:

* **Familia de protocolos**: Referente al tipo de *socket* que se trata, puede tomar los valores **`PF_INET`** (comunicaciones Internet IPv4), `PF_UNIX`, `PF_APPLETALK`, ...
* **Servicio**: Tipo de servicio ofrecido por las capas inferiores. Puede ser, entre otros, `SOCK_STREAM` para TCP, `SOCK_DGRAM` para UDP, `SOCK_RAW` para IP o inferiores.
* **IP local**
* **IP remota**
* **Puerto local**
* **Puerto remoto**

## Retardo en cola

Para estimar los retardos en cola se usa la teoría de colas:

El uso de un servidor se modela con un sistema M/M/1 y el retardo en cola es:

$$
R=\dfrac{\lambda\cdot(T_s)^2}{1-\lambda\cdot T_s}
$$

donde T_s(distr exponencial) es el tiempo de servicio y \lambda (Poisson) la ratio de llegada de solicitudes.

La misma expresión se puede usar para calcular retardo en cola en un router.

## ¿Qué es y qué define un protocolo?

* **Tipo de servicio**: SOC o SNOC, realimentado o no.
* **Tipo de mensaje**: request, response, ...
* **Sintaxis**: Definición y estructura de "campos" en el mensaje.
* **Semántica**: Significado de los "campos".
* **Reglas**: Cuándo los procesos envían o responden mensajes.

## Tipos de protocolos

* Protocolos de **dominio público** vs **propietarios**:
  * **Público**: Libre disposición de cualquier interesado.
  * **Propietario**: Propiedad de una empresa privada.
* Protocolos **in-band** vs **out-of-band**:
  * **in-band**: La información de control usa el mismo canal que los datos.
  * **out-of-band**: Un canal para datos y otro para control.
* Protocolos **stateless** vs **state-full**:
  * **stateless**: Se envían mensajes con independencia de lo intercambiado anteriormente. Consume menos recursos pero es más pesado.
  * **state-full**: Las entidades recuerdan su historia. Consumen más recursos pero es más ligero.

* Protocolos **persistentes** vs **no persistentes**:
  * **persistentes**: Se usa una misma conexión para múltiples peticiones.
  * **no persistentes**: Se usa una conexión para cada petición.

Se tiende a hacer los protocolos flexibles con una **cabecera fija**  y una serie de **trozos** (obligatorios y opcionales). Los trozos pueden incluir una cabecera específica y una serie de datos en forma de parámetros fijos y variables.

## Requerimientos de algunas aplicaciones

* **Pérdidas de datos (errores)**: Algunas apps pueden tolerar algunas pérdidas de datos, otras requieren una transferencia 100% fiable.
* **Requisitos temporales**: Algunas apps denominadas *inelásticas* requieren retardo acotado (delay).
* **Ancho de banda (tasa de transmisión)**: Algunas apps requieren una tasa de transmisión determinada.
* **Seguridad**: Encriptación, autenticación, no repudio...

## DNS

El DNS (Domain Name System) es un servicio que posibilita la conversión automática de nombres a direcciones IP, servicio que se conoce como *resolución de nombres*.

DNS se implementa tanto sobre TCP como sobre UDP (**puerto 53**).

Esquema básico de resolución de nombres:

1. Cuando un programa precisa la resolución de un nombre, contacta con un proceso del SO local llamado *resolver*, el cual consulta los ficheros de datos de conversión y/o una caché donde se almacenan temporalmente resoluciones previas.
2. Si la resolución no puede hacerse efectiva localmente, se contacta con un servidor DNS remoto. El servidor proporcionará la respuesta después de consultar los ficheros oportunos o un servidor de nivel superior.

La resolución de nombres puede ser **recursiva** o **no recursiva**.

* En la recursiva tanto la consulta como la respuesta se trasladan recurrentemente de servidor en servidor dejando la interacción abierta todo el procedimiento.
* La no recursiva se caracteriza por que cuando un servidor no conoce la respuesta, este responde con la identidad del siguiente servidor en la jerarquía con el que el primero debe contactar directamente para hacer la consulta pertinente. La interacción no se mantiene todo el procedimiento.

La resolución recursiva consume más recursos pero puede actualizar las cachés de los servidores involucrados, mientras que la iterativa consume menos recursos pero las memorias caché no pueden ser actualizadas.

* **Zona**: Conjunto completo de nombres de dominio que están por debajo de un nodo del árbol.
* **Autoridad**: Servidor DNS responsable de cada zona que tiene la obligación de conocer la resolución de todos los posibles nombres que pertenezcan a esa zona.
* **Delegación de autoridad**: Una autoridad de una zona dada puede liberarse de la obligación de conocer y gestionar parte de su zona cediendo esta obligación a otro servidor delegado que se ocupará de ello una vez la autoridad haya sido delegada.

Las resoluciones cuya respuestas no sean conocidas por el primer servidor de nombres son redirigidas al raíz. Este realiza un análisis sintáctico del dominio y obtiene el nombre de primer nivel. A partir de este momento, si tienen delegada la autoridad para esa zona y no tiene en caché la resolución, trasladará hacia abajo en el árbol la resolución hasta la autoridad responsable de la zona delegada. Este proceso se repite tantos niveles hacia abajo como hagan falta hasta que, o bien se encuentra la solución en la caché de un servidor, o se llega a la autoridad de la zona correspondiente, la cual tiene la obligación de conocer la respuesta.

### Formato de los mensajes DNS

* **Identificación**: Campo de 16 bits que identifica cada mensaje DNS.
* **Parámetro**: Campo de 16 bits cuyo significado es el siguiente:
  * Bit 0: Tipo de operación: Solicitud/respuesta (0/1).
  * Bits 1-4: Tipo de solicitud.
  * Bit 5: Activado si la respuesta es autorizada.
  * Bit 6: Activado si el mensaje está truncada.
  * Bit 7: Activado si se desea recursión.
  * Bit 8: Activado si la recursión está disponible.
  * Bits 9-11: Reservados.
  * Bits 12-15: Tipo de respuesta.
* **Nsolicitudes**: Número de entradas de la sección *Ssolicitud* del mensaje.
* **Nrespuestas**: Número de entradas de la sección *Srespuesta* del mensaje.
* **Nautoridades**: Número de entradas de la sección *Sautoridad* del mensaje.
* **Nadicionales**: Número de entradas de la sección *Sadicional* del mensaje.
* **Ssolicitud**: Conjunto de octetos a través del cual se lleva a cabo una consulta. Consta a su vez de los siguientes campos:
  * Solicitud
  * Tipo
  * Clase
* **Srespuesta**: Conjunto de octetos a través del cual se da respuesta a una consulta DNS.
* **Sautoridad**: Sección que contiene datos sobre servidores autorizados.
* **Sadicional**: Incluye datos adicionales a las anteriores secciones

El formato de estas últimas tres secciones es: recurso, tipo, clase, tiempo, longitud, datos 

## El correo electrónico

Se identifican las siguientes entidades:

1. **Agente de usuario (MUA)**: Interfaz entre el usuario y el sistema de correo, permitiendo al usuario la lectura y la escritura de mensajes electrónicos.
2. **Agente de transporte (MTA)**: Encargado de recibir y enviar correo electrónico.

Cuando un usuario crea un mensaje a través de su MUA, se almacena en una zona del sistema conocida como *spool de correo*; de ella será leído por el MTA para su envío hacia el destino. Por su parte, la MTA cuando recibe un email lo almacena en una zona denominada *buzón*, a ella debe acceder el usuario a través de la MUA para su lectura.

En el servicio de correo se identifican dos acciones claramente diferenciadas que involucran a dos (o más) protocolos. Por un lado, el procedimiento de envío de correo, que utiliza el protocolo *SMTP* (Simple Mail Transfer Protocol); y el procedimiento de lectura o consulta del buzón, en el que se pueden usar de forma indistinta *POP3* e *IMAP*.

### SMTP

SMTP se implementa mediante el cliente SMTP y el servidor SMTP, ejecutados respectivamente en el mail server que está enviando y recibiendo correo. Usa TCP en el **puerto 25** y es orientado a texto. Es orientado a conexión, *in-band* y *state-full*. La interacción entre el cliente y el servidor se realiza mediante comandos y respuestas:

* Los comandos son texto en ASCII
* Las respuestas se componen de un código y una frase explicativa.

#### Pasos en el envío/recepción de correo

1. El usuario escribe junto a su MUA un mensaje dirigido a una cierta dirección de correo electrónico destino.
2. Se envía con SMTP o HTTP el mensaje a la MTA origen que lo almacena en la cola de mensajes salientes.
3. El cliente SMTP abre una conexión TCP con la MTA destino.
4. El cliente SMTP envía el mensaje sobre la conexión TCP.
5. La MTA destino ubica el mensaje recibido en el mailbox del usuario destino.
6. El usuario destino invoca a su MUA para leer el mensaje utilizando POP3, IMAP o HTTP.



Los comandos por parte del cliente más usuales son:

* **HELO**: Identifica el remitente al destinatario.
* **MAIL FROM**: Identifica una transacción de correo e identifica al emisor.
* **RCPT TO**: Se utiliza para identificar un destinatario individual.
* **DATA**: Permite enviar una serie de líneas. Cada línea admite un máximo de 1000 caracteres y la última línea debe acabar con un punto "." segido de <CR><LF>.
* **RSET**: Aborta la transacción actual.
* **NOOP**: No operación, indica al extremo que envíe una respuesta positiva.
* **QUIT**: Pide al extremo una respuesta positiva y cierra la conexión.

Y los códigos de respuesta del servidor más usuales son:

* **220**: Servicio preparado.
* **221**: Cerrando el canal de transmisión.
* **250**: Operación completada con éxito.
* **354**: Introduzca texto. Finalice con <CR><LF>.<CR><LF>
* **501**: Error de sintaxis.
* **551**: Usuario no local.

#### Extensiones MIME

El protocolo SMTP original presenta algunas limitaciones, como que solo permite el envío de mensajes ASCII y la longitud de las líneas tiene un límite máximo de 1000 caracteres.

De esta forma no sería posible el envío de mensajes multimedia con imágenes, audio, gráficos, etc. Para resolver esta situación se construyeron las *extensiones multipropósito de correo Internet* (MIME), encaminadas a soportar:

* Texto y cabeceras en conjuntos de caracteres distintos de US-ASCII.
* Adjuntos que no sean de tipo texto.
* Cuerpos de mensajes con múltiples partes

MIME incluye una serie de cabeceras *email* adicionales:

* **MIME-Version**: Identifica la versión de MIME.
* **Content-Description**: Cadena de texto que describe el contenido del mensaje.
* **Content-Id**: Identificador único del mensaje.
* **Content-Transfer-Encoding**: 
  * Indican la manera de la que está envuelto el cuerpo para su transmisión, ya que podría haber problemas con la mayoría de los caracteres distintos de letras, números y signos de puntuación.
  * Existen 5 tipos de codificación: ASCII 7, ASCII 8, codificación binaria, base64 y entrecomillada-imprimible.7.2
* **Content-Type**: Especifica la naturaleza del cuerpo del mensaje. Los distintos tipos son:
  * **Text**:
    * **Plain**: texto sin formato.
    * **Richtext**: texto con comandos de formato sencillos.
  * **Image**:
    * **Gif**: Imagen con formato GIF.
    * **Jpeg**: Imagen con formato JPEG.
  * **Audio**:
    * **Basic**: Sonido.
  * **Video**:
    * **Mpeg**: Película en formato MPEG.
  * **Application**: tipo para los formatos que requieren procesamiento externo no cubierto por ninguno de los demás tipos.
    * **Octet-stream**: Secuencia de bytes no interpretados. A su recepción, la MUA debería presentarla por pantalla sugiriendo al usuario que se copie en un archivo solicitando un nombre.
    * **Postscript**: Se refiere al lenguaje PostScript de Adobe Systems.
  * **Message**: Permite que un mensaje esté encapsulado por completo dentro de otro. Útil para reenviar correo electrónico.
    * ***rfc822***: Utilizado cuando se encapsula un mensaje RFC 822 completo en un mensaje externo.
    * **Partial**: Permite dividir un mensaje en pedazos y enviarlos por separado.
    * **External-body**: Puede usarse para mensajes muy grandes, como películas de vídeo.
  * **Multipart**: Permite que un mensaje contenga más de una parte, con el comienzo y fin de cada parte claramente delimitados.
    * **Mixed**: Permite que cada parte sea diferente.
    * **Alternative**: Cada parte contiene el mismo mensaje pero expresado en una codificación diferente.
    * **Parallel**: Usado cuando todas las partes deben "verse" simultáneamente.
    * **Digest**: Usado cuando se juntan muchos mensajes en un mensaje compuesto.

### POP3

Una vez establecida la conexión TCP, tiene tres fases diferenciadas:

1. **Autenticación**: Identificación del cliente. Comandos `user` (nombre de usuario) y `pass` (contraseña).
2. **Transacción**: Estado en el que a través de solicitudes por parte del cliente se accede al correo electrónico especificado. Comandos:
   * `list`: Lista mensajes por número.
   * `retr`: Obtiene mensajes por número.
   * `dele`: Borra mensajes por número.
   * `quit`: Para finalizar servicio.
3. **Actualización**: El servidor elimina los recursos adquiridos en el estado anterior y cierra la conexión TCP.

Las respuestas proporcionadas por el servidor son `OK` en caso de éxito y `ERR` en caso de error seguido de un texto explicativo.

POP3 tiene algunas desventajas que se solucionaron con la implementación de **IMAP4**, que se caracteriza por:

* Permitir organización por carpetas en la MTA.
* Mantener información entre sesiones.
* Permitir descargas de parte de los mensajes.
* Es posible acceder mediante varios clientes.

### Lista de puertos

* **POP3**: 110
* **IMAP**: 143
* **SMTP**: 25
* **HTTP**: 80

## Protocolos seguros

Los objetivos principales del intercambio seguro de información son:

* **Confidencialidad**: Solo accede a la información quien debe hacerlo.
* **Responsabilidad**: Asumir consecuencias "legales".
  * **Autenticación**: Asegurarse que emisor y receptor son quienes dicen ser.
  * **No repudio**: No poder negar el autor de una acción.
  * **Control de accesos**: Garantía de identidad para el acceso a la información.
* **Integridad**: Detección de alteraciones de la información.
* **Disponibilidad**: Servicios disponibles a pesar de la demanda.

Los mecanismos de seguridad más reconocidos son:

* **Cifrado simétrico**: Los mensajes se encriptan con una clave y se descifran con la misma.
* **Cifrado asimétricos**: Los mensajes se encriptan con una clave y se descifran con otra.
* **Funciones Hash**: Se aplica al mensaje una función de la cual se obtiene un "resumen" único para éste.
* **Message Authentication Code**: Se aplica al mensaje un algoritmo MAC junto con una clave y se envía el mensaje junto con el MAC obtenido.
* **Firma digital**: El emisor cifra un resumen del mensaje con su clave privada para que el receptor obtenga el resumen aplicando la clave pública del emisor y así poder identificarlo.
* **Certificado**: Una entidad intermedia entre el emisor y el receptor se asegura del no repudio.
* **Intercambio de claves de Diffie-Hellman**: Permite establecer una clave compartida entre dos entidades a través de un canal no seguro.
* **Ataque man-in-the-middle**.



# Tema 3: Capa de Transporte en internet

## Protocolo de datagrama de usuario (UDP)

La principal característica de UDP es que ofrece un servicio no fiable y no orientado a conexión. Por ello y por su semejanza con IP, la TPDU (Transport Protocol Data Unit) se denomina indistintamenre *datagrama de usuario*, *datagrama UDP* o *paquete UDP*. Los campos del paquete UDP son:

* *Porigen*: Campo de 16 bits que especifica el puerto origen.
* *Pdestino*: Campo de 16 bits que especifica el puerto destino.
* *LongitudUDP*: 16 bits que indican cuantos octetos tiene el datagrama UDP completo.
* *Comprobación*: Contiene el complemento a uno de la suma en complemento a uno del datagrama y de una pseudocabecera.
* *Datos*: PDU de la capa superior.

Los cuatro primeros suponen la cabecera de UDP. Este protocolo no tiene control de flujo ni de errores.

En el cálculo de la suma de comprobación, se incluyen cuatro campos extras de la capa inferior (IP): *dirección IP origen*, *dirección IP destino*, *protocolo* y *longitudUDP* del datagrama. A esta estructura se le denomina *pseudocabecera* UDP.

El incluir en el cálculo del campo comprobación las IP origen y destino tiene como objetivo proteger a UDP de posibles errores en la capa IP.

Sobre el servicio IP, UDP solo añade la *multiplexación*. Es decir, introduce un esquema de direccionamiento extra - basado en los puertos - que permite identificar las aplicaciones origen y destino del datagrama.

Algunos puertos UDP preasignados son el 53 (DNS), 123 (NTP), 7 (echo), 13 (daytime). Los puertos del 0 al 1023 están asignados al sistema, a partir del 1024 son puertos del usuario.

UDP suele usarse en **aplicaciones multimedia** o **juegos online**, aplicaciones tolerantes a fallos y en las que se prima la interactividad a coste de asumir cierta probabilidad de error.

## Protocolo de control de transmisión (TCP)

TCP forma junto a IP el corazón de la arquitectura de internet. Sus principales características son:

* Servicio **punto a punto**, no válido para comunicaciones *multicast*.
* Servicio **orientado a conexión**, requiere un estado común entre emisor y receptor ("*handshaking*").
* **Entrega ordenada** de las secuencias de bytes generadas.
* Transmisión ***full-duplex***.
* Ofrece un mecanismo de **detección y recuperación de errores.**
* Servicio fiable con **control de flujo** y **control de congestión**.
* Incorporación de **confirmaciones**.

Las principales funcionalidades de TCP son: la multiplexación/demultiplexación de aplicaciones, control de la conexión, control de errores, control de flujo, control de conexión.

Algunos campos de la cabecera de TCP son:

* *Hlen*: Campo de 4 bits de valor mínimo igual a 5 que indica la longitud de la cabecera TCP.
* *Reservado*: Campo de 3 bits sin uso específico.
* *Datos*: Mensaje generado por la aplicación.
* *Opciones*: Campo de longitud variable que puede presentar dos formatos: un único octeto que define el tipo de opción o un octeto que indica el tipo, otro la longitud de los datos correspondientes a la opción, y finalmente los datos como tales.

### Multiplexación/demultiplexación de las aplicaciones

Al igual que en UDP, dos aplicaciones ejecutadas en un mismo host pueden comunicarse mediante TCP. Para ello se usan los campos de 16 bits *porigen* y *pdestino*, que especifican los puertos origen y destino de la comunicación y en definitiva identifican al proceso generador y consumidor de la información transportada.

Algunas aplicaciones usuarias de TCP y sus puertos asociados son FTP (puerto 21), SMTP (puerto 25) o HTTP (puerto 80). Algunas aplicaciones como daytime o echo pueden usar tanto TCP como UDP en el mismo puerto. Las aplicaciones que ofrecen un servicio en el mismo puerto independientemente del protocolo se conocen como *servicios multiprotocolo*.

Cabe destacar que TCP multiplexa y demultiplexa aplicaciones estableciendo conexiones entre ellas. Una conexión está unívocamente definida por la dirección y puerto orígenes y la dirección y puerto destino.

### Control de la conexión

TCP ofrece un servicio orientado a conexión, por lo que es necesario un establecimiento  de la conexión entre las entidades previa al intercambio de datos y un cierre de la conexión. Ambos procedimientos involucran los bits *A*, *S* y *F* del campo *control*:

*  **Establecimiento de la conexión**: En la capa de red TCP se apoya en el servicio no fiable que ofrece IP, luego el establecimiento de la conexión debe tener en cuenta que los segmentos intercambiados se pueden perder. Estrictamente hablando es imposible garantizar el éxito en esta tarea, ya que el canal utilizado no está libre de errores. Para paliar este hecho, en el establecimiento de la conexión se adopta el *procedimiento a tres pasos* ("three way handshake"):

  * **Solicitud de conexión**: Uno de los extremos envía un segmento (sólo cabecera) con el bit *S* (SYN) activado y con un número de *secuencia* *X* (apertura activa).  De esta forma sincronizamos las entidades.

  * **Aceptación de la conexión y solicitud en el otro sentido**: El otro extremo reserva recursos en el SO y responde con un segmento con el bit ACK activado y *acuse=X+1*.

    Al ser *full-duplex*, es necesario establecer la conexión en ambos sentidos, para ello, en este mismo segmento se activa el bit SYN y se elige el número de *secuencia* Y (apertura pasiva).

  * **Conexión establecida**: La conexión se establece una vez que el extremo que solicitó la  conexión reserve recursos en el SO y envíe un segmento TCP con *acuse=Y+1* y *ACK=1*.

* **Cierre de la conexión**: Ahora el bit involucrado es *F* (FIN).

  * **Solicitud de cierre**: Cualquiera de las dos entidades cuando no tenga más datos que transmitir enviará un segmento TCP con el bit FIN=1 y *secuencia=X* en uno de los sentidos (cierre activo).
  * **Aceptación de cierre y solicitud en el otro sentido**: El otro extremo debe confirmar la solicitud recibida, enviando un segmento con el bit ACK activado y *acuse=X+1*. Si esta entidad ha terminado de transmitir sus datos, en ese mismo segmento activará el bit FIN y especificará el número de secuencia Y. Esto último no es necesariamente obligatorio, se puede solicitar el cierre de la conexión más tarde, cuando se hayan acabado de transmitir los datos en ese sentido (cierre pasivo).
  * **Conexión finalizada**: Cuando el extremo que solicitó el cierre recibe el segmento descrito en el apartado anterior, se cierra la conexión al enviar un segmento con ACK activado y *acuse=Y+1*.

### Control de errores

Con el fin de mejorar la eficacia y el rendimiento, se adopta un esquema de ventana deslizante y un sistema ARQ con confirmaciones positivas y acumulativas: 

1. Cada segmento se numera por parte del emisor y se envía. En ese momento se almacena una copia en la ventana de emisión y se inicia un contador hasta que se recibe la confirmación asociada a ese segmento, lo cual se traduce en dar por realizada la transmisión y desplazar si procede la ventana de emisión.
2. En el receptor, tras el análisis del campo de *comprobación* y ver que el número de secuencia está entre los definidos por la ventana de recepción, se devuelve la confirmación y puede que acumulativa.
3. Si el temporizador alcanza un valor determinado llamado *timeout* se concluye que ha habido un error y se reenvían los datos correspondientes.

 Se involucran los campos:

* *Secuencia*: Campo de 32 bits que indica el número de byte que ocupa el primer octeto del campo *datos* en el mensaje que genera la aplicación.
* *Acuse*: Campo de confirmación de 32 bits que indica el número de byte que se espera recibir en el destino. Recibir un determinado valor de *acuse* confirma todos los bytes pendientes hasta justo el anterior.
* *el bit A (ACK) del campo control*: Indica la validez o no de la confirmación especificada en el campo *acuse*. 
* *Campo comprobación*: Permite al receptor determinar si el segmento se ha recibido correctamente o no.

Posibles eventos y acciones en el receptor TCP:

* Llegada ordenada de un segmento, sin discontinuidad, todo lo anterior ya confirmado.
  * Retrasar ACK hasta recibir el siguiente segmento o hasta 500 ms.
* Llegada ordenada de un segmento, sin discontinuidad pero hay un ACK pendiente.
  * Enviar inmediatamente un único ACK confirmando los dos segmentos.
* Llegada desordenada de un segmento con número de secuencia mayor que el esperado, discontinuidad detectada.
  * Enviar ACK duplicado indicando el número de secuencia del siguiente byte esperado.
* Llegada de un segmento que completa una discontinuidad parcial o totalmente.
  * Confirmar ACK inmediatamente si el segmento comienza en el extremo inferior de la discontinuidad.

La eficacia y el rendimiento de TCP dependerá del valor de *timeout*, debe ser cercano al tiempo de ida y vuelta entre origen y destino (*RTT*), pero si es muy pequeño se provocarán timeouts prematuros y si es demasiado grande habrá una reacción lenta ante fallos. La mejor solución a estos problemas es adoptar un  esquema adaptable. El sistema es el siguiente:

1. Cada vez que se transmite un segmento, el emisor registra el instante en el que se envía y el instante en el que se recibe la confirmación. La diferencia es el RTT instantáneo para el segmento actual, $RTT_{actual}$.

2. Para cada segmento enviado se obtiene un nuevo RTT, $RTT_{nuevo}$, calculado mediante la expresión:
   $$
   RTT_{nuevo}=\alpha\cdot RTT_{viejo}+(1-\alpha)\cdot RTT_{actual} \hspace{1cm}\alpha\in[0,1]
   $$
   donde $RTT_{viejo}$ es valor RTT anterior y $\alpha$ un parámetro que regula qué importancia tiene el valor instantáneo con respecto al histórico.

3. Por último, el valor de $t_{out}$ se establece mediante un suavizado de la desviación:
   $$
   dev_{nuevo}=(1-\beta)\cdot dev_{vieja}+\beta(|RTT_{nuevo}-RTT_{actual}|)\hspace{1cm}\beta\in[0,1]\\
   t_{out}=RTT_{nuevo}+4\cdot dev_{nuevo}
   $$




Esta estimación produce una dificultad. Cuando se produce un timeout y se reenvía el segmento, no se puede distinguir si la próxima confirmación corresponde al segmento original o al reenviado (*ambigüedad en las confirmaciones*). Este hecho dispara el *algoritmo de Karn*: actualizar el RTT sólo cuando no se produzcan ambigüedades, pero si se produce un timeout, se modifica t_{out}.
$$
t_{outnuevo}=\gamma\cdot t_{outviejo}
$$
siendo 2 un valor típico del parámetro $\gamma$.



# Tema 4: Redes conmutadas e internet

## Conmutación

La **conmutación** es la acción de establecer un camino o comunicación extremo a extremo. Dos tipos fundamentales: de circuitos y de paquetes

### Conmutación de circuitos

Utilizada comunmente en la comunicación telefónica, caracterizada por el establecimiento de una conexión origen-destino previa a la transmisión de datos que se debe cerrar al acabar dicha transmisión. Los pasos involucrados son:

1. **Establecimiento de la conexión**: Elección de la ruta origen-destino a seguir por el mensaje que implica la reserva de los recursos necesarios.
2. **Transmisión**: Intercambio de datos propiamente dicho entre estaciones finales. El mensaje se transfiere de forma secuencial de principio a fin y siguiendo la ruta establecida en la conexión.
3. **Cierre de conexión**: Liberación de los recursos de la subred asociada a la conexión.

### Conmutación de paquetes

La conexión establecida en la conmutación de circuitos deriva en la infrautilización de los recursos de la subred, para evitarlo surgió la técnica de conmutación de paquetes. El proceso de transmisión seguido se fundamenta en:

1. No se establece conexión previa a la transmisión de datos.
2. El mensaje se divide en bloques más pequeños denominados *paquetes* o *datagramas*.
3. Cada paquete se transmite de forma independiente al resto, pueden seguir caminos distintos y llegar al origen de forma desordenada

Para posibilitar el envío individualizado cada paquete debe contener información como el destino al que va dirigido.

La transmisión se realiza en forma de almacenamiento y reenvío. Es decir, los nodos intermedios almacenan temporalmente cada paquete y lo procesan antes de llevar a cabo su retransmisión. Esto supone un tiempo de procesamiento en la retransmisión de todos los paquetes del mensaje.

Para solucionar el problema del retardo causado por el procesamiento en cada nodo surge la variante de *conmutación de paquetes mediante circuitos virtuales*, que mezcla rasgos de las dos configuraciones. Se establece una conexión entre las fuentes finales pero no se reservan recursos en los nodos, sino que pueden ser compartidos entre varias comunicaciones. El mensaje se divide en paquetes que se transmiten siguiendo la línea establecida en la conexión pero con almacenamiento y reenvío. Finalizada la transmisión, se cierra la conexión.

##  Protocolo IP

### Fragmentación IPv4

De acuerdo a su formato, el máximo tamaño que puede tener un datagrama IP es de 65535 octetos, de los cuales un máximo de 65515 corresponden a datos. El transporte de una cantidad tal de datos supera la MTU (Unidad Máxima de Transferencia) usual. Para solucionar esto, se fragmentan los datagramas IP. Para ello se usan los campos *identificación*, *indicadores* y *desplazamiento*.

* *Desplazamiento* es un campo de 13 bits que se utiliza como puntero que señala la posición relativa del primer byte del fragmento dentro del datagrama original. Esta medida no viene dada en octetos, sino en grupos de 8 bytes (64 bits).

  Adicionalmente, todos los fragmentos deben tener el mismo valor en el campo *identificación*, el valor que tenía en el datagrama original, para así permitir su ensamblado en el receptor.

* El campo *indicadores* (3 bits) también está relacionado con esta funcionalidad. El bit más a la izquierda no se utiliza, el de su derecha se denomina DF (Don't Fragment), y sirve para indicar que este mensaje no se puede fragmentar. El tercer byte se denomina MF (More Fragments) y sirve para indicar si el segmento recibido es el último o no.

## Asociación con Capa de Enlace: El protocolo ARP

Tras conocer la dirección IP destino, un *host* o un router deben conocer la dirección MAC (dirección física) del siguiente nodo. El formato de las direcciones MAC es de 6 bytes: HH:HH:HH:HH:HH:HH, por ejemplo: 00:24:21:a8:f7:6a.

Para ello se utiliza el protocolo ARP (Address Resolution Protocol). Este funciona de forma que se envía un paquete ARP (request) a la dirección de difusión (ff:ff:ff:ff:ff:ff) que contiene la dirección IP correspondiente a la MAC solicitada. La entidad que tenga esa IP responderá con otro paquete proporcionando su dirección MAC, que es la deseada por el emisor.

Cabe destacar que las direcciones MAC son únicas y que existe un protocolo análogo que hace la operación contraria, es decir, a partir de la dirección MAC obtiene la dirección IP. Este protocolo se conoce como RARP (Reverse Address Resolution Protocol).

## ICMP (Internet Control Message Protocol)

ICMP es al igual que IP un protocolo de red, pero no es una alternativa, sino un complemento al mismo. De hecho, ICMP es usuario de IP, sus mensajes se encapsulan dentro de paquetes IP.

ICMP define un conjunto de mensajes para notificar ciertas situaciones. La cabecera es de 32 bits y tiene los siguientes campos:

* Tipo (8 bits): Especifica el tipo de mensaje.
* Código (8 bits): Identifica un subtipo.
* Comprobación (8 bits): Complemento a 1 de la suma en complemento a 1 del resto de elementos de la cabecera, usado para el control de errores.

Los mensajes más relevantes son:

* **Eco**: Usado para testear la accesibilidad de cierto destino. Implica la consideración de dos mensajes, uno de solicitud (Tipo=8), y uno de respuesta (Tipo=0).
* **Destino inalcanzable**: (Tipo=3) Se genera cuando el destino IP especificado en un datagrama no es accesible.
* **Ralentización del origen**: (Tipo=4) Notificación explícita de congestión.
* **Redireccionamiento**: (Tipo=5) La ruta indicada para un destino no es la mejor, sino que resulta más adecuada la que pasa por el dispositivo cuya IP viene especificada en el paquete ICMP.
* **Tiempo excedido**: Cuando el campo TTL de cierto datagrama llega a 0, se envía una notificación al emisor (Tipo=11).
* **Problema de parámetros:** (Tipo=12) Ocurrencia de un evento distinto a los anteriores.
* **Sello de tiempo**: En ocasiones resulta interesante conocer el tiempo de ida y vuelta de un mensaje. La solicitud (Tipo=13) especifica por parte del emisor el momento en que se envía el mensaje. La respuesta (Tipo=14) incluye el instante de recepción y el de generación de la respuesta.
* **Máscara de red**: Solicitud de la máscara de red (Tipo=17) y respuesta (Tipo=18).