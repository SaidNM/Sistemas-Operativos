#!/usr/bin/python3

def menu():
	print("\t\t----- Menu -----")
	print("1.-Informacion del procesador")
	print("2.-Uso del CPU")
	print("3.-Uso de memoria")
	print("4.-Uso de uso de disco duro")

def infoprocesador():
	try:
		info=open("/proc/cpuinfo")
	except:
		print("Error al abrir el archivo")

	print("-------------------------Informacion del procesador------------------------")
	for line in info:
		print(info.readline())
	info.close()
#-----------------------------------------------------------------------------------------------------
def uso_mem():
	aux=''
	mem=''
	valores_porcentaje=[]
	mensaje_valor=[]
	contador=0
	try:
		uso=open("/proc/meminfo")
	except:
		print("Error al abrir el archivo")

	for linea in uso.read():
		aux=aux+linea
		if(linea==":"):
			mensaje_valor.append(aux)
			aux=''
		elif(linea=="\n"):
			valores_porcentaje.append(aux)
			aux=''
			contador=contador+1	
	print("---------------------Informacion del uso de memoria--------------------------")
	imprimir_uso_memoria(mensaje_valor,valores_porcentaje,contador)
	uso.close()


def imprimir_uso_memoria(mensaje,valor,contador):
	memTotal= obtener_valor(valor[0])

	for i in range(contador):
		print(mensaje[i]+"%6.2f" % float(conversion_porcentaje(obtener_valor(valor[i]),memTotal))+"%")

def conversion_porcentaje(valor,memTotal):
	porcentaje=(valor*100)/memTotal
	return porcentaje
		
def obtener_valor(valor):
	num=''
	for elemento in valor:
		if (elemento.isdigit() == True):
			num=num+elemento
	return int(num)
#------------------------------------------------------------------------------------------------------------------------------------------
def uso_cpu():
	try:
		archivo=open("/proc/stat")
	except:
		print("Error al abrir el archivo")
	print("----------------------Informacion del CPU--------------------------")
	for elemento in archivo.readlines():
		analisis_cpu(elemento)
	archivo.close()

def analisis_cpu(informacion):
	aux=''
	cpu=[]
	for palabra in informacion:
		if(palabra!=' '):
			aux=aux+palabra
		if(palabra==' 'or palabra=='\n'):
			if(aux==''):
				continue
			cpu.append(aux)
			aux=''
	imprimir_analisis_cpu(cpu)
	
def imprimir_analisis_cpu(cpu):
	try:
		if(cpu[0]!='intr' and cpu[0]!='softirq'):
			total=int(cpu[1])+int(cpu[2])+int(cpu[3])+int(cpu[4])+int(cpu[5])+int(cpu[6])+int(cpu[7])
			valor=int(cpu[1])+int(cpu[2])+int(cpu[3])+int(cpu[6])+int(cpu[7])
			porcentaje=conversion_porcentaje(valor,total)
			print(cpu[0]+"  "+"%0.2f" % porcentaje + "%")
	except:
		print(cpu[0]+" "+cpu[1])

#-------------------------------------------------------------------------------------------------------------------------------------
def uso_disco():
	info_disco=[]
	try:
		archivo=open("/proc/diskstats")
	except:
		print("Error al abrir el archivo")

	for line in archivo.readlines():
		if(line.find("sd")!=-1):
			info_disco.append(line)
	analisis_disco(info_disco)

def analisis_disco(info_disco):
	for info in info_disco:
		imprimir_analisis_disco(info)

def imprimir_analisis_disco(info):
	list_info=[]
	aux=''
	for palabra in info:
		if(palabra!=' '):
			aux=aux+palabra
		elif(palabra==' ' and len(aux)>0):
			list_info.append(aux)
			aux=''
	print("--------------")
	print("Disco: "+list_info[2])
	print("Lecturas realizadas: "+list_info[3])
	print("Num Sectores leidos: "+list_info[5])
	print("Tiempo de lectura: "+"%6.2f" % (float(list_info[6])*0.001) +" seg")
	print("Escrituras completas: "+list_info[7])
	print("Num sectores escritos: "+list_info[9])
	print("Tiempo de escritura: "+"%6.2f" % (float(list_info[10])*0.01) +" seg")
	print("Tiempo de I/O: "+"%6.2f" % (float(list_info[12])*0.01) +" seg")	




#---------------------------------------------------------------------------------------------------------------------------------
def main():
	menu()
	op=input("Elija un opcion: ")
	if(op=='1'):
		infoprocesador()
	elif(op=='2'):
		uso_cpu()
	elif(op=='3'):
		uso_mem()
	elif(op=='4'):
		uso_disco()
	else:
		print("Introduzca una opcion correcta")
main()