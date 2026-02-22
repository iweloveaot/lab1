#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "3d_vector.h"


// Прототип функции тестирования
void run_all_tests(void);

void print_menu() {
    printf("\n========================================\n");
    printf("ПРОГРАММА ДЛЯ РАБОТЫ С 3D ВЕКТОРАМИ\n");
    printf("========================================\n");
    printf("1. Работа с вещественными числами\n");
    printf("2. Работа с комплексными числами\n");
    printf("3. Запустить все тесты\n");
    printf("0. Выход\n");
    printf("Выберите режим: ");
}

void print_vector_menu() {
    printf("\n--- Операции с векторами ---\n");
    printf("1. Создать вектор\n");
    printf("2. Сложить два вектора\n");
    printf("3. Вычислить скалярное произведение\n");
    printf("4. Вычислить векторное произведение\n");
    printf("5. Показать текущие векторы\n");
    printf("6. Очистить все векторы\n");
    printf("0. Вернуться в главное меню\n");
    printf("Выберите операцию: ");
}

void clear_vectors(Vector3D** vectors, int* count) {
    for (int i = 0; i < *count; i++) {
        destroy_vector(vectors[i]);
    }
    *count = 0;
    printf("Все векторы удалены.\n");
}

void show_vectors(Vector3D** vectors, int count) {
    if (count == 0) {
        printf("Нет сохраненных векторов.\n");
        return;
    }

    printf("\nСохраненные векторы:\n");
    for (int i = 0; i < count; i++) {
        printf("v%d = ", i + 1);
        print_vector(vectors[i]);
        printf("\n");
    }
}

void run_interactive_mode(const FieldInfo* info) {
    Vector3D* vectors[10];
    int vector_count = 0;
    int choice;

    do {
        print_vector_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1: { // Создать вектор
                if (vector_count >= 10) {
                    printf("Достигнут лимит векторов (10).\n");
                    break;
                }

                Vector3D* v = input_vector(info);
                if (v) {
                    vectors[vector_count++] = v;
                    printf("Вектор создан: ");
                    print_vector(v);
                    printf("\n");
                } else {
                    printf("Ошибка при создании вектора.\n");
                }
                break;
            }

            case 2: { // Сложение
                if (vector_count < 2) {
                    printf("Недостаточно векторов (нужно минимум 2).\n");
                    break;
                }

                int i1, i2;
                show_vectors(vectors, vector_count);
                printf("Выберите индексы векторов для сложения (1-%d): ", vector_count);
                scanf("%d %d", &i1, &i2);

                if (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count) {
                    printf("Неверные индексы.\n");
                    break;
                }

                Vector3D* sum = vector_add(vectors[i1-1], vectors[i2-1]);
                if (sum) {
                    printf("Результат сложения: ");
                    print_vector(sum);
                    printf("\n");

                    if (vector_count < 10) {
                        vectors[vector_count++] = sum;
                        printf("Результат сохранен как v%d\n", vector_count);
                    } else {
                        printf("Лимит векторов достигнут, результат не сохранен.\n");
                        destroy_vector(sum);
                    }
                }
                break;
            }

            case 3: { // Скалярное произведение
                if (vector_count < 2) {
                    printf("Недостаточно векторов (нужно минимум 2).\n");
                    break;
                }

                int i1, i2;
                show_vectors(vectors, vector_count);
                printf("Выберите индексы векторов (1-%d): ", vector_count);
                scanf("%d %d", &i1, &i2);

                if (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count) {
                    printf("Неверные индексы.\n");
                    break;
                }

                void* sp = scalar_product(vectors[i1-1], vectors[i2-1]);
                if (sp) {
                    printf("Скалярное произведение: ");
                    info->print(sp);
                    printf("\n");
                    free(sp);
                }
                break;
            }

            case 4: { // Векторное произведение
                if (vector_count < 2) {
                    printf("Недостаточно векторов (нужно минимум 2).\n");
                    break;
                }

                int i1, i2;
                show_vectors(vectors, vector_count);
                printf("Выберите индексы векторов (1-%d): ", vector_count);
                scanf("%d %d", &i1, &i2);

                if (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count) {
                    printf("Неверные индексы.\n");
                    break;
                }

                Vector3D* cross = cross_product(vectors[i1-1], vectors[i2-1]);
                if (cross) {
                    printf("Векторное произведение: ");
                    print_vector(cross);
                    printf("\n");

                    if (vector_count < 10) {
                        vectors[vector_count++] = cross;
                        printf("Результат сохранен как v%d\n", vector_count);
                    } else {
                        printf("Лимит векторов достигнут, результат не сохранен.\n");
                        destroy_vector(cross);
                    }
                }
                break;
            }

            case 5: // Показать векторы
                show_vectors(vectors, vector_count);
                break;

            case 6: // Очистить векторы
                clear_vectors(vectors, &vector_count);
                break;

            case 0:
                printf("Возврат в главное меню...\n");
                break;

            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 0);

    clear_vectors(vectors, &vector_count);
}

int main()
{
    int command = 4;

    while (command != 0)
    {
        print_menu();
        printf("Enter the number of command: ");
        scanf("%d", &command);

        switch (command)
        {
            case 1:
                printf("\n=== Working with double numbers ===\n");
                run_interactive_mode(ofDouble());
                break;

            case 2:
                printf("\n=== Working with complex numbers ===\n");
                run_interactive_mode(ofComplex());
                break;

            case 3:
                run_all_tests();
                break;

            case 0:
                printf("Exit.\n");
                break;

            default:
                printf("This command number is not available. Enter command 0, 1, 2 or 3\n");
        }
    }

    return 0;
}
