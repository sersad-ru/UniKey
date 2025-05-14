#include "mode_sw.h"

#include <ssMultiPrint.h>

// Задержка опроса переключателя
#define SW_READ_DELAY_MS 500


ModeSW::ModeSW() : ssExecutor(SW_READ_DELAY_MS) {
  // Настраиваем светодиод
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);

  // Настраиваем переключатель
  pinMode(SW_T, INPUT_PULLUP);
  pinMode(SW_B, INPUT_PULLUP);

  // Получаем начальное положение
  _state = _read_state();
}//ModeSW


// Периодическая работа
void ModeSW::run(){
  _state = _read_state();
}//run


// Прочесть состояние переключателя
SWState ModeSW::_read_state(){
  return digitalRead(SW_T) | (digitalRead(SW_B) << 1);
}//_read_state
