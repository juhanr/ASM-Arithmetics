%ifndef MACROS_INCLUDED ; include guard
%define MACROS_INCLUDED
;; --------------------
;; macros.inc
;; @brief   Basic helper macros for NASM
;; @author  Jorma Rebane
;; @date    10.12.2014
;; @changelog
;;     10.12.2014 - Added local arrays "LOCAL array:dword[20]"
;; --------------------



; // Defines an ASCII zero-terminated string
; //	arg1: variable name
; //	arg2: string literal
; //	ex:
; //		asciiz string0, "Hello world!",13
; //
%imacro asciiz 2+.nolist
	[section .data]
	%1: db %2,0
	%strlen %1.length %2	;; string0.length
	__SECT__				;; change back to previous section
%endmacro


; // Defines a 32-bit data variable
; //	arg1: variable name
; //	arg2: data
; //    ex:
; //    	data32 var1, 0
; //
%imacro data32 2+.nolist
	[section .data]
	%1: dd %2
	__SECT__				;; change back to previous section
%endmacro


; // Find index of a character inside the parameter literal
; // Returns 0 if character not found
; // Else returns 1-based index of character
; // Return value in  %$mstrchr.ret
; // ex:
; //	mstrchr _function@12, @
; //		-> 9
; //
%macro mstrchr 2.nolist
	%push mstrchr
	%ifstr %1
		%define %%source %1
	%else
		%defstr %%source %1
	%endif
	%ifstr %2
		%define %%what %2
	%else
		%defstr %%what %2
	%endif
	%strlen %%whatlen %%what
	%strlen %%srclen %%source
	%substr %%what %%what 1
	%assign %%i 1		;; source loop index (1 based index)
	%assign %%at 0		;; token found at
	%rep %%srclen
		%substr %%c %%source %%i		;; c = source[i]
		%ifidn %%c, %%what				;; if (c == what)
			%assign %%at %%i			;; 	at = i
			%exitrep					;; 	break
		%endif
		%assign %%i %%i+1				;; ++i
	%endrep
	%assign %%temp %%at
	%pop
	%assign %$mstrchr.ret %%temp		;; assign return value
%endmacro



; // Find substring in specified string
; // Returns 0 if substring not found
; // Else returns 1-based index of substring
; // Return value in  %$mstrstr.ret
; // ex:
; //	mstrstr some_var_thing, var
; //		-> 6
; //
%macro mstrstr 2.nolist
	%push mstrstr
	%ifstr %1
		%define %%source %1
	%else
		%defstr %%source %1
	%endif
	%ifstr %2
		%define %%str %2
	%else
		%defstr %%str %2
	%endif
	%strlen %%srclen %%source
	%strlen %%slen %%str
	%substr %%chfirst %%str 1
	%assign %%i 1				;; source loop index (1 based index)
	%assign %%at 0				;; substr found at
	%rep %%srclen
		%substr %%s %%source %%i, %%slen
		%ifidn %%s, %%str			;; if (s == str)
			%assign %%at %%i		;;  at = i
			%exitrep				;;  break
		%endif		
		%assign %%i %%i+1			;; ++i
	%endrep
	%assign %%temp %%at
	%pop
	%assign %$mstrstr.ret %%temp	;; assign return value
%endmacro



; // Helper macro for mcomparejmp
; // arg1: "A??B"
; // arg2: splitindex
; // arg3: splitwidth (1-2)
; // RESULT: cmp A,B
%macro msplitcmp 3
	%ifstr %1
		%define %%str %1
	%else
		%defstr %%str %1
	%endif
	%substr %%first %%str 1, %2-1
	%substr %%second %%str %2+%3, -1
	%deftok %%first %%first
	%deftok %%second %%second
	cmp		%%first, %%second
%endmacro



; // Helper macro for IF and WHILE
; // 	arg1: "A??B"
; // 	arg2: exitlabel (A??B -> false)
%macro mcomparejmp 2
	mstrstr %1, ==
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 2
		jne		%2
	%else
	mstrstr %1, !=
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 2
		je		%2
	%else
	mstrstr %1, >
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 1
		jle		%2
	%else
	mstrstr %1, <
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 1
		jge		%2
	%else
	mstrstr %1, >=
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 2
		jl		%2
	%else
	mstrstr %1, <=
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 2
		jg		%2
	%else
		%error "expected comparison operator (!=,==,>,<,>=,<=) in conditional expression"
	%endif
	%endif
	%endif
	%endif
	%endif
	%endif
%endmacro


; // Helper macro for REPEAT
; // 	arg1: "A??B"
; // 	arg2: continuelabel (A??B -> true)
%macro mcomparecont 2
	mstrstr %1, ==
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 2
		je		%2
	%else
	mstrstr %1, !=
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 2
		jne		%2
	%else
	mstrstr %1, >
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 1
		jge		%2
	%else
	mstrstr %1, <
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 1
		jle		%2
	%else
	mstrstr %1, >=
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 2
		jg		%2
	%else
	mstrstr %1, <=
	%ifnidn %$mstrstr.ret, 0
		msplitcmp %1, %$mstrstr.ret, 2
		jl		%2
	%else
		%error "expected comparison operator (!=,==,>,<,>=,<=) in conditional expression"
	%endif
	%endif
	%endif
	%endif
	%endif
	%endif
%endmacro



; // Macro IF condition, 1 arg
; // ex usage:
; //		.if eax == 0
; //			call	_zerofunc
; //		.elseif eax == 1
; //			call	_onefunc
; //		.else
; //			call	_defaultfunc
; //		.endif
; //			
%imacro .if 1
	%push ctxif			; create new context ".if"
	%$if:
	%assign %$ifid 1
	mcomparejmp %1, %$ifnext%$ifid
%endmacro
; // Macro ELSEIF, 1 arg
%imacro .elseif 1
	%ifctx ctxif
		%assign %%next %$ifid+1 ;; ifid += 1
		jmp		%$endif				;; JMP END if fallthrough from above
		%$ifnext%$ifid:
		mcomparejmp %1, %$ifnext%%next
		%assign %$ifid %%next   ; ifid = next
	%else
		%error "expected '.if' before '.elseif'"
	%endif
%endmacro
; // Macro ELSE, no args
%imacro .else 0
	%ifctx ctxif
		%assign %%next %$ifid+1
		jmp		%$endif				;; JMP END if fallthrough from above
		%$ifnext%$ifid:
		%assign %$ifid %%next
	%else
		%error "expected '.if' before '.else'"
	%endif
%endmacro
; // Macro ENDIF, no args
%imacro .endif 0
	%ifctx ctxif
		%$ifnext%$ifid:
		%$endif:
		%pop		; pop .else context
	%else
		%error "expected '.if', '.elseif' or '.else' before '.endif'"
	%endif
%endmacro




; // Macro WHILE condition, 1 args
; // ex usage:
; //		.while eax != 0	; while(i != 0)
; //			dec eax			; --i
; //		.endwhile
; //
; //		.while			; while(true)
; //			.break
; //		.endwhile
; //
%imacro .while 0-1
	%ifctx .repeat    ;; this is intended for repeat-while loop
		%$continue:
		%if %0 == 1
			mcomparecont %1, %$repeat ;; jmp %$repeat if true
		%endif
		%$break:
		%pop          ;; pop ".repeat" context
	%else
		%push .while			;; create new context ".while"
		%assign %$breakable 1 	;; declare as breakable
		%assign %$loopable 1	;; declare as loopable
		%$continue:
		%if %0 == 1
			mcomparejmp %1, %$break
		%endif
	%endif
%endmacro
; // Macro ENDWHILE, no args
%imacro .endwhile 0
	%ifctx .while
		jmp		%$continue
		%$break:
		%pop			;; pop ".while" context
	%else
		%error "expected a '.while' before '.endwhile'"
	%endif
%endmacro




; // Macro REPEAT condition, 0 args
; // ex usage:
; //		.repeat
; //			dec ecx
; //		.until ecx == 0
; //
; // 		.repeat
; // 			dec ecx
; // 		.while ecx != 0
; // 
%imacro .repeat 0
	%push .repeat			;; create new context ".repeat"
	%assign %$breakable 1 	;; declare as breakable
	%assign %$loopable 1	;; declare as loopable
	%$repeat:
%endmacro
; // Macro UNTIL, 1 arg, condition
%imacro .until 1
	%ifctx .repeat
		%$continue:
		mcomparejmp %1, %$repeat
		%$break:
		%pop			;; pop ".repeat" context
	%else
		%error "expected '.repeat' before '.until ...'"
	%endif
%endmacro



; // Macro SWITCH condition, 1 args
; // ex usage:
; //		.switch EAX
; //			.case 0
; //				mov	ecx, 5
; //				.break
; //			.case 1
; //				mov	ecx, 20
; //				.break
; //			.default
; //				mov ecx, 0
; //				.break
; //		.endswitch
; //
%imacro .switch 1
	%push .switch
	%define %$switchable %1
	%assign %$breakable 1 	;; declare as breakable
	%assign %$caseid 0
	jmp 	%$switchblock
%endmacro
%imacro .case 1
	%ifctx .switch
		%if %1 != %$caseid
			%error Case value %1 must match caseid %$caseid 
		%endif
		%$case%$caseid:				;; declare label
		%assign %$caseid %$caseid+1	;; next id
		pop	eax						;; restore EAX
	%else
		%error "expected '.switch' before '.case'"
	%endif
%endmacro
%imacro .default 0
	%ifctx .switch
		%define %$has_default		; we have a default case
		%$default:
	%else
		%error "expected '.switch' before '.default'"
	%endif
%endmacro
%imacro .endswitch 0
	%ifctx .switch
			jmp		%$.endswitch	;; fallthrough from above exit the switch
		%$switchblock:
			%assign %$MAX %$caseid
			cmp		%$switchable, %$MAX
			%ifdef %$has_default	;; if switchable is out of range
				jge %$default		;; jump to default case
			%else
				jge %$.endswitch	;; or out of the switch
			%endif
			;; use the casetable pointers to jump dynamically
			push eax ;; save EAX
			%ifid %$switchable ;; id like EAX or dword[ebp+8]
				%ifidni %$switchable, EAX
					mov eax, [%$casetable + EAX*4]
				%elifidni %$switchable, EBX
					mov eax, [%$casetable + EBX*4]
				%elifidni %$switchable, ECX
					mov eax, [%$casetable + ECX*4]
				%elifidni %$switchable, EDX
					mov edx, [%$casetable + EDX*4]
				%else
					mov eax, %$switchable
					mov eax, [%$casetable + EAX*4]
				%endif
			%elifnum %$switchable ;; a constant number
				mov eax, [%$casetable + %$switchable*4]
			%endif
			jmp eax ;; jump to case label
		%$casetable:				;; table of pointers
			%assign %$caseid 0
			%rep %$MAX
				dd %$case%$caseid
				%assign %$caseid %$caseid+1
			%endrep
		%$.endswitch:			;; end of switch, or switch exit
		%$break:
		%pop					;; remove context
	%else
		%error "expected '.switch' before '.endswitch'"
	%endif
%endmacro




; // Macro BREAK from loops, 0 args
; // ex usage:
; // .while EAX != 0
; //     .break
; // .endwhile
%imacro .break 0
	%ifidn %$breakable, 1			; lvl 1
		jmp %$break
	%elifidn %$$breakable, 1		; lvl 2
		jmp %$$break
	%elifidn %$$$breakable, 1		; lvl 3
		jmp %$$$break
	%elifidn %$$$$breakable, 1		; lvl 4
		jmp %$$$$break
	%elifidn %$$$$$breakable, 1		; lvl 5
		jmp %$$$$$break
	%elifidn %$$$$$$breakable, 1	; lvl 6
		jmp %$$$$$$break
	%elifidn %$$$$$$$breakable, 1	; lvl 7
		jmp %$$$$$$$break
	%elifidn %$$$$$$$$breakable, 1	; lvl 8
		jmp %$$$$$$$$break
	%else
		%error "expected '.while' or '.repeat' before '.break'"
	%endif
%endmacro




; // Macro CONTINUE loops, 0 args
; // ex usage:
; // .while EAX != 0
; //     .continue
; // .endwhile
%imacro .continue 0
	%ifidn %$loopable, 1			; lvl 1
		jmp %$continue
	%elifidn %$$loopable, 1			; lvl 2
		jmp %$$continue
	%elifidn %$$$loopable, 1		; lvl 3
		jmp %$$$continue
	%elifidn %$$$$loopable, 1		; lvl 4
		jmp %$$$$continue
	%elifidn %$$$$$loopable, 1		; lvl 5
		jmp %$$$$$continue
	%elifidn %$$$$$$loopable, 1		; lvl 6
		jmp %$$$$$$continue
	%elifidn %$$$$$$$loopable, 1	; lvl 7
		jmp %$$$$$$$continue
	%elifidn %$$$$$$$$loopable, 1	; lvl 8
		jmp %$$$$$$$$continue
	%else
		%error "expected '.while' or '.repeat' before '.continue'"
	%endif
%endmacro




; // Detect a proctype based on the symbol name
; // Return:
; //		0 - cdecl (default)
; //		1 - stdcall
; //		2 - fastcall
; //
%macro mdetectproc 1
	%defstr %%proc %1
	%substr %%c %%proc 1
	%ifidn %%c, '_'	;; cdecl: _func or stdcall: _func@N
		mstrchr %%proc, @
		%strlen %%len %%proc
		%assign %%diff %%len - %$mstrchr.ret
		%if %%diff <= 2				;; STDCALL?
			%assign %$proctype 1
			%substr %%nbytes %%proc %$mstrchr.ret+1,-1
			%deftok %$argbytes %%nbytes
		%else
			%assign %$proctype 0 	;; CDECL
		%endif
	%elifidn %%c, '@' ;; fastcall: @func@N
		mstrchr %%proc, @
		%strlen %%len %%proc
		%assign %%diff %%len - %$mstrchr.ret
		%if %%diff <= 2 ;; got args?
			%assign %$proctype 2    ;; FASTCALL
			%substr %%nbytes %%proc %$mstrchr.ret+1,-1
			%deftok %$argbytes %%nbytes
		%else
			%assign %$proctype 0    ;; CDECL
		%endif
	%else
		%assign %$proctype 0 		;; CDECL
	%endif
%endmacro



; // A somewhat safer version of INVOKE macro.
; // Supports CDECL, STDCALL
; // ex:
; //	invoke _function, 10, 20	; CDECL
; //	invoke _function@8, 10, 20	; STDCALL
; //
%imacro invoke 1-8.nolist
	%push .invoke
	%if %0 >= 1
		mdetectproc %1
		%assign %%pushargs %0-1
		%assign %%pushedbytes 0
		%rep	%%pushargs 			;; push all args from last to first
			%rotate -1
			%assign %%size 4
			%ifid %1				;; an identifier like EAX or label
				%ifidni %1, AX
					%assign %%size 2
				%elifidni %1, BX
					%assign %%size 2
				%elifidni %1, CX
					%assign %%size 2
				%elifidni %1, DX
					%assign %%size 2
				%elifidni %1, BP
					%assign %%size 2
				%elifidni %1, SP
					%assign %%size 2
				%elifidni %1, DI
					%assign %%size 2
				%elifidni %1, SI
					%assign %%size 2
				%endif
			%endif
			
			%ifstr %1 ;; is this a string literal: '' "" `` ?
				asciiz %%tmpstr_%[%%pushedbytes], %1
				push %%tmpstr_%[%%pushedbytes]
			%elif %%size = 2 		;; pushing AX:SI, 16-bit values
				push strict word %1
			%else					;; pushing DWORD 32-bit values
				push strict dword %1
			%endif
			%assign %%pushedbytes %%pushedbytes + %%size
		%endrep
		%rotate -1 					;; to initial state
		%if %$proctype == 1 ;; STDCALL
			%if %$argbytes != %%pushedbytes
				%fatal STDCALL %1 : argument count mismatch. Pushed %%pushedbytes bytes instead of %$argbytes 
			%endif
		%endif
		call %1						;; call mangled symbol
		%if %$proctype == 0
			add	esp, %%pushedbytes	;; stack cleanup for CDECL
		%endif
	%else
		%fatal 'invoke usage: invoke _proc, a, b, ...'
	%endif
	%pop
%endmacro






; // Uses statement for saving registers in globalfunc
; //
; // usage:
; //     uses EBX, ESI, EDI
; // example:
; //     uses ebx
; //
%macro  uses 1-* 
	%ifctx .func
		%assign %$saved_count 0
		%rep %0
			%rotate -1
			%ifidni %1, ebx
			%elifidni %1, esi
			%elifidni %1, edi
			%else
				%fatal USES: only EBX, ESI or EDI registers allowed, got: %1
			%endif
			%define %$saved_%[%$saved_count] %1
			%assign %$saved_count %$saved_count+1
		%endrep
	%else
		%fatal USES expects a previous GLOBALFUNC definition
	%endif
%endmacro




; // Macro prologue
; //
; // usage:
; //	prologue [localbytes]
; // example:
; //	Simple prologue, no local vars
; //		prologue
; //	Reserving 8 bytes for local vars
; //		prologue 8
; //
%imacro prologue 0-1
	%ifctx .func ; we have locals
		%define %$prologue
		push   ebp
		mov    ebp, esp
		%if %$localbytes != 0
			%if %0 == 1
				%warning PROLOGUE preceded by LOCAL-s, localbytes argument ignored
			%endif
			sub    esp, %$localbytes
		%elif %0 == 1
			sub    esp, %1
		%endif
		%ifdef %$saved_count ;; push regs?
			%assign %%index 0
			%rep %$saved_count
				push   %$saved_%[%%index]
				%assign %%index %%index+1
			%endrep
		%endif
	%else
		%push .prologue ;; create new prologue context
		push   ebp
		mov    ebp, esp
		%if %0 == 1
			sub    esp, %1
		%endif
	%endif
%endmacro




; // Macro epilogue
; //
; // usage:
; //	epilogue [cleanupbytes for stdcall]
; // example:
; //	Simple epilogue, not stdcall
; //		epilogue
; //	An stdcall function with 8 bytes of args
; //		epilogue 8
; //
%imacro epilogue 0-1
	%ifctx .func
		%ifdef %$saved_count
			%assign %%index %$saved_count
			%rep %$saved_count ;; reverse iterate:
				%assign %%index %%index-1
				pop    %$saved_%[%%index]
			%endrep
		%endif
		mov   esp, ebp
		pop   ebp
		%if %0 = 1
			ret %1
		%else
			ret
		%endif
		%pop  ;; erase context
	%elifctx .prologue
		mov		esp, ebp
		pop		ebp
		%if %0 = 1
			ret %1
		%else
			ret
		%endif
		%pop  ;; erase context
	%else
		%fatal EPILOGUE expects a previous PROLOGUE or GLOBALFUNC definition
	%endif
%endmacro



; // Declares a new local variable. Assumes a function PROLOGUE will be set
; // ex:
; //
%imacro local 1-*.nolist
	%ifnctx .func
		%fatal LOCAL expects a previous GLOBALFUNC definition
	%endif
	%rep %0
		%ifstr %1
			%define %%str %1
		%else
			%defstr %%str %1
		%endif
		mstrchr %%str, ':'
		%if %$mstrchr.ret == 0
			%fatal LOCAL expects a type definition:  "local myvar:TYPE"
		%else
			%substr %%first %%str 1, %$mstrchr.ret-1
			%substr %%second %%str %$mstrchr.ret+1, -1
			%deftok %%first %%first		;; myvar
			
			mstrchr %%second, '['
			%if %$mstrchr.ret == 0
				%deftok %%second %%second	;; dword
				%ifidni %%second, QWORD
					%assign %$localbytes %$localbytes+8
					%assign %[%%first].value %$localbytes
					%define .%[%%first]  qword[ebp-%[%%first].value]
					%define @%[%%first]  ebp-%[%%first].value
				%elifidni %%second, DWORD
					%assign %$localbytes %$localbytes+4
					%assign %[%%first].value %$localbytes
					%define .%[%%first]  dword[ebp-%[%%first].value]
					%define @%[%%first]  ebp-%[%%first].value
				%else
					%fatal LOCAL expects :QWORD or :DWORD as TYPE
				%endif
			%else
				%substr %%third  %%second %$mstrchr.ret+1, -2
				%substr %%second %%second 1,%$mstrchr.ret-1
				%deftok %%second %%second
				%deftok %%third %%third
				
				%ifidni %%second, QWORD
					%assign %$localbytes %$localbytes+(8*%%third)
					%assign %[%%first].value %$localbytes
					%define .%[%%first]  qword[ebp-%[%%first].value]
					%define @%[%%first]  ebp-%[%%first].value
				%elifidni %%second, DWORD
					%assign %$localbytes %$localbytes+(4*%%third)
					%assign %[%%first].value %$localbytes
					%define .%[%%first]  dword[ebp-%[%%first].value]
					%define @%[%%first]  ebp-%[%%first].value
				%elifidni %%second, WORD
					%assign %$localbytes %$localbytes+(2*%%third)
					%assign %[%%first].value %$localbytes
					%define .%[%%first]  word[ebp-%[%%first].value]
					%define @%[%%first]  ebp-%[%%first].value
				%elifidni %%second, BYTE
					%assign %$localbytes %$localbytes+(1*%%third)
					%assign %[%%first].value %$localbytes
					%define .%[%%first]  byte[ebp-%[%%first].value]
					%define @%[%%first]  ebp-%[%%first].value
				%else
					%fatal LOCAL expects :QWORD, :DWORD, :WORD or :BYTE as TYPE
				%endif
			%endif
		%endif
		%rotate 1	;; next macro arg
	%endrep
%endmacro


; // Create a global function declaration in global scope
; // ex:
; //	globalfunc _astrlen, str:dword
; //        prologue
; //		xor   eax, eax   ;; return 0;
; //		epilogue
; //
%imacro globalfunc 1-*.nolist
	[section .text]
	global %1
	%1:
	%ifctx .func
		%repl .func		;; delete prev func context and replace with a new one
	%else
		%push .func		;; create a new .FUNC context
	%endif
	%assign %$parambytes 8	;; always EBP+8 for first offset
	%assign %$localbytes 0	;; default value
	
	%rep %0-1
		%rotate 1	;; next macro arg
		%ifstr %1
			%define %$str %1
		%else
			%defstr %$str %1
		%endif
		mstrchr %$str, ':'
		%if %$mstrchr.ret == 0
			%fatal PARAM expects a type definition:  "param myvar:TYPE"
		%else
			%substr %$first %$str 1, %$mstrchr.ret-1
			%substr %$second %$str %$mstrchr.ret+1, -1
			%deftok %$first %$first
			%deftok %$second %$second
			%assign %[%$first].value %$parambytes

			%ifidni %$second, QWORD
				%define .%[%$first]  qword[ebp+%[%$first].value]
				%define @%[%$first]  ebp+%[%$first].value
				%assign %$parambytes %$parambytes+8
			%elifidni %$second, DWORD
				%define .%[%$first]  dword[ebp+%[%$first].value]
				%define @%[%$first]  ebp+%[%$first].value
				%assign %$parambytes %$parambytes+4
			%else
				%fatal PARAM expects :QWORD or :DWORD as TYPE, got %%second
			%endif
		%endif
	%endrep
%endmacro


%endif	; include guard