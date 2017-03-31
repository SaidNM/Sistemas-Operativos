section .data
	mensaje db "Inserte un numero: ",0x0A
	tam1 equ $ -mensaje 

	mensaje2 db "El numero introducido fue : ",0x0A
	tam2 equ $ -mensaje2
section .bss
	resultado resb 6
	
section .text
	global _start

	_start:
;impresion del primer mensaje 
	mov eax,4
	mov ebx,1
	mov ecx,mensaje
	mov edx,tam1
	int 80h
;lectura del numero introducido 
	mov eax,3 		;valor de sistema para lectura
	mov ebx,2  		;salida de stdin
	mov ecx,resultado
	mov edx,5
	int 80h
;imprimir segundo mensaje 
	mov eax,4
	mov ebx,1
	mov ecx,mensaje2
	mov edx,tam2
	int 80h
;impresion de el stdin
	mov eax,4
	mov ebx,1
	mov ecx,resultado
	mov edx,5
	int 80h
;cerrar programa

	mov rax,1
	mov rbx,0
	int 80h