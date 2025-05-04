#include <EEPROM.h>
#include <ssMultiPrint.h>
#include "flashcfg.h"

bool cfg_init(flashcfg &cfg) {
  return cfg_load(cfg); //Читаем
}//cfg_init


bool cfg_load(flashcfg &cfg) {
  EEPROM.get(0, cfg); //Читаем
  if(!cfg.noValue) return true; //Значение в памяти были
  cfg_reset(cfg); //Ставим по умолчанию
  return false; //не было значений в памяти
}//cfg_load


void cfg_save(flashcfg &cfg) {
  EEPROM.put(0, cfg); //Записываем значения
}//cfg_save


void cfg_reset(flashcfg &cfg) {
  //Ставим значения по умолчанию

  cfg.noValue = 0;//Выставляем флаг, что данные установлены
  cfg_save(cfg); //сохраняем значения
}//cfg_reset


void cfg_print(Print &p, flashcfg &cfg){
  
}//cfg_print
