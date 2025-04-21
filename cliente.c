// cliente.c (mejora visual)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PUERTO 8080

int sock;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *recibir_mensajes(void *arg) {
    char buffer[1024];
    int valread;

    while ((valread = read(sock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[valread] = '\0';

        pthread_mutex_lock(&lock);
        printf("\r\033[K%s\nTú: ", buffer); // \r limpia línea actual, \033[K limpia hacia el final
        fflush(stdout);
        pthread_mutex_unlock(&lock);

        memset(buffer, 0, sizeof(buffer));
    }

    printf("\nDesconectado del servidor.\n");
    exit(0);
}

int main() {
    struct sockaddr_in serv_addr;
    char mensaje[1024];
    char nombre[32];
    pthread_t hilo_receptor;

    // Crear socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nError al crear el socket\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PUERTO);

    // Cambiar IP del servidor
    if (inet_pton(AF_INET, "IP_DEL_SERVIDOR", &serv_addr.sin_addr) <= 0) {
        printf("\nDirección inválida o no soportada\n");
        return -1;
    }

    // Conectar
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nError de conexión\n");
        return -1;
    }

    printf("Conectado al servidor de chat.\n");

    // Enviar nombre
    printf("Ingresa tu nombre de usuario: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = 0;
    send(sock, nombre, strlen(nombre), 0);

    // Hilo para recibir mensajes
    if (pthread_create(&hilo_receptor, NULL, recibir_mensajes, NULL) < 0) {
        perror("Error al crear hilo receptor");
        return 1;
    }

    // Bucle para enviar mensajes
    while (1) {
        printf("Tú: ");
        fflush(stdout);
        fgets(mensaje, sizeof(mensaje), stdin);

        if (strncmp(mensaje, "exit", 4) == 0 || strncmp(mensaje, "quit", 4) == 0) {
            printf("Saliendo del chat...\n");
            close(sock);
            exit(0);
        }

        send(sock, mensaje, strlen(mensaje), 0);
    }

    close(sock);
    return 0;
}