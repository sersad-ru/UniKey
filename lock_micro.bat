@echo off
setlocal
:: Укажите ваш COM-порт
set PORT=COM3
:: Путь к avrdude (проверьте этот путь у себя!)
set AVR_PATH="%LOCALAPPDATA%\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\bin\avrdude.exe"
set CONF_PATH="%LOCALAPPDATA%\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17\etc\avrdude.conf"

echo 1. Заблокировать клавиатуру
echo 2. Стереть и разблокировать
set /p choice="Выберите вариант: "

if "%choice%"=="1" (
    echo Нажмите RESET на плате дважды и нажмите любую клавишу...
    pause > nul
    %AVR_PATH% -C %CONF_PATH% -p m32u4 -c avr109 -P %PORT% -U lock:w:0xFC:m
) else if "%choice%"=="2" (
    echo Нажмите RESET на плате дважды и нажмите любую клавишу...
    pause > nul
    %AVR_PATH% -C %CONF_PATH% -p m32u4 -c avr109 -P %PORT% -e -U lock:w:0x3F:m
)

pause