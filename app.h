/*
* Основная логика приложения
*/
#pragma once
#include <Arduino.h>
#include "flashcfg.h"
#include "keypad.h"
#include "mode_sw.h"
#include "cmd.h"

class App {
  public:
    App(flashcfg * c, Keypad * k, ModeSW * s, CMD * cmd);
    void run(); // Выполнение периодических заданий
  private:
    flashcfg * _cfg; // Конфигурация
    Keypad * _keys;  // Клавиатура
    ModeSW * _sw;    // Переключатель режимов
    CMD * _cmd;      // Коммандный интерфейс
    uint8_t _test_mode = false; // Флаг включения тестового режима

    // Отработать нажатие на кнопку
    void _processKeys();

    // Отправить последовательность для HTML. key_num - номер кнопки в массиве конфигурации
    void _sendHTML(const uint8_t key_num);

    // Отправить последовательность для гнома. key_num - номер кнопки в массиве конфигурации
    void _sendGnome(const uint8_t key_num);

    // Отправить последовательность для винды. key_num - номер кнопки в массиве конфигурации
    void _sendWin(const uint8_t key_num);

    // Просто вывести тестовую информацию
    void _printTest(const __FlashStringHelper* mode, const uint8_t key_num);

    // Отработать команды
    void _processCmd();
};//class