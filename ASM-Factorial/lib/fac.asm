SYS_EXIT  equ 1

section .data

	enterMessage db "Type a number to take factorial (-1 to exit): ", 0
	readInput db "%d", 0
	resultMessage db "%d! = %d", 10, 0
	badInputMsg db "The given number must be between 0 and 12!", 0xA	; factorials can't be taken from number 13 and higher because of the size limit of a integer

section .bss
	input resd 1

section .text

	global main
	extern printf
	extern scanf

main:
	; ask for input
	push enterMessage
	call printf

	; read input
	add esp,4
	push input
	push readInput
	call scanf

	add esp, 8
	mov ebx,[input]

	; check input
	cmp ebx, -1
	je exit
	jl badInput
	cmp ebx, 12
	jg badInput

	mov eax, 1

facLoop:
	cmp ebx, 1
	jle writeResult
	mul ebx
	sub ebx,1
	jmp facLoop

writeResult:
	; display result
	add esp,8
	push eax
	mov ebx, [input]
	push ebx
	push resultMessage
	call printf
	add esp,12
	jmp main

exit:    
	mov eax, SYS_EXIT   
	xor ebx, ebx 
	int 0x80

badInput:
	add esp,8
	push badInputMsg
	call printf
	add esp,4
	jmp main
