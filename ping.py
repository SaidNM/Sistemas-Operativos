#!/usr/bin/python

import subprocess, sys


def ping(ip):
	ret = subprocess.call(['ping','-c','3','-w','5',ip], stdout=open('/dev/null','w'), stderr=open('/dev/null','w')) 
	return ret==0

def conexion():
	sic=open('conectadas.txt','w+')
	noc=open('noconectadas.txt','w+')
	ip=''
	for r in range(1,255):
		ip=sys.argv[1].replace('x',str(r))
		if ping(ip)==0:
			print("No:"+ip)
			no.write(ip+'\n')
		else:
			print("Si: "+ip)
			si.write(ip+'\n')
	sic.close()
	noc.close()
conexion()


