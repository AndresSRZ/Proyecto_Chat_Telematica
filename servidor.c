// servidor_multicliente.c (con logs)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>

#define PUERTO 8080
#define MAX_CLIENTES 100

int clientes[MAX_CLIENTES];
char nombres[MAX_CLIENTES][32];
int num_clientes = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;

// Función para escribir en el log
void escribir_log(const char *mensaje) {
    pthread_mutex_lock(&log_lock);
    FILE *log_file = fopen("chat.log", "a");
    if (log_file != NULL) {
        time_t ahora = time(NULL);
        struct tm *tm_info = localtime(&ahora);
        char tiempo[26];
        strftime(tiempo, 26, "%Y-%m-%d %H:%M:%S", tm_info);

        fprintf(log_file, "[%s] %s\n", tiempo, mensaje);
        fclose(log_file);
    }
    pthread_mutex_unlock(&log_lock);
}

// Función para enviar mensaje a todos los clientes
void enviar_a_todos(char *mensaje, int emisor) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < num_clientes; i++) {
        if (clientes[i] != emisor) {
            send(clientes[i], mensaje, strlen(mensaje), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

// Función para manejar un cliente
void *manejar_cliente(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char buffer[1024];
    char mensaje_final[1056];
    int valread;
    int id_cliente;

    pthread_mutex_lock(&lock);
    id_cliente = num_clientes;
    clientes[num_clientes++] = sock;
    pthread_mutex_unlock(&lock);

    // Recibir nombre de usuario
    memset(buffer, 0, sizeof(buffer));
    valread = read(sock, buffer, sizeof(buffer));
    if (valread <= 0) {
        close(sock);
        pthread_exit(NULL);
    }
    buffer[strcspn(buffer, "\n")] = 0;
    strcpy(nombres[id_cliente], buffer);

    printf("Cliente conectado: %s\n", nombres[id_cliente]);

    char log_msg[150];
    snprintf(log_msg, sizeof(log_msg), "Cliente conectado: %s", nombres[id_cliente]);
    escribir_log(log_msg);

    // Escuchar mensajes del cliente
    while ((valread = read(sock, buffer, sizeof(buffer))) > 0) {
        buffer[valread] = '\0';

        snprintf(mensaje_final, sizeof(mensaje_final), "%s: %s", nombres[id_cliente], buffer);
        printf("%s", mensaje_final);

        escribir_log(mensaje_final);

        enviar_a_todos(mensaje_final, sock);

        memset(buffer, 0, sizeof(buffer));
        memset(mensaje_final, 0, sizeof(mensaje_final));
    }

    // Cliente desconectado
    printf("Cliente desconectado: %s\n", nombres[id_cliente]);

    snprintf(log_msg, sizeof(log_msg), "Cliente desconectado: %s", nombres[id_cliente]);
    escribir_log(log_msg);

    pthread_mutex_lock(&lock);
    for (int i = id_cliente; i < num_clientes - 1; i++) {
        clientes[i] = clientes[i + 1];
        strcpy(nombres[i], nombres[i + 1]);
    }
    num_clientes--;
    pthread_mutex_unlock(&lock);

    close(sock);
    free(socket_desc);
    pthread_exit(NULL);
}

int main() {
    int servidor_fd, nuevo_socket, *nuevo_sock;
    struct sockaddr_in direccion;
    int opt = 1;
    int addrlen = sizeof(direccion);
    pthread_t hilo_cliente;

    // Crear socket
    if ((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Error en setsockopt");
        exit(EXIT_FAILURE);
    }

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(PUERTO);

    if (bind(servidor_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    if (listen(servidor_fd, MAX_CLIENTES) < 0) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor de chat esperando conexiones en el puerto %d...\n", PUERTO);

    while (1) {
        nuevo_socket = accept(servidor_fd, (struct sockaddr )&direccion, (socklen_t)&addrlen);
        if (nuevo_socket < 0) {
            perror("Error en accept");
            continue;
        }

        nuevo_sock = malloc(1);
        *nuevo_sock = nuevo_socket;

        if (pthread_create(&hilo_cliente, NULL, manejar_cliente, (void *)nuevo_sock) < 0) {
            perror("Error al crear hilo");
            free(nuevo_sock);
            continue;
        }

        pthread_detach(hilo_cliente);
    }

    close(servidor_fd);
    return 0;
}