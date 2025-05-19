#include "app.h"
#include <ssMultiPrint.h>
#include <ssStrPrintf.h>
#include <Keyboard.h>

App::App(flashcfg * c, Keypad * k, ModeSW * s, CMD * cmd){
  _cfg = c;
  _keys = k;
  _sw = s;
  _cmd = cmd;
}//App


// Выполнение периодических заданий
void App::run(){
  _processKeys(); // Отратабываем нажатие кнопок
  //_cmd->exec(); // Отрабатываем комады
  

}//run 


void App::_processKeys(){
  _keys->exec(); // Сканируем клавиатуру
  _sw->exec();   // Проверяем состояние переключателя
  uint16_t val = _keys->getKey(); // Получаем очередную нажатую кнопку
  if(val == KP_NONE) return; // Ничего не нажато

  ssMultiPrint(Serial, KEY_CODE_NAME, KP_NUM(val), " = ");
  ssHexPrint(Serial, _cfg->keyCode[KP_NUM(val)]);
  ssMultiPrintln(Serial, " (", _cfg->keyCode[KP_NUM(val)], ")");

  //_sendHTML(_cfg->keyCode[KP_NUM(val)]);
  //_sendGnome(_cfg->keyCode[KP_NUM(val)]);
  _sendWin(_cfg->keyCode[KP_NUM(val)]);
}//_processKeys


// Отправить последовательность для HTML
void App::_sendHTML(const uint16_t code){
  Keyboard.print("&#");
  Keyboard.print(code);
  Keyboard.print(";");
}//_sendHTML


// Отправить последовательность для гнома
void App::_sendGnome(const uint16_t code){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.print("u");
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_CTRL);
  ssHexPrint(Keyboard, code, false);
  Keyboard.write(KEY_RETURN);
}//_sendGnome


// Отправить последовательность для винды
void App::_sendWin(const uint16_t code){
  const uint8_t dig2kp[] = {KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4, KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9};
  char buf[7] = "000000"; // Ведущий 0 + 5 цифр + завершающий ноль
  utoa(code, &buf[1], 10); // Оставляем ведущий 0 для винды
  Serial.println(buf);
  for(uint8_t i = 0; buf[i]; i++){
    Serial.println(dig2kp[buf[i] - '0']); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  // Keyboard.press(KEY_LEFT_ALT);
  //   //Keyboard.write(KEY_KP_0);
  //   Keyboard.write(KEY_KP_1);
  //   Keyboard.write(KEY_KP_6);
  //   Keyboard.write(KEY_KP_9);
  // Keyboard.release(KEY_LEFT_ALT);
}//_sendWin
