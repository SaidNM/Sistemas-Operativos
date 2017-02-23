#!/usr/bin/python

import subprocess, time

hosts=('8.8.8.8','kernel.org','yahoo.com')

def ping(hosts):
	ret=subprocess.call(['ping','-c','3','-w','5',hosts], stdout=open('/dev/null','w'), stderr=open('/dev/null','w')) #llamada al proceso 
																													  #stdout-->salida estandar           /dev/null---tiene una terminal
																													  #stderr-->salida estandar de error
																													  #stdin-->entrada estandar
	return ret == 0

def net_is_up():
	print "[%s] check if network is up..." % time.strftime("%Y-%m-%d %H-%M-%S")
	xstatus=1

	for h in hosts:
		if ping(h):
			print "[%s] network is up..." % time.strftime("%Y-%m-%d %H-%M-%S")
			xstatus=0
			break
	if xstatus:
		print "[%s] network is down..." % time.strftime("%Y-%m-%d %H-%M-%S")
	return xstatus

quit(net_is_up())