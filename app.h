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

    // Отработать нажатие на кнопку
    void _processKeys();

    // Отправить последовательность для HTML
    void _sendHTML(const uint16_t code);

    // Отправить последовательность для гнома
    void _sendGnome(const uint16_t code);

    // Отправить последовательность для винды
    void _sendWin(const uint16_t code);

    // Отработать команды
};//class