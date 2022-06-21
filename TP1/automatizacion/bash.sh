#!/bin/bash
# $1 es para ingresar 1 o 2 (SIGUSR1 y SIGUSR2 respectivamente)
# $2 es el parametro del PID del programa writer.c
echo "se ejecuta señal SIGUSR:"
kill -s USR$1 $2