#https://www.youtube.com/watch?v=xRNRWp3e3Ro
# no olvidar hacer sudo chmod u+x "nombre del archivo shell"

#!/bin/bash
echo "compilando..."
gcc ../writer.c -o ../objectos/writer
gcc ../reader.c -o ../objectos/reader
echo "compilo"
