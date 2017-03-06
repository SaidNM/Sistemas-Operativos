section.data ; seccion de datos 
	msg db "Hola Mundo",0x0A ;el 10 corresponde al codigo ASCII del salto de linea
							 ;se puede indicar en decimal, octal o hexadecimal
							 ;db es una variable 
	length equ $-msg      	 ;Calcula el tamaño del mensaje, se utiliza la palabra reservada 'equ'
section.text				 ;Inicio de la seccion de código 
	global _start			 ;Declaracion de una variable global

	_start:					 ;Inicio de codigo 
	;---------- Impresión en pantalla ------------------
	mov eax,4 				 ;Pon el valor 4 en el registro eax, Llamada al sistema, equivalente a un 'sys_write  
							 ;Máximo se utilizan 32 bits, ya que es el valor maximo del registro eax
	mov ebx,1				 ;Decimos que utilizamos stdout 
	mov ecx,msg 			 
	mov edx,length
	int 80h					 ;Palabra reservada para interrupcion, en este caso es la 80 


