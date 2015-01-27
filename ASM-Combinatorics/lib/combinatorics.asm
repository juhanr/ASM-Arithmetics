%include "macros.inc"

%macro start 0
	push ebp
	mov ebp, esp
%endmacro

%macro end 0
	mov esp, ebp
	pop ebp
	ret
%endmacro

; Checks if the given factorial is between 0 and 12. 
; Factorial can't be taken from higher numbers than 12, because of the size limitation of a 32-bit integer.
; The program returns -1 if the given factorial can't be calculated.
%macro check_factorial 1
	mov eax, %1
	.if eax > 12
		mov eax, -1
		end
	.elseif eax < 0
		mov eax, -1
		end
	.endif
%endmacro

; Factorial: n! = n*(n-1)*(n-2)*...
%macro factorial 1
	mov ebx, %1
	check_factorial ebx
	mov eax, 1 ; eax will hold the factorial result, smallest possible result is 1
	.while ebx > 1
		mul ebx ; eax = eax * ebx
		dec ebx ; ebx--
	.endwhile
%endmacro

; Permutations without repetition: P(n,r) = n! / (n-r)!
%macro permutation 2
	; Calculate n! and store it in variable nf
	factorial %1
	mov [nf], eax
	; Calculate (n-r)! and store it in register ebx
	mov eax, %1
	sub eax, %2
	factorial eax
	mov ebx, eax
	; Divide n! by (n-r)!
	mov eax, [nf]
	div ebx
%endmacro

section .bss
	nf resd 1 ; n!
	rf resd 1 ; r!

section .text

; Factorial: n! = n*(n-1)*(n-2)*...
globalfunc fac, n:dword
	start
	factorial .n
	end

; Permutations without repetition: P(n,r) = n! / (n-r)!
globalfunc perm, n:dword, r:dword
	start
	permutation .n, .r
	end

; Permutations with repetition: P(n,r) = n^r
globalfunc permrep, n:dword, r:dword
	start
	mov eax, .n
	mov ebx, .r
	.while ebx > 1
		mul .n
		dec ebx
	.endwhile
	end

; Combinations without repetition: C(n,r) = n! / (r!(n-r)!) = (n! / (n-r)!) / r!
globalfunc comb, n:dword, r:dword
	start
	; Calculate r! and store it in variable rf
	factorial .r	
	mov [rf], eax
	; Calculate n! / (n-r)!
	permutation .n, .r
	; Divide n! / (n-r)! by r!
	mov ebx, [rf]
	div ebx	
	end

; Combinations with repetition: C(n,r) = (n+r-1)! / (r!(n-1)!)
globalfunc combrep, n:dword, r:dword
	start 
	; Calculate r! and store it in variable rf
	factorial .r
	mov [rf], eax
	; Calculate (n-1)!
	mov eax, .n
	dec eax
	factorial eax
	; Multiply (n-1)! by r! and store the result in register ecx
	mov ebx, [rf]
	mul ebx
	mov ecx, eax
	; Calculate (n+r-1)!
	mov eax, .n
	add eax, .r
	dec eax
	factorial eax
	; Divide (n+r-1)! by r!(n-1)!
	div ecx
	end
