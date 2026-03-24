#!/bin/bash

# Настройка порта (измените, если у вас другой, например /dev/ttyACM1)
PORT="/dev/ttyACM0"
CHIP="m32u4"
PROG="avr109"

echo "--- Утилита управления защитой Arduino Micro ---"
echo "1. Заблокировать (Lock 0xFC)"
echo "2. Разблокировать и стереть (Unlock + Erase)"
read -p "Выберите действие [1-2]: " choice

if [ "$choice" == "1" ]; then
    echo "Нажмите RESET на плате дважды и нажмите Enter здесь..."
    read
    avrdude -p $CHIP -c $PROG -P $PORT -U lock:w:0xFC:m
elif [ "$choice" == "2" ]; then
    echo "Нажмите RESET на плате дважды и нажмите Enter здесь..."
    read
    avrdude -p $CHIP -c $PROG -P $PORT -e -U lock:w:0x3F:m
else
    echo "Отмена."
fi