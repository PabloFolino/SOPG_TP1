# SOPG_TP1
TP1 de la materia Sistema Operativo de Propósitos Generales de la carrera Especialización de Sistemas Embebidos FIUBA.

# Objetivo:
Comunicar dos procesos por medio de un named fifo. El proceso Writer podrá recibir texto por la consola y signals. El proceso Reader deberá loguear en un archivo el texto que recibir el proceso Writer y en otro archivo las signals que recibe el proceso Writer.

 Partes del sistema:
 
  Proceso Writer:
    Este proceso iniciará y se quedará esperando que el usuario ingrese texto hasta que presione
    ENTER. En ese momento escribirá en una named fifo los datos con el siguiente formato:
    
                               DATA:XXXXXXXXX
    
    En cualquier momento el proceso podrá recibir las signals SIGUSR1 y SIGUSR2. En dicho caso deberá escribir en el named fifo el siguiente mensaje:
SIGN:1 o SIGN:2

  Proceso Reader:
  Este proceso leerá los datos del named fifo y según el encabezado “DATA” o “SIGN” escribirá en el
  archivo log.txt o signals.txt.

