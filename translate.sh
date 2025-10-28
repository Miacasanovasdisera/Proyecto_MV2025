#!/bin/bash

# --- Configura el nombre del ejecutable (relativo a la carpeta Test) ---
# EXECUTABLE_NAME="./vmtV2"
EXECUTABLE_NAME="vmtV2.exe" # Si usas Wine

# Verifica si se pasó un argumento (ej: "ejercicioX")
if [ -z "$1" ]; then
    echo "Error: Debes especificar el nombre del ejercicio."
    echo "Uso: translate [nombre_base]"
    exit 1
fi

# Guarda el directorio actual y se mueve a la carpeta "Test" RELATIVA (modo silencioso)
pushd ./Test > /dev/null

# Verifica si la carpeta "./Test" existe
if [ $? -ne 0 ]; then
    echo "Error: No se pudo encontrar la carpeta 'Test' en este directorio."
    exit 1
fi

# Ejecuta tu comando usando el primer argumento ($1)
wine "$EXECUTABLE_NAME" "$1.asm" "$1.vmx" -o

# Regresa al directorio original (modo silencioso)
popd > /dev/null