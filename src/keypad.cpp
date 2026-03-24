#include "Arduino.h"
#include "keypad.h"

// Задержка сканирования клавиатуры
#define KP_READ_DELAY_MS 50

#define arraySize(_array) ( sizeof(_array) / sizeof(*(_array)) )


Keypad::Keypad() : ssExecutor(KP_READ_DELAY_MS) {
  // Строки
  for(uint8_t i = 0; i < arraySize(_rows); i++) pinMode(_rows[i], INPUT_PULLUP); 

  // Столбцы
  for(uint8_t i = 0; i < arraySize(_cols); i++){
    pinMode(_cols[i], OUTPUT); 
    digitalWrite(_cols[i], HIGH);
  }//for 
}//Keypad


// Периодическая работа
void Keypad::run(){
  uint16_t new_state = 0;
  static uint16_t old_state = 0; // Предудущее сотояние. Автоповторов нет!
  // Сканируем матрицу
  for(uint8_t col = 0; col < arraySize(_cols); col++){
    digitalWrite(_cols[col], LOW); // Прижали столбец
    for(uint8_t row = 0; row < arraySize(_rows); row++)
      if(!digitalRead(_rows[row])) new_state |= KP_VAL(((arraySize(_rows) * row) + col)); // Нажата. Ставим соответствующий бит в 1
    digitalWrite(_cols[col], HIGH); // Вернули столбец назад
  }//for
  if(old_state == new_state) return; // Ничего не изменилось с прошлого раза
  old_state = new_state;
  if(new_state) _put(new_state); // Если не отпустили все кнопки, то это нажатие и оно попадает в буфер
}//run


// Поместить значение в FIFO (при переполнении значение теряется и возвращается false)
uint8_t Keypad::_put(const uint16_t val){
  if(_buf_tail >= arraySize(_buf)) return false; // Переполнились
  _buf[_buf_tail++] = val;
  return true;
}//_put


// Получить значение из FIFO
uint16_t Keypad::_get(){
  if(_is_empty()) return KP_NONE;
  uint16_t val = _buf[0]; // Взяли значение из начала
  memcpy(_buf, &_buf[1], sizeof(_buf[0]) * _buf_tail--);  //Сдвигаем буфер влево на 1 и сдвигаем хвост
  return val;
}//_get

