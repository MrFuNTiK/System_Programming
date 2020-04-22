#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "time.h"

void printArrays(uint8_t* first_ptr, uint8_t* second_ptr, uint8_t size)
{
    uint8_t i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", first_ptr[i]);
    }
    printf("\n");
    for (i = 0; i < size; i++)
    {
        printf("%d ", second_ptr[i]);
    }
    printf("\n");
}

int main()
{
    int size;
    uint16_t sum = 0;
    uint16_t *sum_ptr = &sum;
    uint8_t* first_array;
    uint8_t* second_array;
    printf("Enter ammount of elevents in array:\n");
    scanf("%d", &size);
    //size -= 48;
    first_array = (uint8_t*)malloc(sizeof(uint8_t)*size);
    second_array = (uint8_t*)malloc(sizeof(uint8_t)*size);

    
    srand(time(NULL));
    for (uint8_t i = 0; i < size; i++)
    {
        first_array[i] = rand()%10;
        second_array[i] = rand()%10;
    }
    
    printArrays(first_array, second_array, size);


    asm volatile(
        "_count_sum:\n\t"
        "mov (%%rbx), %%rax\n\t"
        "mulb (%%rdx)\n\t"
        "add %%rax, (%%rdi)\n\t"
        "add $1, %%rbx\n\t"
        "add $1, %%rdx\n\t"
        "loop _count_sum\n\t"
        :
        :"b"(first_array), "d"(second_array), "D"(sum_ptr), "c"(size)
        :
    );
    printf("%u", sum);
    printf("\n");
	free(first_array);
	free(ssecond_array);
    return 0;
}
