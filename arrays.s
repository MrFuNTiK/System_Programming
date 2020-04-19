.section .data
first_array:
    .byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
second_array:
    .byte 2, 3, 5, 4, 8, 1, 9, 7, 6, 0

.section .text
.global _start
_start:
    mov $first_array, %ebx
    mov $second_array, %edx
    mov $sum, %edi
    mov $10, %cx
    count_sum:
        mov (%ebx), %al
        mulb (%edx)
        add %eax, (%edi)
        add $1, %ebx
        add $1, %edx
    loop count_sum


    mov $1, %eax
    mov $0, %ebx
    int $0x80
