# Trabajos finales
- curso: Sistemas Operativos de Proposito General
- cohorte: 16 - 2022
- link oficial: https://campus.fi.uba.ar/course/view.php?id=1444
# TP 1
## consideraciones
Se usa como archivos base los que se encuentran en la carpeta _clase_4_ (_reader.c_ y _writer.c_)
## ejecucion
- ubicate en la carpeta _TP1_
- ejecuta el archivo _compilar.sh_ para ejecutar los archivos objetos
- ir a la carpeta _objetos_ y ejecuta los archivos objectos en distintas terminales: 
```bash
./writer
./reader
```
## log...
- escribe lo que quieras dentro de la terminal de writer
## sign...
- Dentro de la carpeta _automatizacion_ ejecutar el siguiente comando y copia el ID del proceso en la fila PID.
```bash
./id.sh
```
- Asumamos que el ID del proceso _writer_ es _1234_ y que queremos mandar las señales 1 y 2 al archivo _Sign.txt_. Para ello ejecutar el file _bash.sh_ dos veces de la siguiente manera:

```bash
./bash.sh 1 1234
./bash.sh 2 1234
```

sal de writer con _control + c_. entra a la carpeta _salidas_ y revisarlos los archivos _Log.txt_ y _Sign.txt_ y ver los resultados.

# TP 2
## consideraciones
- para iniciar el proyecto, tomamos como archivo base el archivo ubicado en la siguiente ubicacion: __clase_6/practica_clase6/server_tcp.c__
- la plantilla del TP2 se extrajo de la carpeta __clase_6/Tp_Final_V2_domotica_campus__
- una vez copiado todos los archivos se cambio la primera linea de los archivos de la carpeta __cgi-bin__ de __#! /usr/bin/env python__ por __#! /usr/bin/env python3__


## ejecucion
Dentro del la carpeta del repositorio ejecutar los siguientes comando en terminales distintas.

```bash
# terminal 1: Desde la carpeta del repositorio...
cd ./TP2/EmuladorHardware
python3 Emulador.py
# terminal 2: Desde la carpeta del repositorio...
cd ./TP2/SerialServiceEsqueleto
./compilar.sh
./serialService
# terminal 3: Desde la carpeta del repositorio...
cd ./TP2/InterfaceService
python3 Main_python3.py
# terminal 4: Desde la carpeta del repositorio...
cd ./TP2/web
./runserver_python3.sh
```

# organizacion de carpetas
```
|-- bimestre-3-SOPG-2 (carpeta de repositorio)
    |-- clase_3 (descargada del link del curso)
    |-- clase_4 (descargada del link del curso)
    |-- clase_5 (descargada del link del curso)
    |-- clase_6 (descargada del link del curso)
    |-- ejecutables (ejecutables de las clases. Esto es para no modificar los archivos descagardos)
    |-- TP1 (trabajo final)
    |-- TP2 (trabajo final)
    |-- ejecutar.sh (compila los archivos C de las clases en la carpeta ejecutables)
```
