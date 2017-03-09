#!/bin/bash
gnome-terminal -e "/bin/bash -c 'nasm -f elf64 -o resultado.o $1; ld -s resultado.o -o run; ./run; echo; read -p 'Pulse Intro';exit' "
