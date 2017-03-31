section .data
	mensaje db "El incremento de 2 es "
	tam equ $-mensaje

	mensaje1 db 0x0A, "El Decremento de 5 es "
	tam1 equ $-mensaje1

	mensaje2 db 0x0A, "La suma de 1 y 3 es "
	tam2 equ $-mensaje2

	mensaje3 db 0x0A, "La resta de 6 y 4 es "
	tam3 equ $-mensaje3

	mensaje4 db 0x0A, "La multiplicacion de 3 y 3 es "
	tam4 equ $-mensaje4

	mensaje5 db 0x0A, "La division de 5 y 2 es "
	tam5 equ $-mensaje5

	mensaje6 db 0x0A, "La multiplicacion con signo de -3 y 2 es "
	tam6 equ $-mensaje6

	mensajeR db " el residuo es "
	tam7 equ $-mensajeR
section .bss
	resultado resb 1
	resultado1 resb 2

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
	mov eax,4
	mov ebx,1
	mov ecx,mensaje4
	mov edx,tam4
	int 80h

;operacion de multiplicacion
	mov eax, 0x03
	mov ebx, 0x03
	mul ebx
	add eax,'0'
	mov [resultado1],eax
;impresion del resultado de la multiplicacion 
	mov eax,4
	mov ebx,1
	mov ecx,resultado1
	mov edx,2
	int 80h

;impresion del sexto mensaje 
	mov eax,4
	mov ebx,1
	mov ecx,mensaje5
	mov edx,tam5
	int 80h
;operacion de division
	mov al, 0x05
	mov bl, 0x02
	div bl
	add al,'0'
	add ah, '0'
	mov [resultado1],al
	mov [resultado],ah
;impresion del resultado de la multiplicacion
	mov eax,4
	mov ebx,1
	mov ecx,resultado1
	mov edx,2
	int 80h
;impresion del mensajeR de la multiplicacion
	mov eax,4
	mov ebx,1
	mov ecx,mensajeR
	mov edx,tam7
	int 80h
;impresion del residuo de la multiplicacion
	mov eax,4
	mov ebx,1
	mov ecx,resultado
	mov edx,1
	int 80h
;impresion del mensaje de la multiplicacion con signo
	mov eax,4
	mov ebx,1
	mov ecx,mensaje6
	mov edx,tam6
	int 80h

;multiplicacion con signo 
	mov al,-3
	mov bl,2
	imul bl 
	mov [resultado],al

	mov al,[resultado]
	neg al
		js print_imul
	mov [resultado],al


	mov eax,4
	mov ebx,1
	mov ecx,resultado
	mov edx,1
	int 80h

	print_imul:
	xor al,al
	mov al,[resultado]
	add al,'0'
	mov [resultado],al

	mov eax,4
	mov ebx,1
	mov ecx,resultado
	add edx,1
	int 80h


;fin del programa 
	mov rax,1
	mov rbx,0
	int 80h


