/*
* UniKey
* v 1.0
* (c)04.05.2025 by sersad
* 04.05.2025
*/

/*
* Board: Arduino AVR Boards / Arduino Micro (Leonardo)
*/

#include <ssBuildID.h>
#include <ssMultiPrint.h>
#include "version.h"
#include "flashcfg.h"
#include "pinout.h"
#include "keypad.h"
#include "mode_sw.h"
#include "cmd.h"
#include "app.h"

#include <Keyboard.h>


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

  ssMultiPrintln(Serial, "\n", APP_NAME, APP_VER, APP_COPYRIGHT, APP_SERSAD);
  ssMultiPrintln(Serial, APP_BUILD_ID, BUILD_ID);

  cfg_init(cfg); // Инициализируем конфиг (значения по умолчанию ставятся там же - в flashcfg.cpp
  Serial.println(F("\nConfig loaded."));
#ifdef RESET_FLASH_CFG
  cfg_reset(cfg); //Принудительно сбрасываем значения
  Serial.println(F("\nConfig reseted."));
#endif
  if(Serial.availableForWrite()) cfg_print(Serial, cfg);
  Serial.println();

  cfg.keyCode[0] = 0x221E;
  cfg.keyCode[1] = 0xff1E;

  //Serial.println(KP_NUM(10));
  
/*
#define arraySize(_array) ( sizeof(_array) / sizeof(*(_array)) )
  uint16_t arr [] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  uint8_t _buf_tail = 9; 

  ssArrayPrintln(Serial, arr, arraySize(arr));

  for(uint8_t i = 0; i < 5; i++){
    
    memcpy(arr, &arr[1], sizeof(arr[0]) * _buf_tail--);  //Сдвигаем буфер влево на 1 и сдвигаем хвост
  //memcpy(arr, &arr[1], sizeof(arr) - sizeof(arr[0]));
  //memcpy(arr, &arr[1], sizeof(arr) - sizeof(arr[0]));
    ssMultiPrint(Serial, _buf_tail, " -> "); 
    ssArrayPrintln(Serial, arr, arraySize(arr)); 
  }
*/

/*
  delay(2000); // Короче, под гномом пока толком не работает. Пересекается с самой ардуиной.
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.write("u");
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_ALT); //u   u00b0
  Keyboard.write("00b0\n"); 
  //Keyboard.write(KEY_RETURN);// градус 
*/
}//setup


void loop() {
  app.run();

  //ssMultiPrintln(Serial, "SW_T:", digitalRead(SW_T), " SW_B:", digitalRead(SW_B));
  //digitalWrite(LED_G, digitalRead(SW_T)); // Красный
  //digitalWrite(LED_R, digitalRead(SW_B)); // Зеленый
  // Если средняя позиция, т.е. оба = 1, то будет желтый
  //ssMultiPrintln(Serial, "A:", digitalRead(ROW_A), " B:", digitalRead(ROW_B), " C:", digitalRead(ROW_C), " D:", digitalRead(ROW_D));
/*
  keypad.exec();
  sw.exec();
  static ssExecutor exObj = ssExecutor();
  exObj.exec(1000, [](){
    uint16_t val = keypad.getKey();
    if(val) ssMultiPrintln(Serial, "Key:", KP_NUM(val));
    //ssMultiPrintln(Serial, "SW:", sw.getState());
  });
*/  
  //delay(500);

    // Юникоды: https://symbl.cc
    // Короче, все, что можно через ALT-ы - в винде (проверять по charmap)
    // По линуху:  https://help.ubuntu.com/stable/ubuntu-help/tips-specialchars.html
    // Ctrl+Shift+u <hex-код на основных кнопках и маленьких буквах> <space> или <enter>
    // Иметь ввиду, что смену раскладки надо свалить только на правые Ctrl+Shift (если оставить на любой - то работать не будет)
/*    
    Keyboard.press(KEY_LEFT_ALT); //https://support.microsoft.com/en-us/office/insert-ascii-or-unicode-latin-based-symbols-and-characters-d13f58d3-7bcb-44a7-a4d5-972ee12e50e0
    Keyboard.write(KEY_KP_0);
    Keyboard.write(KEY_KP_1);
    Keyboard.write(KEY_KP_6);
    Keyboard.write(KEY_KP_9);
    Keyboard.release(KEY_LEFT_ALT);
*/    

}//loop
