.section .data
str:
	.ascii "Hello, world!\n"

.section .text
.global _start

_start:
	mov $4, %eax
	mov $1, %ebx
	mov $str, %ecx
	mov $14, %edx
	int $0x80

	mov $1, %eax
	mov $0, %ebx
	
	int $0x80
