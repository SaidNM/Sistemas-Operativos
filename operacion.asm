section .data
	msg db "EL incremento de 2 es ",0x0A,0x0D     ;0x0A --Salto de linea
													  ;0x0D --Salto de carro
	tamanio equ $-msg
	
	mensaje1 db 0x0A,"El decremento de 2 es",0x0D
	tamanio1 equ $-mensaje1
	
	mensaje 2 db 0x0A, "La suma de 2+2 es",0x0A,0x0D
	tamanio2 equ $-mensaje2

section .bss
	resultado resb 1 ;variable temporal de 1 byte

section .text
	global _start

	_start:
; ---Impresion del primer mensaje --
		mov eax,4
		mov ebx,1
		mov ecx,msg
		mov edx,tamanio
		int 80h
;operacion de incremento 
	    mov eax,0x02
		inc eax
		add eax,'0'
		mov [resultado],eax
;imprimir variable temporal	
		mov eax,4
		mov ebx,1
		mov ecx,resultado
		mov edx, 1
		int 80h
;decremento
		mov eax,4
		mov ebx,1
		mov ecx,mensaje1
		mov edx,tamanio1
		int 80h
;Operacion decremento 
		mov eax,0x02
		dec eax
		add eax,'0'
		mov [resultado],eax
;impresion del decremento 
		mov eax,4
		mov ebx,1
		mov ecx,resultado
		mov edx,1
		int 80h
;impresion de la suma 
		mov eax,4
		mov ebx,1
		mov ecx,mensaje2
		mov edx,tamanio2
		int 80h
;operacion suma 
		mov eax,0x02
		mov ebx,0x02
		add eax,ebx 
		add eax,'0'
		mov [resultado],eax
;imprimir suma realizada 
		mov eax,4
		mov ebx,1
		mov ecx,resultado
		mov edx,1
		int 80h
;finalizar programa
	mov rax,1
	mov rbx,0
	int 80h



