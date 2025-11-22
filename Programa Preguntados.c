#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>

void limpiar_pantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --- Función 'leer_con_tiempo' ---
int leer_con_tiempo(int segundos_max) {
    int tiempo = 0;
    char buffer[10];
    int pos = 0;
    int iteraciones_por_segundo = 20;
    int iter_actual = 0;

    printf("\n(Tienes %d segundos para responder)\n", segundos_max);
    buffer[pos] = '\0';

    while (tiempo < segundos_max) {
        if (_kbhit()) {
            char c = getch();
            if (c == 8) { // Backspace
                if (pos > 0) {
                    pos--;
                    buffer[pos] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == '\r' && pos > 0) { // Enter
                return atoi(buffer);
            }
            else if (c >= '0' && c <= '9' && pos < 9) {
                printf("%c", c);
                buffer[pos++] = c;
                buffer[pos] = '\0';
            }
        }
        Sleep(50);
        iter_actual++;
        if (iter_actual >= iteraciones_por_segundo) {
            tiempo++;
            iter_actual = 0;
        }
    }
    return -1;
}

// --- Función auxiliar para mezclar opciones ---
void mezclar_opciones(char opciones[4][50], int *indice_correcto) {
    int i, j;
    char temp_str[50];
    for (i = 0; i < 4; i++) {
        int r = rand() % 4;

        // Intercambiar textos
        strcpy(temp_str, opciones[i]);
        strcpy(opciones[i], opciones[r]);
        strcpy(opciones[r], temp_str);

        // Si movimos la respuesta correcta, actualizamos el índice
        if (*indice_correcto == i + 1) *indice_correcto = r + 1;
        else if (*indice_correcto == r + 1) *indice_correcto = i + 1;
    }
}

int main() {
    // Inicializar la semilla aleatoria (IMPORTANTE para que cambien los números)
    srand(time(NULL));

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

        // Ahora usamos buffers de texto más grandes para construir las preguntas dinámicas
        char preguntas_txt[10][100];
        char opciones_txt[10][4][50];
        int correcta[10];
        int respuestas_usuario[10];
        int tiempos_limite[10]; // Array para guardar los tiempos variables

        // Variables auxiliares para matemáticas
        int n1, n2, n3, res_correcta, falsa1, falsa2, falsa3;

        switch (opcion) {

        case 1: // --- MATEMATICAS ---

            {
                int base = 2 + rand() % 4;
                int exp1 = 3 + rand() % 3;
                int exp2 = -2 + rand() % 2;
                int exp3 = 1;
                int exp_final = exp1 + exp2 - exp3;

                res_correcta = pow(base, exp_final);
                sprintf(preguntas_txt[0], "Valor de (%d^%d * %d^%d) / %d", base, exp1, base, exp2, base);

                // Generar distractores
                falsa1 = res_correcta + base;
                falsa2 = res_correcta * base;
                falsa3 = (res_correcta > 2) ? res_correcta - 2 : res_correcta + 5;

                sprintf(opciones_txt[0][0], "%d", res_correcta); // La correcta empieza en la pos 0 (opción 1)
                sprintf(opciones_txt[0][1], "%d", falsa1);
                sprintf(opciones_txt[0][2], "%d", falsa2);
                sprintf(opciones_txt[0][3], "%d", falsa3);
                correcta[0] = 1; // Inicialmente es la 1, luego se mezcla
                tiempos_limite[0] = 30;
            }

            // 2. Funciones: f(x)=Ax^2 - Bx + C, calcular f(val)
            {
                int A = 1 + rand() % 3; // 1 a 3
                int B = 1 + rand() % 4; // 1 a 4
                int C = 1 + rand() % 10; // 1 a 10
                int val = 3 + rand() % 3; // x será 3, 4 o 5

                res_correcta = (A * val * val) - (B * val) + C;
                sprintf(preguntas_txt[1], "Si f(x)=%dx^2 - %dx + %d, cual es f(%d)?", A, B, C, val);

                falsa1 = res_correcta + 10;
                falsa2 = res_correcta - 5;
                falsa3 = res_correcta + 2; // Error común

                sprintf(opciones_txt[1][0], "%d", res_correcta);
                sprintf(opciones_txt[1][1], "%d", falsa1);
                sprintf(opciones_txt[1][2], "%d", falsa2);
                sprintf(opciones_txt[1][3], "%d", falsa3);
                correcta[1] = 1;
                tiempos_limite[1] = 30;
            }

            // 3. Ecuacion Lineal: A(Bx - C) = Dx + E
            {
                int x_target = 2 + rand() % 8; // La respuesta que queremos
                int A = 2 + rand() % 3;
                int B = 1 + rand() % 5;
                int C = (A * x_target) - (A * B); // Calculamos C para que cuadre

                // Si C es negativo, imprimimos diferente
                if (C >= 0) sprintf(preguntas_txt[2], "Solucion de %d(x - %d) = %d", A, B, C);
                else sprintf(preguntas_txt[2], "Solucion de %d(x - %d) = %d", A, B, C);

                res_correcta = x_target;
                falsa1 = x_target + 1;
                falsa2 = x_target * 2;
                falsa3 = (x_target > 2) ? x_target - 2 : x_target + 4;

                sprintf(opciones_txt[2][0], "x=%d", res_correcta);
                sprintf(opciones_txt[2][1], "x=%d", falsa1);
                sprintf(opciones_txt[2][2], "x=%d", falsa2);
                sprintf(opciones_txt[2][3], "x=%d", falsa3);
                correcta[2] = 1;
                tiempos_limite[2] = 30;
            }

            // 4. Derivada: f(x)=Ax^n - Bx + C
            // Derivada = (A*n)x^(n-1) - B
            {
                int A = 2 + rand() % 4; // 2 a 5
                int n = 2 + rand() % 2; // 2 o 3 (cuadrado o cubo)
                int B = 1 + rand() % 5;
                int C = rand() % 10;

                sprintf(preguntas_txt[3], "Derivada de f(x)=%dx^%d - %dx + %d", A, n, B, C);

                // Respuesta correcta
                char resp_buff[50];
                sprintf(resp_buff, "%dx^%d - %d", A*n, n-1, B);
                if (n-1 == 1) sprintf(resp_buff, "%dx - %d", A*n, B); // Quitar el ^1 si es lineal

                strcpy(opciones_txt[3][0], resp_buff);

                // Falsas
                sprintf(opciones_txt[3][1], "%dx^%d + %d", A*n, n-1, B); // Signo mal
                sprintf(opciones_txt[3][2], "%dx^%d - %d", A, n, B); // No derivó A
                sprintf(opciones_txt[3][3], "%dx - %dx", A*n, B); // Agregó x extra

                correcta[3] = 1;
                tiempos_limite[3] = 30;
            }

            // 5. Logaritmos: log_base(x) = exp => x = base^exp
            {
                int base = 2;
                int exp = 3 + rand() % 3; // 3, 4 o 5
                res_correcta = pow(base, exp);

                sprintf(preguntas_txt[4], "Si log%d(x) = %d, entonces x vale:", base, exp);

                falsa1 = base * exp;
                falsa2 = pow(base, exp-1);
                falsa3 = pow(base, exp+1);

                sprintf(opciones_txt[4][0], "%d", res_correcta);
                sprintf(opciones_txt[4][1], "%d", falsa1);
                sprintf(opciones_txt[4][2], "%d", falsa2);
                sprintf(opciones_txt[4][3], "%d", falsa3);
                correcta[4] = 1;
                tiempos_limite[4] = 30;
            }

            // 6. Sistema: x + y = A, x - y = B
            // 2x = A+B => x = (A+B)/2. Para que sea entero, A y B deben tener misma paridad.
            {
                int x = 2 + rand() % 5;
                int y = 1 + rand() % 3;
                int A = x + y;
                int B = x - y;

                sprintf(preguntas_txt[5], "Solucion sistema: x+y=%d y x-y=%d", A, B);

                sprintf(opciones_txt[5][0], "x=%d, y=%d", x, y); // Correcta
                sprintf(opciones_txt[5][1], "x=%d, y=%d", y, x); // Invertida
                sprintf(opciones_txt[5][2], "x=%d, y=%d", x+1, y-1);
                sprintf(opciones_txt[5][3], "x=%d, y=%d", A, B);

                correcta[5] = 1;
                tiempos_limite[5] = 120; // Tiempo largo
            }

            // 7. Magnitud Vector v=(A, B) => sqrt(A^2 + B^2)
            // Usaremos ternas pitagóricas para que de entero: (3,4)->5, (6,8)->10, (5,12)->13
            {
                int tipo = rand() % 3;
                int vx, vy, modulo;

                if (tipo == 0) { vx=3; vy=4; modulo=5; }
                else if (tipo == 1) { vx=6; vy=8; modulo=10; }
                else { vx=5; vy=12; modulo=13; }

                // A veces ponemos negativo para despistar (magnitud siempre es positiva)
                if (rand()%2 == 0) vy = -vy;

                sprintf(preguntas_txt[6], "Magnitud del vector v=(%d, %d)", vx, vy);

                sprintf(opciones_txt[6][0], "%d", modulo);
                sprintf(opciones_txt[6][1], "%d", modulo + 2);
                sprintf(opciones_txt[6][2], "%d", vx + abs(vy)); // Suma simple (error común)
                sprintf(opciones_txt[6][3], "%d", abs(vx*vy));

                correcta[6] = 1;
                tiempos_limite[6] = 120;
            }

            // 8. Integral simple: Integral de 0 a N de 2x dx = [x^2] = N^2
            {
                int N = 2 + rand() % 4; // 2 a 5
                res_correcta = N * N;

                sprintf(preguntas_txt[7], "Area bajo la recta y=2x entre 0 y %d", N);

                falsa1 = 2 * N;
                falsa2 = N * N * N;
                falsa3 = res_correcta + 1;

                sprintf(opciones_txt[7][0], "%d", res_correcta);
                sprintf(opciones_txt[7][1], "%d", falsa1);
                sprintf(opciones_txt[7][2], "%d", falsa2);
                sprintf(opciones_txt[7][3], "%d", falsa3);
                correcta[7] = 1;
                tiempos_limite[7] = 120;
            }

            // 9. Suma Vectores u=(A,B) v=(C,D)
            {
                int u1 = 1 + rand() % 5;
                int u2 = 1 + rand() % 5;
                int v1 = 1 + rand() % 5;
                int v2 = 1 + rand() % 5;

                sprintf(preguntas_txt[8], "Suma de u=(%d,%d) y v=(%d,%d)", u1, u2, v1, v2);

                sprintf(opciones_txt[8][0], "(%d, %d)", u1+v1, u2+v2); // Correcta
                sprintf(opciones_txt[8][1], "(%d, %d)", u1-v1, u2-v2); // Resta
                sprintf(opciones_txt[8][2], "(%d, %d)", u1+v2, u2+v1); // Cruzado
                sprintf(opciones_txt[8][3], "(%d, %d)", u1*v1, u2*v2); // Multiplicado

                correcta[8] = 1;
                tiempos_limite[8] = 20; // Tiempo corto
            }

            // 10. Determinante Matriz 2x2 |A B| |C D| = AD - BC
            {
                int A = 2 + rand() % 3;
                int D = 2 + rand() % 3;
                int B = 1;
                int C = 1 + rand() % 2;

                res_correcta = (A * D) - (B * C);

                sprintf(preguntas_txt[9], "Determinante matriz |%d %d| |%d %d|", A, B, C, D);

                sprintf(opciones_txt[9][0], "%d", res_correcta);
                sprintf(opciones_txt[9][1], "%d", res_correcta + 2);
                sprintf(opciones_txt[9][2], "%d", (A*D) + (B*C)); // Suma en vez de resta
                sprintf(opciones_txt[9][3], "%d", A+B+C+D); // Suma de elementos

                correcta[9] = 1;
                tiempos_limite[9] = 30;
            }

            // *** MEZCLAR TODAS LAS OPCIONES ***
            // ahora las mezclamos para que sea aleatorio A, B, C o D.
            int k;
            for(k=0; k<10; k++){
                mezclar_opciones(opciones_txt[k], &correcta[k]);
            }

            break;

        case 2: // FISICA
            strcpy(preguntas_txt[0], "Unidad de fuerza");
            strcpy(opciones_txt[0][0], "Joule"); strcpy(opciones_txt[0][1], "Newton"); strcpy(opciones_txt[0][2], "Pascal"); strcpy(opciones_txt[0][3], "Watt");
            correcta[0] = 2; tiempos_limite[0] = 10;

            strcpy(preguntas_txt[1], "Velocidad de la luz");
            strcpy(opciones_txt[1][0], "3x10^8 m/s"); strcpy(opciones_txt[1][1], "3x10^6 m/s"); strcpy(opciones_txt[1][2], "3x10^3 m/s"); strcpy(opciones_txt[1][3], "3x10^5 m/s");
            correcta[1] = 1; tiempos_limite[1] = 30;

            strcpy(preguntas_txt[2], "Ley F = m * a");
            strcpy(opciones_txt[2][0], "Gravitacion"); strcpy(opciones_txt[2][1], "2da Ley Newton"); strcpy(opciones_txt[2][2], "Ohm"); strcpy(opciones_txt[2][3], "Pascal");
            correcta[2] = 2; tiempos_limite[2] = 30;

            strcpy(preguntas_txt[3], "Unidad de energia");
            strcpy(opciones_txt[3][0], "Newton"); strcpy(opciones_txt[3][1], "Joule"); strcpy(opciones_txt[3][2], "Lux"); strcpy(opciones_txt[3][3], "Tesla");
            correcta[3] = 2; tiempos_limite[3] = 30;

            strcpy(preguntas_txt[4], "Unidad de potencia");
            strcpy(opciones_txt[4][0], "Watt"); strcpy(opciones_txt[4][1], "Newton"); strcpy(opciones_txt[4][2], "Joule"); strcpy(opciones_txt[4][3], "Volt");
            correcta[4] = 1; tiempos_limite[4] = 30;

            strcpy(preguntas_txt[5], "Aceleracion gravedad");
            strcpy(opciones_txt[5][0], "8 m/s2"); strcpy(opciones_txt[5][1], "9.8 m/s2"); strcpy(opciones_txt[5][2], "10 m/s2"); strcpy(opciones_txt[5][3], "12 m/s2");
            correcta[5] = 2; tiempos_limite[5] = 120; // Tiempos largos según tu esquema

            strcpy(preguntas_txt[6], "Unidad de Presion");
            strcpy(opciones_txt[6][0], "Joule"); strcpy(opciones_txt[6][1], "Pascal"); strcpy(opciones_txt[6][2], "Newton"); strcpy(opciones_txt[6][3], "Bar");
            correcta[6] = 2; tiempos_limite[6] = 20;

            strcpy(preguntas_txt[7], "Unidad carga electrica");
            strcpy(opciones_txt[7][0], "Volt"); strcpy(opciones_txt[7][1], "Ampere"); strcpy(opciones_txt[7][2], "Coulomb"); strcpy(opciones_txt[7][3], "Ohm");
            correcta[7] = 3; tiempos_limite[7] = 30;

            strcpy(preguntas_txt[8], "Unidad de Voltaje");
            strcpy(opciones_txt[8][0], "W"); strcpy(opciones_txt[8][1], "A"); strcpy(opciones_txt[8][2], "V"); strcpy(opciones_txt[8][3], "C");
            correcta[8] = 3; tiempos_limite[8] = 20;

            strcpy(preguntas_txt[9], "Formula de Trabajo =");
            strcpy(opciones_txt[9][0], "F * d"); strcpy(opciones_txt[9][1], "m * g"); strcpy(opciones_txt[9][2], "P * t"); strcpy(opciones_txt[9][3], "m * v");
            correcta[9] = 1; tiempos_limite[9] = 30;
            break;

        case 3: // QUIMICA
            strcpy(preguntas_txt[0], "Simbolo del sodio");
            strcpy(opciones_txt[0][0], "So"); strcpy(opciones_txt[0][1], "Sd"); strcpy(opciones_txt[0][2], "Na"); strcpy(opciones_txt[0][3], "S");
            correcta[0] = 3; tiempos_limite[0] = 30;

            strcpy(preguntas_txt[1], "Z del Oxigeno");
            strcpy(opciones_txt[1][0], "6"); strcpy(opciones_txt[1][1], "7"); strcpy(opciones_txt[1][2], "8"); strcpy(opciones_txt[1][3], "10");
            correcta[1] = 3; tiempos_limite[1] = 30;

            strcpy(preguntas_txt[2], "Comparte electrones");
            strcpy(opciones_txt[2][0], "Ionico"); strcpy(opciones_txt[2][1], "Covalente"); strcpy(opciones_txt[2][2], "Metalico"); strcpy(opciones_txt[2][3], "Nuclear");
            correcta[2] = 2; tiempos_limite[2] = 30;

            strcpy(preguntas_txt[3], "PH Acido");
            strcpy(opciones_txt[3][0], "2"); strcpy(opciones_txt[3][1], "7"); strcpy(opciones_txt[3][2], "10"); strcpy(opciones_txt[3][3], "12");
            correcta[3] = 1; tiempos_limite[3] = 30;

            strcpy(preguntas_txt[4], "Simbolo Potasio");
            strcpy(opciones_txt[4][0], "P"); strcpy(opciones_txt[4][1], "Po"); strcpy(opciones_txt[4][2], "Pt"); strcpy(opciones_txt[4][3], "K");
            correcta[4] = 4; tiempos_limite[4] = 30;

            strcpy(preguntas_txt[5], "Agua a 100C");
            strcpy(opciones_txt[5][0], "Liquido"); strcpy(opciones_txt[5][1], "Gas"); strcpy(opciones_txt[5][2], "Solido"); strcpy(opciones_txt[5][3], "Plasma");
            correcta[5] = 2; tiempos_limite[5] = 20;

            strcpy(preguntas_txt[6], "Creador Tabla Periodica");
            strcpy(opciones_txt[6][0], "Newton"); strcpy(opciones_txt[6][1], "Einstein"); strcpy(opciones_txt[6][2], "Mendeleiev"); strcpy(opciones_txt[6][3], "Curie");
            correcta[6] = 3; tiempos_limite[6] = 20;

            strcpy(preguntas_txt[7], "Z del Hidrogeno");
            strcpy(opciones_txt[7][0], "1"); strcpy(opciones_txt[7][1], "2"); strcpy(opciones_txt[7][2], "3"); strcpy(opciones_txt[7][3], "0");
            correcta[7] = 1; tiempos_limite[7] = 20;

            strcpy(preguntas_txt[8], "Simbolo del hierro");
            strcpy(opciones_txt[8][0], "I"); strcpy(opciones_txt[8][1], "H"); strcpy(opciones_txt[8][2], "Ir"); strcpy(opciones_txt[8][3], "Fe");
            correcta[8] = 4; tiempos_limite[8] = 20;

            strcpy(preguntas_txt[9], "Mezcla homogenea");
            strcpy(opciones_txt[9][0], "Arena+agua"); strcpy(opciones_txt[9][1], "Aceite+agua"); strcpy(opciones_txt[9][2], "Sal+agua"); strcpy(opciones_txt[9][3], "Tierra");
            correcta[9] = 3; tiempos_limite[9] = 30;
            break;
        }

        // ---- BUCLE DE PREGUNTAS ----
        if (opcion >= 1 && opcion <= 3) {
            int i;
            int puntaje_ronda = 0;

            for (i = 0; i < 10; i++) {

                limpiar_pantalla();
                printf("Pregunta %d/10:\n", i + 1);
                printf("%s\n", preguntas_txt[i]); // Imprime la pregunta dinámica
                printf("1) %s\n2) %s\n3) %s\n4) %s\n",
                        opciones_txt[i][0], opciones_txt[i][1], opciones_txt[i][2], opciones_txt[i][3]);

                printf("Respuesta: ");

                // Usamos el tiempo específico que definimos en cada case
                respuesta = leer_con_tiempo(tiempos_limite[i]);

                if (respuesta == -1) {
                    printf("\nTiempo agotado!\n");
                } else {
                    respuestas_usuario[i] = respuesta;

                    if (respuesta == correcta[i]) {
                        printf("\nRespuesta correcta!\n");
                        puntaje_ronda++;
                    } else {
                        printf("\nRespuesta incorrecta.\n");
                        printf("La correcta era la opcion %d\n", correcta[i]); // Feedback útil
                    }
                }
                Sleep(1000);
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
