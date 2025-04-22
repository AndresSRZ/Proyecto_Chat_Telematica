# Proyecto Telematica - Chat Multicliente en C

## Link del video
- https://drive.google.com/file/d/1H1hMWdLrRqGstRVCgXwvdWOkpvljvtWK/view?usp=sharing

## Introducción

El presente proyecto tiene como objetivo desarrollar un sistema de chat funcional utilizando sockets en lenguaje C bajo una arquitectura cliente-servidor.  
Se implementa el protocolo TCP/IP para garantizar una comunicación confiable entre múltiples clientes conectados simultáneamente.  
Este proyecto permite comprender los principios de la programación en red, el manejo de concurrencia usando hilos (pthread) y la gestión eficiente de múltiples conexiones en tiempo real.
Podra encontrar un pdf en la seccion de archivos donde podra observar como ejecutar correctamente el proyecto. 

---

## Desarrollo

El sistema se compone de dos siguientes aplicaciones principales:

### 1. Servidor de Chat (Se creo una maquina virtual como servidor)
- Programado en C utilizando la API de sockets de Berkeley.
- Escucha conexiones entrantes en el puerto 8080.
- Acepta múltiples clientes simultáneamente utilizando *hilos* (pthread).
- Recibe mensajes de los clientes y los *retransmite* a todos los demás conectados.
- Gestiona la desconexión de clientes de manera segura.
- Maneja concurrencia de los datos sobre la sesion. (Momento de la accion, quien se conecto, que mensaje mando y quien se desconecto). 

### 2. Cliente de Chat (Se creo una maquina virtual distinta por cada cliente para verificar la capacidad del servidor y el funcionamiento correcto del chat. Creamos 2 clientes en total).
- Programado en C utilizando la API de sockets de Berkeley.
- Solicita el *nombre de usuario* al iniciar la sesión.
- Permite el *envío y recepción* de mensajes en paralelo utilizando hilos (pthread).
- Permite salir del chat escribiendo exit o quit.
- Optimiza la interfaz de terminal para un flujo de chat limpio y ordenado.

*Tecnologías utilizadas*:
- Lenguaje: C
- Librerías: sys/socket.h, netinet/in.h, arpa/inet.h, pthread.h
- Entorno de despliegue: Ubuntu Server (AWS EC2 Instances)

---

## Aspectos logrados y no logrados

### Aspectos logrados
- Conexión simultánea de múltiples clientes al servidor.
- Comunicación en tiempo real entre los clientes.
- Implementación correcta del protocolo TCP/IP.
- Manejo de concurrencia mediante hilos (pthread).
- Sistema de identificación de usuarios mediante nombre.
- Permitir desconexión voluntaria del cliente (exit, quit).
- Flujo de chat ordenado y visualmente limpio en terminal.
- Implementa persistencia de mensajes (se guardan en el archivo, chat.log).


### Aspectos no logrados
- No se implementa autenticación avanzada (contraseñas o roles de usuarios).
- No se implementa reconexión automática de clientes después de desconexión.

---

## Conclusiones

Este proyecto permitió aplicar conceptos fundamentales de la programación en red, el manejo de sockets TCP/IP y la creación de aplicaciones concurrentes mediante hilos.  
La experiencia adquirida en la construcción de un sistema de chat real resalta la importancia del diseño de sistemas distribuidos confiables y eficientes.  
Además, se destacó la importancia de manejar adecuadamente las entradas y salidas en entornos de múltiples usuarios para garantizar una buena experiencia de usuario.  

---

## Referencias

- *Beej's Guide to Network Programming*: https://beej.us/guide/bgnet/
- *Beej's "Bgnet" Repository*: https://github.com/beejjorgensen/bgnet
- *The Linux Programming Interface* - Michael Kerrisk
- *Sockets programming* - Think and learn: https://www.youtube.com/playlist?list=PLPyaR5G9aNDvs6TtdpLcVO43_jvxp4emI 
- *Sockets - Introducción para Programadores en C* - Programación en lenguaje C en Linux: https://www.youtube.com/watch?v=V4ItxHko6ec&list=PL19snTOMdnWupnb1vcTfjmp6BdNh_YxOp
- *API de sockets de Berkeley* - Documentación oficial
