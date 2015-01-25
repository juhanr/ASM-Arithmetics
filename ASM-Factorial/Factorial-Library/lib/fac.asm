%include "macros.inc"

globalfunc factorial, arg:dword
start:
	prologue
	mov ebx, .arg

checkInput:
	cmp ebx, 0
	jl badInput
	cmp ebx, 12
	jg badInput

facLoop:
	mov eax, 1 ; eax will hold the factorial result, smallest possible result is 1
	.while ebx > 1
	mul ebx ; eax = eax * ebx
	dec ebx ; decrement ebx by 1
	.endwhile

exit:
	epilogue

badInput:
	mov eax, -1 ; -1 is returned when the input is not calculatable
	jmp exit
