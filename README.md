# c-server-lite
Little project to understand how basic html server work and exercise c-sockets.
Based on Nir Lichtman's YouTube video "Making Minimalist Web Server in C on Linux"

# Servidor HTTP en C

Un servidor web básico hecho en C con sockets. Escucha en el puerto 8080
y sirve archivos HTML. Lo hice para aprender cómo funciona HTTP por debajo 
aprender a abrir puertos del router y desoxidar los sockets de la asignatura 
de Redes de Computadores de la Universidad.
Lo tengo corriendo en una Raspberry Pi expuesto al público (Está cerrado ahora 
mismo para ahorrar algo de recursos y prevenir riesgos al ser un proyecto
muy poco seguro).

## Cómo se compila y ejecuta

    gcc server.c -o server
    ./server

Después ya estaría operativo accediendo a la IP pública de la Pi Zero 2W

## Resumen de la operativa

  1) Abre un socket y escucha conexiones (por TCP).
  2) Lee la petición y saca el archivo que piden.
  3) Si existe, lo envía con "sendfile". Si no, devuelve un 404.

## Cosas por añadir

- Ahora mismo sirve siempre "index.html", podría hacer un servidor web dinámico.
- Podría protegerse la conexión añadiendo TLS con OpenSSL o similares.
- Solo maneja una conexión cada vez, podría añadirse concurrencia con threads.

Empleada la guía de Beej's para Network Programming
https://beej.us/guide/bgnet/html/
