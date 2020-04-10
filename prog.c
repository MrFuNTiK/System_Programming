#include "stdio.h"
#include "stdint.h"
//#include "stdlib.h"
//#include "time.h"

int main()
{
    //uint8_t size;
    uint8_t sum = 0;
    uint8_t *sum_ptr = &sum;
    //uint8_t* first_array;
    //uint8_t* second_array;
    //printf("Enter ammount of elevents in array:\n");
    //scanf("%c", &size);
    //first_array = (uint8_t*)malloc(sizeof(uint8_t)*10);
    //second_array = (uint8_t*)malloc(sizeof(uint8_t)*10);

    /*
    srand(time(NULL));
    for (uint8_t i = 0; i < 10; i++)
    {
        first_array[i] = rand()%10;
        second_array[i] = rand()%10;
    }
    */

    uint8_t first_array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t second_array[10] = {2, 3, 5, 4, 8, 1, 9, 7, 6, 0};

    //Проверить работу с передачей указателей на массивы, возможно это можно удалить
    uint8_t* first_ptr = first_array;
    uint8_t* second_ptr = second_array;


    //************************************

    asm volatile(
        "xor %%rcx, %%rcx\n\t"
        "mov $10, %%cx\n\t"
        "_count_sum:\n\t"
        "mov (%%rbx), %%rax\n\t"
        "mulb (%%rdx)\n\t"
        "add %%rax, (%%rdi)\n\t"
        "add $1, %%rbx\n\t"
        "add $1, %%rdx\n\t"
        "loop _count_sum\n\t"
        :
        :"b"(first_ptr), "d"(second_ptr), "D"(sum_ptr)
        :
    );
    printf("%d", sum);
    printf("\n");
    return 0;
}

        //"mov $0, %%ebx\n\t"
        //"mov $1, %%edx\n\t"
        //"mov $2, %%edi\n\t"