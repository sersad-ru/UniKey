/* Переключатель режимов */

/*
  На основе pinout.h
*/

#pragma once
#include <Arduino.h>
#include "pinout.h"

// Режимы (соотвествуют цвету светодиода и положению переключателя)
typedef enum {
  SW_GREEN,   // Цвет зеленый, переключатель вверх (к USB-разъему)
  SW_YELLOW,  // Цвет желтый, переключатель посредине
  SW_RED      // Цвет красный, переключатель вниз (от USB-разъема)
} SWState;

class ModeSW {
  public:
    ModeSW();
    SWState getState(){return _state;};
  private:
    SWState _state;
};//class