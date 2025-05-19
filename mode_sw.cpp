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

  _state = _read_state();// Получаем начальное положение
  _show_state();// Отображаем его
}//ModeSW


// Периодическая работа
void ModeSW::run(){
  SWState new_state = _read_state();
  if(new_state == _state) return; // Состояние не изменилось

  _was_changed = true; // У нас новое состояние
  _state = new_state; // Сохранили новое состояние
  _show_state();// Меняем состояние светодиода
}//run


// Прочесть состояние переключателя
SWState ModeSW::_read_state(){
  return digitalRead(SW_T) | (digitalRead(SW_B) << 1);
}//_read_state


// Отобразить состояние светодиодом
void ModeSW::_show_state(){
  digitalWrite(LED_G, _state & 0x01); // Младший бит - зеленый
  digitalWrite(LED_R, _state & 0x02); // Старший бит - зкрасный
}
