section .data
	enterMessage db "Type a number to take factorial (-1 to exit): ", 0
	inputFormat db "%d", 0
	resultMessage db "%d! = %d", 10, 0
	badInputMsg db "The given number must be between 0 and 12!", 10, 0

section .bss
	input resd 1

section .text
	global main
	extern printf
	extern scanf

main:
	; ask for input
	push enterMessage ; push the 'enter input' message to the stack
	call printf
	add esp, 4	; balance stack pointer

	; read input
	push input	; 'input' will hold the input value
	push inputFormat ; 'inputFormat' gives the input format (an integer in a string)
	call scanf
	add esp, 8	; balance stack pointer

	mov ebx, [input] ; write input value into ebx

	; check input
	cmp ebx, -1 ; compare input with -1
	je exit		; if input == -1, then jump to 'exit' label
	jl badInput ; if input < -1, then jump to 'badInput' label
	cmp ebx, 12 ; compare input with 12
	jg badInput ; if input > 12, then jump to 'badInput' label

	mov eax, 1	; eax will hold the factorial result, smallest possible result is 1

facLoop:		; loop for calculating the factorial
	cmp ebx, 1	; compare ebx with 1, as the value of ebx decreases by 1 with every loop and must get to 1 eventually
	jle writeResult ; if ebx <= 1, then jump to label writeResult, because the calculation is complete
	mul ebx		; eax = eax * ebx
	sub ebx, 1	; decrement ebx by 1
	jmp facLoop	; jump back to the beginning of the loop

writeResult:
	push eax	; push the result integer of the result message to the stack
	mov ebx, [input] ; write the initial input to ebx
	push ebx	; push the input integer of the result message to the stack
	push resultMessage ; push the result message to the stack
	call printf
	add esp, 12	; balance stack pointer
	jmp main	; jump back to label 'main' to continue the program as a loop

exit:			; exits the program
	ret

badInput:		; factorials can't be taken from number 13 and higher because of the size limit of a integer
	push badInputMsg ; push the bad input message to the stack
	call printf
	add esp, 4	; balance stack pointer
	jmp main	; jump back to label 'main' to continue the program as a loop
