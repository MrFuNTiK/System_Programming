#include "stdint.h"
#include "stdio.h"
#include "inttypes.h"

uint16_t invertHighBits(uint16_t tmp)
{
    uint16_t low = 0x00ff & tmp;
    uint16_t high = 0xff00 & tmp;
    high = ~high;
    high = high / 256;
    high = high * 256;
    high += low;
    return high;
}

int main()
{
    uint16_t array[10] = {65292, 65312, 65318, 65390, 65416, 65336, 65433, 65529, 65423, 65343};
    uint32_t sum = 0;
    for (uint8_t i = 0; i < 10; i++)
    {
        array[i] = invertHighBits(array[i]);
        if ((i % 2) == 1)
        {
            sum += array[i];
        }
    }
    printf("%u", sum);
    printf("\n");
    return 0;
}