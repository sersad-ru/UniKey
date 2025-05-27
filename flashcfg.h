/*
 * Хранение в EEPROM
*/
#pragma once
#include <Arduino.h>

#define FF(val) ((const __FlashStringHelper*)val)
#define arraySize(_array) ( sizeof(_array) / sizeof(*(_array)) )

// Параметры по-умолчанию
// Код кнопки
const char KEY_CODE_VAL[] PROGMEM = "Key_";
#define KEY_CODE_NAME FF(KEY_CODE_VAL)

#define KEY_CODE_0_DEFAULT  0x0030 // "0" - Кнопка 0 (слева направо, сверху вниз) 
#define KEY_CODE_1_DEFAULT  0x0031 // "1"
#define KEY_CODE_2_DEFAULT  0x0032 // "2"
#define KEY_CODE_3_DEFAULT  0x0033 // "3"
#define KEY_CODE_4_DEFAULT  0x0034 // "4"
#define KEY_CODE_5_DEFAULT  0x0035 // "5"
#define KEY_CODE_6_DEFAULT  0x0036 // "6"
#define KEY_CODE_7_DEFAULT  0x0037 // "7"
#define KEY_CODE_8_DEFAULT  0x0038 // "8"
#define KEY_CODE_9_DEFAULT  0x0039 // "9"
#define KEY_CODE_10_DEFAULT 0x0041 // "A"
#define KEY_CODE_11_DEFAULT 0x0042 // "B"
#define KEY_CODE_12_DEFAULT 0x0043 // "C"
#define KEY_CODE_13_DEFAULT 0x0044 // "D"
#define KEY_CODE_14_DEFAULT 0x0045 // "E"
#define KEY_CODE_15_DEFAULT 0x0046 // "F"

// Режим переключателя 
const char SW_MODE_VAL[] PROGMEM = "Green light for ";
#define SW_MODE_NAME FF(SW_MODE_VAL)
const char SW_MODE_WIN_VAL[] PROGMEM = "WIN";
#define SW_MODE_WIN_NAME FF(SW_MODE_WIN_VAL)
const char SW_MODE_GNOME_VAL[] PROGMEM = "GNOME";
#define SW_MODE_GNOME_NAME FF(SW_MODE_GNOME_VAL)

#define SW_MODE_DEFAULT true // По-умолчанию зеленый режим - это windows

// Параметры конфигурации
typedef int32_t keyCodeType;

typedef struct {
  uint8_t  noValue; // Флаг того (==1), что не было сохраненных значений 
  
  keyCodeType keyCode[16]; // Коды выводимых символов [0..15] (unicode 2 байта)
  uint8_t is_green_win; // Флаг указывающий использовать win-отправку, когда переключатель зеленый (red - всегда html, а yellow - что осталось)
} flashcfg;

bool cfg_init(flashcfg &cfg); //Подготовиться к работе с конфигом и загрузить его, а если надо и прописать по умолчанию (вернет false - если по умолчанию)
bool cfg_load(flashcfg &cfg); //Загрузить конфиг из флеша (false - если по умолчанию)
void cfg_save(flashcfg &cfg); //Сохранить конфиг во флеш
void cfg_reset(flashcfg &cfg); //Сбросить конфиг на значения по умолчанию
void cfg_print(Print &p, flashcfg &cfg); // Вывести настройки

void cfg_print_key(Print &p, flashcfg &cfg, const uint8_t key_num, const uint8_t str_size = 0); // Вывести параметры кнопки (если str_size !=0, то выравниваем вправо нулями до str_size)
void cfg_print_sw(Print &p, flashcfg &cfg); // Вывести параметры переключателя
