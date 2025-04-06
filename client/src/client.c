#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	logger = iniciar_logger();
	log_info(logger, "Hola soy un logger");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
	config = iniciar_config();

	/* Usando el config creado previamente, leemos los valores del config y los dejamos 
	en las variables 'ip', 'puerto' y 'valor'*/
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	
	// Loggeamos el valor de config
	log_info(logger,"Valor: %s", valor);
	log_info(logger, "IP: %s", ip);
	log_info(logger,"Puerto: %s", puerto);
	
	/* ---------------- LEER DE CONSOLA ---------------- */
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "cliente", true, LOG_LEVEL_INFO);

	if(nuevo_logger == NULL){
		printf("No se pudo crear el logger\n");
        abort(); //Mas abrupto que Exit(1)
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create ("cliente.config");

	if (nuevo_config == NULL) {
        printf("No se pudo abrir el archivo cliente.config");
        exit(1); // No termina el programa abruptamente como abort()
    }
	
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	while (strcmp(leido, "") != 0) {
            free(leido);
			leido = readline(">");
			log_info(logger, "%s",leido);
            break;
        }

        free(leido);  // readline usa malloc, hay que liberar la memoria
    }


void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("Enviar mensaje al servidor: ");
	agregar_a_paquete(paquete, leido, strlen(leido) + 1);
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	free(leido);
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  liberar_conexion(conexion);
	  log_destroy(logger);
	  config_destroy(config);
}
