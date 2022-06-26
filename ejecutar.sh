#!/bin/bash
#echo "hola manooo"
#gcc ./clase_3/ejemploSignals.c -o ./ejecutables/clase_3_ejemploSignals

#gcc ./clase_4/ejemplo_named_fifo/reader.c -o ./ejecutables/clase_4_reader
#gcc ./clase_4/ejemplo_named_fifo/writer.c -o ./ejecutables/clase_4_writer

#gcc ./clase_5/ejemplos_sockets_2021/ejemplos/cliente_tcp.c -o ./ejecutables/clase_5_cliente_tcp
#gcc ./clase_5/ejemplos_sockets_2021/ejemplos/server_tcp.c -o ./ejecutables/clase_5_server_tcp

gcc -pthread ./clase_6/Ejemplos_Threads_2019/Ejemplo_Cancel/main.c -o ./ejecutables/clase_6_Ejemplo_Cancel
#gcc -pthread ./clase_6/Ejemplos_Threads_2019/Ejemplo_Join/main.c -o ./ejecutables/clase_6_Ejemplo_Join
#gcc -pthread ./clase_6/Ejemplos_Threads_2019/Ejemplo_signals/main.c -o ./ejecutables/clase_6_Ejemplo_signals
#gcc ./clase_6/practica_clase6/cliente_tcp.c -o ./ejecutables/clase_6_cliente_tcp
#gcc ./clase_6/practica_clase6/server_tcp.c -o ./ejecutables/clase_6_server_tcp
