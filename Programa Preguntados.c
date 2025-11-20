#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>    // Para _kbhit() y getch()

void limpiar_pantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --- Función 'leer_con_tiempo' MEJORADA ---
int leer_con_tiempo(int segundos_max) {

    int tiempo = 0;
    char buffer[10];
    int pos = 0;

    // <<< CORREGIDO: Cálculo para un temporizador responsivo >>>
    // 1000ms / 50ms = 20 iteraciones por segundo
    int iteraciones_por_segundo = 20;
    int iter_actual = 0;

    printf("\n(Tienes %d segundos para responder)\n", segundos_max);

    // Limpiar el buffer
    buffer[pos] = '\0';

    while (tiempo < segundos_max) {

        if (_kbhit()) {    // Si el usuario escribe algo
            char c = getch();

            // <<< CORREGIDO: Lógica para BORRAR (Backspace) >>>
            if (c == 8) { // 8 es el código ASCII de Backspace
                if (pos > 0) {
                    pos--;
                    buffer[pos] = '\0';
                    printf("\b \b"); // Mueve cursor atrás, imprime espacio, mueve atrás
                }
            }
            // <<< CORREGIDO: Lógica para ENTER (solo si se ha escrito algo) >>>
            else if (c == '\r' && pos > 0) {  // Enter -> fin de entrada
                return atoi(buffer);
            }
            // <<< CORREGIDO: Lógica para NÚMEROS (y evitar desbordamiento) >>>
            else if (c >= '0' && c <= '9' && pos < 9) {
                printf("%c", c);
                buffer[pos++] = c;
                buffer[pos] = '\0'; // Siempre mantener el fin de cadena
            }
        }

        // <<< CORREGIDO: Temporizador responsivo >>>
        Sleep(50);    // Esperar solo 50ms en lugar de 1000ms
        iter_actual++;

        // Si se completó 1 segundo (20 iteraciones * 50ms)
        if (iter_actual >= iteraciones_por_segundo) {
            tiempo++;
            iter_actual = 0;
        }
    }

    return -1;    // Tiempo agotado
}

int main() {

    int opcion, respuesta;
    int puntaje_total = 0;
    int matematicas_100 = 0;
    int fisica_100 = 0;
    int quimica_100 = 0;

    do {
        limpiar_pantalla();
        printf("==============================\n");
        printf("         PREGUNTADOS\n");
        printf("==============================\n");
        printf("Puntaje total: %d\n\n", puntaje_total);
        printf("Escoge la categoria:\n");
        printf("1. Matematicas%s\n", matematicas_100 ? " (100%)" : "");
        printf("2. Fisica%s\n",      fisica_100      ? " (100%)" : "");
        printf("3. Quimica%s\n",      quimica_100     ? " (100%)" : "");
        printf("4. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if (opcion == 4) {
            limpiar_pantalla();
            printf("Gracias por jugar!\n");
            printf("Puntaje final: %d\n", puntaje_total);
            break;
        }

        const char *preguntas[10];
        const char *opciones[10][4];
        int correcta[10];
        int respuestas_usuario[10];

        switch (opcion) {

        case 1: // MATEMATICAS
            preguntas[0] = "15 * 3";
            opciones[0][0] = "30"; opciones[0][1] = "45"; opciones[0][2] = "35"; opciones[0][3] = "50";
            correcta[0] = 2;

            preguntas[1] = "Raiz de 81";
            opciones[1][0] = "8"; opciones[1][1] = "9"; opciones[1][2] = "7"; opciones[1][3] = "6";
            correcta[1] = 2;

            preguntas[2] = "20 / 5";
            opciones[2][0] = "5"; opciones[2][1] = "6"; opciones[2][2] = "4"; opciones[2][3] = "3";
            correcta[2] = 3; // <<< CORREGIDO (Era 1)

            preguntas[3] = "12 * 12";
            opciones[3][0] = "124"; opciones[3][1] = "132"; opciones[3][2] = "144"; opciones[3][3] = "122";
            correcta[3] = 3;

            preguntas[4] = "Raiz de 100";
            opciones[4][0] = "8"; opciones[4][1] = "10"; opciones[4][2] = "12"; opciones[4][3] = "9";
            correcta[4] = 2;

            preguntas[5] = "50 / 2";
            opciones[5][0] = "20"; opciones[5][1] = "30"; opciones[5][2] = "25"; opciones[5][3] = "15";
            correcta[5] = 3;

            preguntas[6] = "7 * 8";
            opciones[6][0] = "56"; opciones[6][1] = "54"; opciones[6][2] = "49"; opciones[6][3] = "64";
            correcta[6] = 1;

            preguntas[7] = "9 * 9";
            opciones[7][0] = "81"; opciones[7][1] = "72"; opciones[7][2] = "99"; opciones[7][3] = "90";
            correcta[7] = 1;

            preguntas[8] = "100 - 45";
            opciones[8][0] = "65"; opciones[8][1] = "55"; opciones[8][2] = "45"; opciones[8][3] = "60";
            correcta[8] = 2;

            preguntas[9] = "25 + 40";
            opciones[9][0] = "50"; opciones[9][1] = "60"; opciones[9][2] = "70"; opciones[9][3] = "65";
            correcta[9] = 4; // <<< CORREGIDO (Era 3)

            break;

        case 2: // FISICA (Sin errores)
            preguntas[0] = "Unidad de fuerza";
            opciones[0][0] = "Joule"; opciones[0][1] = "Newton"; opciones[0][2] = "Pascal"; opciones[0][3] = "Watt";
            correcta[0] = 2;

            preguntas[1] = "Velocidad de la luz";
            opciones[1][0] = "3x10^8 m/s"; opciones[1][1] = "3x10^6 m/s"; opciones[1][2] = "3x10^3 m/s"; opciones[1][3] = "3x10^5 m/s";
            correcta[1] = 1;

            preguntas[2] = "Ley F = m * a";
            opciones[2][0] = "Gravitacion"; opciones[2][1] = "2da Ley Newton"; opciones[2][2] = "Ohm"; opciones[2][3] = "Pascal";
            correcta[2] = 2;

            // ... (resto de preguntas de física) ...
            preguntas[3] = "Unidad de energia";
            opciones[3][0] = "Newton"; opciones[3][1] = "Joule"; opciones[3][2] = "Lux"; opciones[3][3] = "Tesla";
            correcta[3] = 2;
            preguntas[4] = "Unidad de potencia";
            opciones[4][0] = "Watt"; opciones[4][1] = "Newton"; opciones[4][2] = "Joule"; opciones[4][3] = "Volt";
            correcta[4] = 1;
            preguntas[5] = "Aceleracion de la gravedad";
            opciones[5][0] = "8 m/s2"; opciones[5][1] = "9.8 m/s2"; opciones[5][2] = "10 m/s2"; opciones[5][3] = "12 m/s2";
            correcta[5] = 2;
            preguntas[6] = "Unidad de Presion";
            opciones[6][0] = "Joule"; opciones[6][1] = "Pascal"; opciones[6][2] = "Newton"; opciones[6][3] = "Bar";
            correcta[6] = 2;
            preguntas[7] = "Unidad de carga electrica";
            opciones[7][0] = "Volt"; opciones[7][1] = "Ampere"; opciones[7][2] = "Coulomb"; opciones[7][3] = "Ohm";
            correcta[7] = 3;
            preguntas[8] = "Unidad de Voltaje";
            opciones[8][0] = "W"; opciones[8][1] = "A"; opciones[8][2] = "V"; opciones[8][3] = "C";
            correcta[8] = 3;
            preguntas[9] = "la formula de Trabajo =";
            opciones[9][0] = "F * d"; opciones[9][1] = "m * g"; opciones[9][2] = "P * t"; opciones[9][3] = "m * v";
            correcta[9] = 1;

            break;

        case 3: // QUIMICA (Sin errores)
            preguntas[0] = "Simbolo del sodio";
            opciones[0][0] = "So"; opciones[0][1] = "Sd"; opciones[0][2] = "Na"; opciones[0][3] = "S";
            correcta[0] = 3;

            preguntas[1] = "Numero atomico del Oxigeno";
            opciones[1][0] = "6"; opciones[1][1] = "7"; opciones[1][2] = "8"; opciones[1][3] = "10";
            correcta[1] = 3;

            preguntas[2] = "Enlace que comparte electrones";
            opciones[2][0] = "Ionico"; opciones[2][1] = "Covalente"; opciones[2][2] = "Metalico"; opciones[2][3] = "Nuclear";
            correcta[2] = 2;

            // ... (resto de preguntas de química) ...
            preguntas[3] = "PH Acido";
            opciones[3][0] = "2"; opciones[3][1] = "7"; opciones[3][2] = "10"; opciones[3][3] = "12";
            correcta[3] = 1;
            preguntas[4] = "Simbolo del Potasio";
            opciones[4][0] = "P"; opciones[4][1] = "Po"; opciones[4][2] = "Pt"; opciones[4][3] = "K";
            correcta[4] = 4;
            preguntas[5] = "Estado del agua a 100C";
            opciones[5][0] = "Liquido"; opciones[5][1] = "Gas"; opciones[5][2] = "Solido"; opciones[5][3] = "Plasma";
            correcta[5] = 2;
            preguntas[6] = "Tabla Periodica creada por:";
            opciones[6][0] = "Newton"; opciones[6][1] = "Einstein"; opciones[6][2] = "Mendeleiev"; opciones[6][3] = "Curie";
            correcta[6] = 3;
            preguntas[7] = "Numero atomico del Hidrogeno";
            opciones[7][0] = "1"; opciones[7][1] = "2"; opciones[7][2] = "3"; opciones[7][3] = "0";
            correcta[7] = 1;
            preguntas[8] = "Simbolo del hierro";
            opciones[8][0] = "I"; opciones[8][1] = "H"; opciones[8][2] = "Ir"; opciones[8][3] = "Fe";
            correcta[8] = 4;
            preguntas[9] = "Mezcla homogenea";
            opciones[9][0] = "Arena y agua"; opciones[9][1] = "Aceite y agua"; opciones[9][2] = "Sal y agua"; opciones[9][3] = "Tierra";
            correcta[9] = 3;

            break;
        }

        // ---- BUCLE DE PREGUNTAS ----
        // (No se necesita 'if (opcion >= 1 && opcion <= 3)' si 'default' está vacío)
        if (opcion >= 1 && opcion <= 3) {
            int i;
            int puntaje_ronda = 0;

            for (i = 0; i < 10; i++) {
                limpiar_pantalla();
                printf("Pregunta %d/10:\n", i + 1);
                printf("%s\n", preguntas[i]);
                printf("1) %s\n2) %s\n3) %s\n4) %s\n",
                        opciones[i][0], opciones[i][1], opciones[i][2], opciones[i][3]);

                printf("Respuesta: ");

                respuesta = leer_con_tiempo(15); // <<< TIEMPO LÍMITE (usa la función mejorada)

                if (respuesta == -1) {
                    printf("\nTiempo agotado!\n");
                } else {
                    respuestas_usuario[i] = respuesta;

                    if (respuesta == correcta[i]) {
                        printf("\nRespuesta correcta!\n");
                        puntaje_ronda++;
                    } else {
                        printf("\nRespuesta incorrecta.\n");
                    }
                }

                Sleep(1000); // <<< CORREGIDO: Pausa reducida a 1 segundo
            }

            if (puntaje_ronda == 10) {
                switch (opcion) {
                    case 1: matematicas_100 = 1; break;
                    case 2: fisica_100 = 1; break;
                    case 3: quimica_100 = 1; break;
                }
            }

            puntaje_total += puntaje_ronda;

            limpiar_pantalla();
            printf("Ronda terminada.\n");
            printf("Puntaje obtenido: %d/10\n", puntaje_ronda);
            printf("Puntaje total: %d\n", puntaje_total);

            printf("\nPresione ENTER para volver al menu...");
            getchar(); getchar();
        }

    } while (1);

    return 0;
}
