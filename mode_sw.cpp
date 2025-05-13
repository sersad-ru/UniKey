#include "mode_sw.h"

ModeSW::ModeSW(){
  // Настраиваем светодиод
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);

  // Настраиваем переключатель
  pinMode(SW_T, INPUT_PULLUP);
  pinMode(SW_B, INPUT_PULLUP);

  // Получаем начальное положение !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
}//ModeSW

