/* Опрос клавиш */
/*
  Жестко 4x4 на основании pinout.h
  Столбцы поочередно прижимаются к земле.
  Со строк читаются уровни. "0" - кнопка нажата
   [0]  [1]  [2]  [3]
   [4]  [5]  [6]  [7]
   [8]  [9] [10] [11]
  [12] [13] [14] [15]
*/
#pragma once
#include <Arduino.h>
#include <ssExecutor.h>
#include "pinout.h"

#define KP_NONE 0 // Ничего не нажато
#define KP_VAL(x) ((uint16_t)(1 << x)) // Значение, соответствующее кнопке номер x (0..15)
#define KP_NUM(v) (__builtin_ctz(v))  // _Младший_ номер кнопки, соотвествущей переданному значению (для комбинации кнопок отдаст мадшую)
// т.е. для значения 8 (b1000) выдаст =3, для 2 (b0010) =1, а для 10 (b1010) выдаст тоже =1, что соответствует _младшей_ кнопки в комбинации (1)
// https://www.geeksforgeeks.org/builtin-functions-gcc-compiler/?ysclid=mamo6jbcti822329143

class Keypad : public ssExecutor {
  public:
    Keypad();
    // Получить комбинацию клавиш из буфера. KP_NONE - если буфер пуст. После получения эта комбинация удаляется.
    uint16_t getKey(){return _get();};

    // Периодическая работа
    void run() override;
  private:
    uint8_t _cols[4] {COL_1, COL_2, COL_3, COL_4}; // Столбцы
    uint8_t _rows[4] {ROW_A, ROW_B, ROW_C, ROW_D}; // Строки

    uint16_t _buf[16]; // Буфер нажатий FIFO
    uint8_t _buf_tail = 0; // Позиция в буфере последнего элемента

    // Поместить значение в FIFO (при переполнении значение теряется и возвращается false)
    uint8_t _put(const uint16_t val);

    // Получить значение из FIFO (0 - если буфер пуст)
    uint16_t _get();

    // Является ли очередь пустой
    uint8_t _is_empty(){return !_buf_tail;};

};//class