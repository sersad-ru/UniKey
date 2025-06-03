#include "flashcfg.h"
#include "app.h"
#include "version.h"
#include "mode_sw.h"
#include <ssBuildID.h>
#include <ssMultiPrint.h>
#include <Keyboard.h>


const char ERROR_IN_CMD_VAL[] PROGMEM = "*** Error in command: \"";
#define ERROR_IN_CMD_NAME FF(ERROR_IN_CMD_VAL)
const char CFG_SAVED_VAL[] PROGMEM = "Config saved.";
#define CFG_SAVED_NAME FF(CFG_SAVED_VAL)

const char HLINE_VAL[] PROGMEM = "─";
#define HLINE FF(HLINE_VAL)
const char VLINE_VAL[] PROGMEM = "│";
#define VLINE FF(VLINE_VAL)
const char CELL_LINE_VAL[] PROGMEM  = "─────";
#define CELL_LINE FF(CELL_LINE_VAL)
const char CELL_EMPTY_VAL[] PROGMEM = "     ";
#define CELL_EMPTY FF(CELL_EMPTY_VAL)

const char TOP_LEFT_VAL[] PROGMEM = "┌";
#define TOP_LEFT FF(TOP_LEFT_VAL)
const char TOP_MIDDLE_VAL[] PROGMEM = "┬";
#define TOP_MIDDLE FF(TOP_MIDDLE_VAL)
const char TOP_RIGHT_VAL[] PROGMEM = "┐";
#define TOP_RIGHT FF(TOP_RIGHT_VAL)

const char BOTTOM_LEFT_VAL[] PROGMEM = "└";
#define BOTTOM_LEFT FF(BOTTOM_LEFT_VAL)
const char BOTTOM_MIDDLE_VAL[] PROGMEM = "┴";
#define BOTTOM_MIDDLE FF(BOTTOM_MIDDLE_VAL)
const char BOTTOM_RIGHT_VAL[] PROGMEM = "┘";
#define BOTTOM_RIGHT FF(BOTTOM_RIGHT_VAL)

const char MIDDLE_LEFT_VAL[] PROGMEM = "├";
#define MIDDLE_LEFT FF(MIDDLE_LEFT_VAL)
const char MIDDLE_MIDDLE_VAL[] PROGMEM = "┼";
#define MIDDLE_MIDDLE FF(MIDDLE_MIDDLE_VAL)
const char MIDDLE_RIGHT_VAL[] PROGMEM = "┤";
#define MIDDLE_RIGHT FF(MIDDLE_RIGHT_VAL)



// Список команд
typedef enum {
  CMD_HELP   = 'h', // h - справка по командам
  CMD_INFO   = '?', // ? - информация о билде, название и конфигурация
  CMD_LAYOUT = 'l', // l - раскладка клавиатуры
  CMD_RESET  = '&', // & - сброс к настройкам по умолчанию (единственным аргументом должна быть "F" или "f". т.е. "&F")
  CMD_GREEN  = 'g', // g - переключить тип вывода для зеленого состояния индикатора Win/Gnome
  CMD_KEY    = 'k', // k - кнопка k1=125
  CMD_TEST   = 't', // t - включить / выключить вывод событий в UART (не сохраняется в настройках)
} CMDCommands;

App::App(flashcfg * c, Keypad * k, ModeSW * s, CMD * cmd){
  char cmdArr[] = {CMD_HELP, CMD_INFO, CMD_LAYOUT, CMD_RESET, CMD_GREEN, CMD_KEY, CMD_TEST}; // Настраиваем список команд
  _cfg = c;
  _keys = k;
  _sw = s;
  _cmd = cmd;
  _cmd->setCommands(cmdArr, arraySize(cmdArr));
}//App


// Выполнение периодических заданий
void App::run(){
  _processKeys(); // Отратабываем нажатие кнопок
  _processCmd(); // Отрабатываем комады
}//run 


void App::_processKeys(){
  _keys->exec(); // Сканируем клавиатуру
  _sw->exec();   // Проверяем состояние переключателя
  uint16_t val = _keys->getKey(); // Получаем очередную нажатую кнопку
  if(val == KP_NONE) return; // Ничего не нажато

  // Выбираем метод отправки символа
  switch(_sw->getState()){
    case SW_GREEN: (_cfg->is_green_win) ? _sendWin(KP_NUM(val)) : _sendGnome(KP_NUM(val)); break;
    case SW_YELLOW: (_cfg->is_green_win) ? _sendGnome(KP_NUM(val)) : _sendWin(KP_NUM(val)); break;
    case SW_RED: _sendHTML(KP_NUM(val)); break;
  }//switch
}//_processKeys


// Отправить последовательность для HTML
void App::_sendHTML(const uint8_t key_num){
  if(_test_mode){ // В тестовом режиме только выводим и ничего не посылаем
    _printTest(F("HTML"), key_num);
    return;
  } //if 

  Keyboard.print("&#"); // &#code;
  Keyboard.print(_cfg->keyCode[key_num]);
  Keyboard.print(";");
}//_sendHTML


// Отправить последовательность для гнома
void App::_sendGnome(const uint8_t key_num){
  if(_test_mode){ // В тестовом режиме только выводим и ничего не посылаем
    _printTest(F("Gnome"), key_num);
    return;
  } //if 

  Keyboard.press(KEY_LEFT_CTRL); //<Ctrl>+<Shift>+<u>
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.print("u");
  Keyboard.release(KEY_LEFT_SHIFT);
  Keyboard.release(KEY_LEFT_CTRL);
  ssHexPrint(Keyboard, _cfg->keyCode[key_num], false);
  Keyboard.write(KEY_RETURN);
}//_sendGnome


// Отправить последовательность для винды
void App::_sendWin(const uint8_t key_num){
  if(_test_mode){ // В тестовом режиме только выводим и ничего не посылаем
    _printTest(F("Win"), key_num);
    return;
  } //if 
  //Трансляция цифр в коды цифровой клавиатуры
  const uint8_t dig2kp[] = {KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4, KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9};
  char buf[12] = "0"; // Ведущий 0 + 10 цифр + завершающий ноль
  ultoa(_cfg->keyCode[key_num], &buf[1], 10); // Оставляем ведущий 0 для винды
  Keyboard.press(KEY_LEFT_ALT); //<Alt>+0+<Keypad>
  for(uint8_t i = 0; buf[i]; i++) Keyboard.write(dig2kp[buf[i] - '0']);
  Keyboard.release(KEY_LEFT_ALT);
}//_sendWin


// Вывести тестовую информацию
void App::_printTest(const __FlashStringHelper* mode, uint8_t key_num){
  if(!Serial.availableForWrite()) return; // Некуда писать  
  ssMultiPrint(Serial, F("Mode is: \""), mode, "\". Presed ", KEY_CODE_NAME, key_num, " = ");
  ssHexPrint(Serial, _cfg->keyCode[key_num]);
  ssMultiPrintln(Serial, " (", _cfg->keyCode[key_num], ")");
}//_printTest


// Отработать команды
void App::_processCmd(){
  _cmd->exec(); // Отрабатываем комады
  uint8_t key_num = 0; // Номер кнопки
  keyCodeType key_code = 0; // Код символа (читаем int32, поскольку sscanf не умеет принимать hex и dec для unsigned)
  switch(_cmd->getCommand()){
    case CMD::CMD_NONE: break; // Нет команды

    case CMD::CMD_ERROR: ssMultiPrintln(Serial, ERROR_IN_CMD_NAME, _cmd->getArgs() - 1, "\""); break;

    case CMD_TEST: // Переключение тестового режима
      _test_mode = ! _test_mode;
      ssMultiPrintln(Serial, F("Test mode: "), (_test_mode) ? F("ON") : F("OFF")); 
    break;

    case CMD_GREEN: // Режим для зеленого состояния переключателя. HTML - всегда на красном, а другой на желтый
      _cfg->is_green_win = ! _cfg->is_green_win;
      cfg_print_sw(Serial, *_cfg); // Вывести состояние переключателя
      cfg_save(*_cfg); //сохраняем значения
      Serial.println(CFG_SAVED_NAME);
    break;

    case CMD_KEY: // Присваивание кода кнопке
      // Получаем номер и код кнопки
      if(sscanf(_cmd->getArgs(), "%hhu = %li", &key_num, &key_code) != 2){
        ssMultiPrintln(Serial, ERROR_IN_CMD_NAME, _cmd->getArgs() - 1, "\""); 
        break;
      }//if
      // Проверяем номер кнопки
      if(key_num > arraySize(_cfg->keyCode) - 1){ // Тот ли номер кнопки
        ssMultiPrintln(Serial, F("*** Error: Illegal key number: "), key_num, F(" (not in [1.."), arraySize(_cfg->keyCode) - 1, F("]) in command \""), (char*)(_cmd->getArgs() - 1), "\""); 
        break;
      }//if

      _cfg->keyCode[key_num] = key_code;
      cfg_print_key(Serial, *_cfg, key_num);
      cfg_save(*_cfg); //сохраняем значения
      Serial.println(CFG_SAVED_NAME);
    break;

    case CMD_RESET: // Сброс к настройкам по умолчанию
      if(_cmd->getArgs()[0] != 'f') {
        ssMultiPrintln(Serial, ERROR_IN_CMD_NAME, _cmd->getArgs() - 1, "\""); 
        break;
      }//if 
      cfg_reset(*_cfg);
      Serial.println(F("Reset to defaults."));
    break;

    case CMD_INFO: // Информация о билде, название и конфигурация
      ssMultiPrintln(Serial, "\n", APP_NAME, APP_VER, APP_COPYRIGHT, APP_SERSAD);
      ssMultiPrintln(Serial, APP_BUILD_ID, BUILD_ID, "\n");
      cfg_print(Serial, *_cfg);
      ssMultiPrintln(Serial, F("Test mode: "), (_test_mode) ? F("ON") : F("OFF"));
      Serial.println(F("Use \"h\"\t- for help."));
      Serial.println();
    break;

    case CMD_HELP: // Справка
      Serial.println(F("--- HELP ---"));
      Serial.println(F("h\t- this help"));
      Serial.println(F("?\t- show version, build number and current config"));
      Serial.println(F("l\t- show keyboard layout"));
      Serial.println(F("&F\t- reset to system defaults and save config"));
      Serial.println(F("t\t- turn on / off test mode (only console print and no real action)"));
      Serial.println(F("g\t- swap mode for green light. Win/Gnome"));
      ssMultiPrintln(Serial, F("kN=C\t- key settings: N-key number[0.."), arraySize(_cfg->keyCode) - 1, F("], C - key code. \"k0=169\" - for '©' (copyright) symbol on key 0"));
      Serial.println(F("--- --- ---"));
    break;

    case CMD_LAYOUT:
      Serial.println(F("Keyboard layout: "));
      // Номера
      //"┌────┬────┬────┬────┬────┐"
      ssMultiPrintln(Serial, TOP_LEFT, CELL_LINE, TOP_MIDDLE, CELL_LINE, TOP_MIDDLE, CELL_LINE, TOP_MIDDLE, CELL_LINE, TOP_MIDDLE, CELL_LINE, TOP_RIGHT);
      for(uint8_t i = 0; i < arraySize(_cfg->keyCode); i++){
        Serial.print("│ k");
        if(i < 10) Serial.print('0');
        Serial.print(i);
        Serial.print(' ');
        if(!((i + 1) % 4) && (i < 12)) // "│    │\n├────┼────┼────┼────┤    │"
          ssMultiPrintln(Serial, VLINE, CELL_EMPTY, VLINE, "\n", MIDDLE_LEFT, CELL_LINE, MIDDLE_MIDDLE, CELL_LINE, MIDDLE_MIDDLE, CELL_LINE, MIDDLE_MIDDLE, CELL_LINE, MIDDLE_RIGHT, CELL_EMPTY, VLINE);
      }//for
      //"│    │\n└────┴────┴────┴────┴────┘"
      ssMultiPrintln(Serial, VLINE, CELL_EMPTY, VLINE, "\n", BOTTOM_LEFT, CELL_LINE, BOTTOM_MIDDLE, CELL_LINE, BOTTOM_MIDDLE, CELL_LINE, BOTTOM_MIDDLE, CELL_LINE, BOTTOM_MIDDLE, CELL_LINE, BOTTOM_RIGHT);

      // Знчения
      ssMultiPrintln(Serial, TOP_LEFT, CELL_LINE, TOP_MIDDLE, CELL_LINE, TOP_MIDDLE, CELL_LINE, TOP_MIDDLE, CELL_LINE, TOP_MIDDLE, CELL_LINE, TOP_RIGHT);
      for(uint8_t i = 0; i < arraySize(_cfg->keyCode); i++){
        Serial.print("│ ");
        if(_cfg->keyCode[i] < 0x2700) Serial.print(' '); // Для обычных (не широких) символов
        if(_cfg->keyCode[i] == 0x0301) Serial.print('x'); // Специальный слуяай для знака ударения
        ssUnicodeCharPrint(Serial, _cfg->keyCode[i]);
        Serial.print("  ");
        if(!((i + 1) % 4) && (i < 12)) 
          ssMultiPrintln(Serial, VLINE, CELL_EMPTY, VLINE, "\n", MIDDLE_LEFT, CELL_LINE, MIDDLE_MIDDLE, CELL_LINE, MIDDLE_MIDDLE, CELL_LINE, MIDDLE_MIDDLE, CELL_LINE, MIDDLE_RIGHT, CELL_EMPTY, VLINE);
      }//for
      ssMultiPrintln(Serial, VLINE, CELL_EMPTY, VLINE, "\n", BOTTOM_LEFT, CELL_LINE, BOTTOM_MIDDLE, CELL_LINE, BOTTOM_MIDDLE, CELL_LINE, BOTTOM_MIDDLE, CELL_LINE, BOTTOM_MIDDLE, CELL_LINE, BOTTOM_RIGHT);
    break;

    default: ssMultiPrintln(Serial, F("!!! Can't find event for this command: \""), _cmd->getArgs() - 1, "\""); break; // Необработанная команда
  }//switch
}//_processCmd

