.section .data
    array:
        .short 65292, 65312, 65318, 65390, 65416, 65336, 65433, 65529, 65423, 65343
    str:
        .ascii "                                       \n" #10 пробелов
    invertor:
        .short 0xff00

.section .text
.global _start
    _start:
        mov $10, %cx #итератор в сх = 10 для инвертирования
        mov $array, %esi
        mov invertor, %esp

    invert:
        xor %esp, (%esi) #ff в старших битах при xor инертируют старшие биты элемента массива
        add $2, %esi #смещаемся на один элемент (2 байта)
    loop invert

    mov $5, %cx #итератор = 5 для суммы четных элементов
    mov $array, %esi #восстанавливаем адрес начала массива в esi
    add $2, %esi #смещаемся на второй элемент массива
    mov $0, %edi #кладем 0 в edi, сюда будем писать сумму
    sum:
        add (%esi), %edi #берем адрес в esi и содержимое по этому адресу прибавляем к сумме
        add $4, %esi #смещаемся на 2 элемента (4 байта)
    loop sum

    mov %edi, %eax #переносим сумму в eax
    mov $str, %ebp #адрес строки в ebp
    add $25, %ebp #смещаемся в конец строки

    #mov $40, %cx

    convert:
        mov $10, %ebx #основание системы счисления в ebx
        xor %edx, %edx #чистим edx, сюда будет писаться остаток от деления
        div %bx #делим сумму в eax на 10, остаток в dx, частное в ax
        add $48, %dl #добавляем к остатку 48, чтобы полученное значение стало аски кодом полученного числа
        mov %dl, (%ebp) #пишем цифру по адресу нужного символа
        sub $1, %ebp #смещаемся на один символ влево по строке
        cmpw $0, %ax
        je print_and_exit


    loop convert

    print_and_exit:
        mov $4, %eax
        mov $1, %ebx
        mov $str, %ecx
        mov $41, %edx

        int $0x80

        mov $1, %eax
        mov $0, %ebx
        int $0x80
