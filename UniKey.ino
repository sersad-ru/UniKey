/*
* UniKey
* v 1.0
* (c)04.05.2025 by sersad
* 04.05.2025
* 03.06.2025
*/

/*
* Board: Arduino AVR Boards / Arduino Micro (Leonardo) 
*/

#include <ssBuildID.h>
#include <ssMultiPrint.h>
#include "src/version.h"
#include "src/flashcfg.h"
#include "src/pinout.h"
#include "src/keypad.h"
#include "src/mode_sw.h"
#include "src/cmd.h"
#include "src/app.h"

//#define RESET_FLASH_CFG //Сбросить конфигурацию

// Конфиг
flashcfg cfg;
Keypad keypad = Keypad(); // Кнопки
ModeSW sw = ModeSW(); // Переключатель режима
CMD cmd = CMD(Serial); // Командный интерфейс
App app = App(&cfg, &keypad, &sw, &cmd); // Приложение

void setup() {
  Serial.begin(9600);
  // Порт должен очухаться 
#if defined(__AVR_ATmega328P__)  
  delay(50);
#else  
  for(uint8_t i = 0; i < 20; i++){
    delay(100);
    if(Serial.availableForWrite()) Serial.print('*');
    Serial.flush();
  }//for
#endif

  if(Serial.availableForWrite()){
    ssMultiPrintln(Serial, "\n", APP_NAME, APP_VER, APP_COPYRIGHT, APP_SERSAD);
    ssMultiPrintln(Serial, APP_BUILD_ID, BUILD_ID);
  }//if

  cfg_init(cfg); // Инициализируем конфиг (значения по умолчанию ставятся там же - в flashcfg.cpp
  if(Serial.availableForWrite()) Serial.println(F("\nConfig loaded."));
#ifdef RESET_FLASH_CFG
  cfg_reset(cfg); //Принудительно сбрасываем значения
  if(Serial.availableForWrite()) Serial.println(F("\nConfig reseted."));
#endif

  if(Serial.availableForWrite()) ssMultiPrintln(Serial, APP_NAME, F("is ready."));  
}//setup


void loop() {
  app.run();
}//loop
