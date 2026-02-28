#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "3d_vector.h"
#include "tests.h"


void print_menu() 
{
    printf("\n========================================\n");
    printf("        WORKING WITH 3D-VECTORS        \n");
    printf("========================================\n");
    printf("1. Start working \n");
    printf("2. Run tests \n");
    printf("0. Exit\n");
}

void print_vector_menu() 
{
    printf("1. Create vector\n");
    printf("2. Change vector\n");
    printf("3. Add two vectors\n");
    printf("4. Calculate scalar product\n");
    printf("5. Calculate cross product\n");
    printf("6. Show all vectors\n");
    printf("7. Clear all vectors\n");
    printf("0. Back to main menu\n");
    printf("Enter the number of command: ");
}

void clear_vectors(Vector3D** vectors, int count) 
{
    for (int i = 0; i < count; i++) 
    {
        deleteVector(vectors[i]);
    }
    printf("All vectors are cleared.\n");
    printf("\n");
}

void show_vectors(Vector3D** vectors, int count) 
{
    if (count == 0) 
    {
        printf("No saved vectors.\n");
        printf("\n");
        return;
    }
    
    printf("\nYour vectors:\n");
    for (int i = 0; i < count; i++) 
    {
        printf("v%d = ", i + 1);
        printVector(vectors[i]);
        printf("\n");
    }
}

void run_interactive_mode() 
{
    Vector3D* vectors[15];
    int vector_count = 0;
    int command = 15;

    printf("You can create no more than 15 vectors during the session!\n");
    while (command != 0) 
    {
        print_vector_menu();
        int code = scanf("%d", &command);
        while (code != 1) {
            printf("Incorrect input, please repeat\n");
            while (getchar() != '\n');
            code = scanf("%d", &command);
    }
        
        switch (command) 
        {
            case 1: 
            {
                if (vector_count >= 15) 
                {
                    printf("Vectors limit reached (15).\n");
                    printf("\n");
                    break;
                }
                
                int type_n = 0;
                while (!(type_n == 1 || type_n == 2))
                {
                    printf("Enter type of your vector (1-double/2-complex): ");
                    int code = scanf("%d", &type_n);
                    while (code != 1) {
                        printf("Incorrect input, please repeat\n");
                        while (getchar() != '\n');
                        code = scanf("%d", &type_n);
                    }
                    switch (type_n)
                    {
                        case 1:
                        {
                            Vector3D *v = inputVector(ofDouble());
                            if (v) 
                            {
                                vectors[vector_count++] = v;
                                printf("Your new vector: ");
                                printVector(v);
                                printf("\n");
                            } else {
                                printf("Error while creating vector.\n");
                            }
                            break;
                        }
                        case 2:
                        {
                            Vector3D *v = inputVector(ofComplex());
                            if (v) 
                            {
                                vectors[vector_count++] = v;
                                printf("Your new vector: ");
                                printVector(v);
                                printf("\n");
                            } else {
                                printf("Error while creating vector.\n");
                            }
                            break;
                        }
                        default:
                        {
                            printf("Wrong number, try 1 or 2\n");
                            break;
                        }
                    }
                }
                break;
            }
            
            case 2:
            {
                int ind=0;
                show_vectors(vectors, vector_count);
                while (ind < 1 || ind > vector_count)
                {
                    printf("Enter number of vector to change (1-%d): ", vector_count);
                    int code = scanf("%d", &ind);
                    while (code != 1) {
                        printf("Incorrect input, please repeat\n");
                        while (getchar() != '\n');
                        code = scanf("%d", &ind);
                    }
                    if (ind < 1 || ind > vector_count) 
                        printf("There's no vector with this number, try again!\n");
                }

                Vector3D *new_v = inputVector(vectors[ind-1]->type_info);
                if (new_v)
                {
                    free(vectors[ind-1]);
                    vectors[ind-1] = new_v;
                    printf("Successfully changed.\n");
                    printf("\n");
                }
                break;
            }

            case 3: 
            {
                if (vector_count < 2) 
                {
                    printf("Not enough vectors, need at least 2.\n");
                    printf("\n");
                    break;
                }

                if (vector_count == 15) 
                {
                    int for_lim;
                    printf("Vectors limit reached (15). Your result won't be saved. Continue adding? (1-yes/0-stop): ");
                    int code = scanf("%d", &for_lim);
                    while (code != 1) {
                        printf("Incorrect input, please repeat\n");
                        while (getchar() != '\n');
                        code = scanf("%d", &for_lim);
                    }
                    if (!for_lim) break;

                }
                
                int i1=0, i2=0;
                show_vectors(vectors, vector_count);
                while (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count)
                {
                    printf("Enter numbers of vectors to add (1-%d): ", vector_count);
                    int code = scanf("%d %d", &i1, &i2);
                    while (code != 2) {
                        printf("Incorrect input, please repeat\n");
                        while (getchar() != '\n');
                        code = scanf("%d %d", &i1, &i2);
    }

                    if (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count) 
                        printf("There're no vectors with these numbers, try again!\n");
                }
                
                Vector3D* sum = vectorAdd(vectors[i1-1], vectors[i2-1]);
                if (sum) 
                {
                    printf("Result vector: ");
                    printVector(sum);
                    printf("\n");
                    
                    if (vector_count < 10) 
                    {
                        vectors[vector_count++] = sum;
                        printf("Result saved with number v%d\n", vector_count);
                    } else {
                        printf("Vectors limit reached (15). Your result weren't saved.\n");
                        deleteVector(sum);
                    }
                }
                break;
            }
            
            case 4: 
            {
                if (vector_count < 2) 
                {
                    printf("Not enough vectors, need at least 2.\n");
                    printf("\n");
                    break;
                }
                
                int i1=0, i2=0;
                show_vectors(vectors, vector_count);
                while (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count)
                {
                    printf("Enter numbers of vectors to multiply (1-%d): ", vector_count);
                    int code = scanf("%d %d", &i1, &i2);
                    while (code != 2) {
                        printf("Incorrect input, please repeat\n");
                        while (getchar() != '\n');
                        code = scanf("%d %d", &i1, &i2);
                    }
                    if (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count) 
                        printf("There're no vectors with these numbers, try again!\n");
                }
                
                printf("Scalar product: ");
                if (vectors[i1-1]->type_info == ofDouble())
                {
                    Double sp;
                    scalarProduct(vectors[i1-1], vectors[i2-1], &sp);
                    vectors[i1-1]->type_info->print(&sp);
                } else {
                    Complex sp;
                    scalarProduct(vectors[i1-1], vectors[i2-1], &sp);
                    vectors[i1-1]->type_info->print(&sp);
                }
                printf("\n");
                break;
            }
            
            case 5: 
            {
                if (vector_count < 2) 
                {
                    printf("Not enough vectors, need at least 2.\n");
                    printf("\n");
                    break;
                }

                if (vector_count == 15) 
                {
                    int for_lim;
                    printf("Vectors limit reached (15). Your result won't be saved. Continue adding? (1-yes/0-stop): ");
                    int code = scanf("%d", &for_lim);
                    while (code != 1) {
                        printf("Incorrect input, please repeat\n");
                        while (getchar() != '\n');
                        code = scanf("%d", &for_lim);
                    }
                    if (!for_lim) break;

                }
                
                int i1=0, i2=0;
                show_vectors(vectors, vector_count);
                while (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count)
                {
                    printf("Enter numbers of vectors to multiply (1-%d): ", vector_count);
                    int code = scanf("%d %d", &i1, &i2);
                    while (code != 2) {
                        printf("Incorrect input, please repeat\n");
                        while (getchar() != '\n');
                        code = scanf("%d %d", &i1, &i2);
                    }
                    if (i1 < 1 || i1 > vector_count || i2 < 1 || i2 > vector_count) 
                        printf("There're no vectors with these numbers, try again!\n");
                }
                
                Vector3D* cross = crossProduct(vectors[i1-1], vectors[i2-1]);
                if (cross) 
                {
                    printf("Result vector: ");
                    printVector(cross);
                    printf("\n");
                    
                    if (vector_count < 10) 
                    {
                        vectors[vector_count++] = cross;
                        printf("Result saved with number v%d\n", vector_count);
                    } else {
                        printf("Vectors limit reached (15). Your result not saved.\n");
                        deleteVector(cross);
                    }
                }
                break;
            }
            
            case 6: // Показать векторы
                show_vectors(vectors, vector_count);
                break;
                
            case 7: // Очистить векторы
                clear_vectors(vectors, vector_count);
                vector_count = 0;
                break;
                
            case 0:
                printf("Back to main menu...\n");
                break;
                
            default:
                printf("Unavailable number of command! Available numbers: 1 2 3 4 5 6 7 0.\n");
                printf("\n");
        }
    }
    
    clear_vectors(vectors, vector_count);
    print_menu();
}

int main() 
{
    int command = 3;
    
    print_menu();
    while (command != 0) 
    {
        printf("Enter the number of command: ");
        int code = scanf("%d", &command);
        while (code != 1) {
            printf("Incorrect input, please repeat\n");
            while (getchar() != '\n');
            code = scanf("%d", &command);
        }
        printf("\n");
        switch (command) 
        {
            case 1:
                printf("\n=== Working with 3D-vectors ===\n");
                run_interactive_mode();
                break;
                
            case 2:
                run_all_tests();
                break;
                
            case 0:
                printf("Bye-bye.\n");
                break;
                
            default:
                printf("Unavailable number of command! Available numbers: 1 2 0\n");
        }
    }
    
    return 0;
}