/* Переключатель режимов */

/*
  На основе pinout.h
  Ассинхронно сканирует переключатель и отображает светодиодом его состояние
*/

#pragma once
#include <Arduino.h>
#include <ssExecutor.h>
#include "pinout.h"

// Режимы (соотвествуют цвету светодиода и положению переключателя)
typedef enum {
  SW_GREEN  = B0001, // Цвет зеленый, переключатель вверх (к USB-разъему)
  SW_YELLOW = B0011, // Цвет желтый, переключатель посредине
  SW_RED    = B0010  // Цвет красный, переключатель вниз (от USB-разъема)
} SWState;

class ModeSW  : public ssExecutor {
  public:
    ModeSW();

    // Получить состояние переключателя
    SWState getState(){_was_changed = false; return _state;};

    // Вернет 1, если состояние изменилось после последнего вызова getState, иначе вернет 0
    uint8_t wasChanged(){ return _was_changed;};

    // Периодическая работа
    void run() override;
  private:
    SWState _state; // Текущее состояние переключателя
    uint8_t _was_changed = false; // Флаг того, что состояние сменилось

    // Прочесть состояние переключателя
    SWState _read_state();

    // Отобразить состояние светодиодом
    void _show_state();
};//class