@echo off

REM Verifica si se pasó un argumento (ej: "ejercicioX")
IF "%~1"=="" (
    echo Error: Debes especificar el nombre del ejercicio.
    echo Uso: translate [nombre_base]
    GOTO :EOF
)

REM Guarda el directorio actual y se mueve a la carpeta "Test" RELATIVA
pushd Test

REM Verifica si la carpeta "Test" existe en el directorio actual
IF %ERRORLEVEL% NEQ 0 (
    echo Error: No se pudo encontrar la carpeta "Test" en este directorio.
    popd
    GOTO :EOF
)

REM Ejecuta tu comando (asumiendo que vmtV2.exe está DENTRO de Test)
vmtV2.exe %1.asm %1.vmx -o

REM Regresa al directorio original desde donde llamaste al script
popd