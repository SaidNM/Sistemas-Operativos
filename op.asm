section .data
	mensaje db "El incremento de 2 es "
	tam equ $-mensaje

	mensaje1 db 0x0A, "El Decremento de 5 es "
	tam1 equ $-mensaje1

	mensaje2 db 0x0A, "La suma de 1 y 3 es "
	tam2 equ $-mensaje2

	mensaje3 db 0x0A, "La resta de 6 y 4 es "
	tam3 equ $-mensaje3

section .bss
	resultado resb 1

section .text
	global _start

	_start:
; Impresion del primer mensaje 
	mov eax,4
	mov ebx,1
	mov ecx,mensaje
	mov edx,tam
	int 80h
;operacion de incremento 
	mov eax, 0x02
	inc eax
	add eax, '0'
	mov [resultado],eax
;imprimir variable temporal con incremento
	mov eax,4
	mov ebx,1
	mov ecx,resultado
	mov edx,1
	int 80h
; Impresion del segundo mensaje 
	mov eax,4
	mov ebx,1
	mov ecx,mensaje1
	mov edx,tam1
	int 80h
;operacion de decremento 
	mov eax, 0x05
	dec eax
	add eax, '0'
	mov [resultado],eax
;impresion de la variable con decremento 
	mov eax,4
	mov ebx,1
	mov ecx,resultado
	mov edx,1
	int 80h
;impresion del tercer mensaje 
	mov eax,4
	mov ebx,1
	mov ecx,mensaje2
	mov edx,tam2
	int 80h
;operacion suma 
	mov eax, 0x01
	mov ebx, 0x03
	add eax,ebx
	add eax, '0'
	mov [resultado],eax
;impresion el resultado de la suma 
	mov eax,4
	mov ebx,1
	mov ecx,resultado
	mov edx,1
	int 80h
;impresion del cuarto mensaje 
	mov eax,4
	mov ebx,1
	mov ecx,mensaje3
	mov edx,tam3
	int 80h
;operacion de resta 
	mov eax, 0x06
	mov ebx, 0x04
	sub eax,ebx
	add eax, '0'
	mov [resultado],eax
;impresion del resultado de la resta
	mov eax,4
	mov ebx,1
	mov ecx,resultado
	mov edx,1
	int 80h
;impresion del quinto mensaje 

;operacion de multiplicacion

;impresion del resultado de la multiplicacion 

;fin del programa 
	mov rax,1
	mov rbx,0
	int 80h


