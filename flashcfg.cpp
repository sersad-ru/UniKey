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
  cfg.keyCode[0]  = KEY_CODE_0_DEFAULT;
  cfg.keyCode[1]  = KEY_CODE_1_DEFAULT;
  cfg.keyCode[2]  = KEY_CODE_2_DEFAULT;
  cfg.keyCode[3]  = KEY_CODE_3_DEFAULT;
  cfg.keyCode[4]  = KEY_CODE_4_DEFAULT;
  cfg.keyCode[5]  = KEY_CODE_5_DEFAULT;
  cfg.keyCode[6]  = KEY_CODE_6_DEFAULT;
  cfg.keyCode[7]  = KEY_CODE_7_DEFAULT;
  cfg.keyCode[8]  = KEY_CODE_8_DEFAULT;
  cfg.keyCode[9]  = KEY_CODE_9_DEFAULT;
  cfg.keyCode[10] = KEY_CODE_10_DEFAULT;
  cfg.keyCode[11] = KEY_CODE_11_DEFAULT;
  cfg.keyCode[12] = KEY_CODE_12_DEFAULT;
  cfg.keyCode[13] = KEY_CODE_13_DEFAULT;
  cfg.keyCode[14] = KEY_CODE_14_DEFAULT;
  cfg.keyCode[15] = KEY_CODE_15_DEFAULT;

  cfg.is_green_win = SW_MODE_DEFAULT;

  cfg.noValue = 0;//Выставляем флаг, что данные установлены
  cfg_save(cfg); //сохраняем значения
}//cfg_reset


void cfg_print(Print &p, flashcfg &cfg){
  uint8_t max_len = 0; // Максимальная длина строки
  // Ищем максимальную длину строки
  for(uint8_t i = 0; i < arraySize(cfg.keyCode); i++){
    uint8_t len = ssGetCharCount(cfg.keyCode[i]);
    max_len = max(max_len, len);
  }//for
  for(uint8_t i = 0; i < arraySize(cfg.keyCode); i++) cfg_print_key(p, cfg, i, max_len);
  cfg_print_sw(p, cfg);
}//cfg_print


// Вывести параметры кнопки
void cfg_print_key(Print &p, flashcfg &cfg, const uint8_t key_num, const uint8_t str_size){
  ssMultiPrint(p, KEY_CODE_NAME, key_num, ": ");

  // Выравниваем вправо, если надо
  if(str_size){
    if(key_num < 10) p.print(' ');
    ssAlignPrint(Serial, cfg.keyCode[key_num], str_size, ALIGN_RIGHT, '0');
  } else p.print(cfg.keyCode[key_num]);  //if
  
  p.print(F(" ("));
  ssHexPrint(p, cfg.keyCode[key_num]);
  p.print(F(") \""));   
  if(cfg.keyCode[key_num] == 0x0301) p.print('x'); // Специальный слуяай для знака ударения
  ssUnicodeCharPrint(p, cfg.keyCode[key_num]);
  p.println(F("\" "));
}//cfg_print_key


 // Вывести параметры переключателя
void cfg_print_sw(Print &p, flashcfg &cfg){
  ssMultiPrintln(p, SW_MODE_NAME, (cfg.is_green_win) ? SW_MODE_WIN_NAME : SW_MODE_GNOME_NAME);
}//cfg_print_sw
