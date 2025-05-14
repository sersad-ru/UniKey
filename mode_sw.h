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
    SWState getState(){return _state;};

    // Периодическая работа
    void run() override;
  private:
    SWState _state;
    // Прочесть состояние переключателя
    SWState _read_state();

};//class