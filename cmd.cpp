#include "cmd.h"


// Задержка опроса командного интерфейса
#define CMD_READ_DELAY_MS 250

CMD::CMD(Serial_ &ser) : ssExecutor(CMD_READ_DELAY_MS){
  _ser = ser;
}// CMD


// Попытаться прочесть команду
void CMD::_read_command(){
  // Читаем (копим) строку
  static uint8_t i = 0;
  while(_ser.available()){
    _buf[i++] = _ser.read();
    if((_buf[i - 1] == 0x0D) || (_buf[i - 1] == 0x0A) || (i > (CMD_BUF_SIZE - 1))){
      _buf[i - 1] = 0; // ставим 0 вместо \r или \n или последним символом буфера
      if(i < 2) i = 0; // Если сразу прилетело окончание строки, то начинать сначала
      while(_ser.available())_ser.read(); // подчищаем буфер
      break;
    }//if
  }//while
  
  // К этому моменту i указывает на позицию, куда попадет СЛЕДУЮЩИЙ символ. На следующей итерации!
  if((i < 2) || (_buf[i - 1] != 0)) return; // Минимальный индекс  = 2 ([0]: символ, [1]: \0). Последний записанный (i - 1) символ должен быть \0

  // Считаем, что строку собрали. В следующий раз будем начинать с начала
  i = 0;

  // Анализируем, что начитали
  // !!!!!!!!!!!!!!!!!!! TODO: CaseSentensitive!!!!!!!!!!!!!!!!!
  char cmd = tolower((char)_buf[0]); // Команда

  // Проверяем, есть ли команда в списке
  _last_command = CMD_ERROR; // Считаем, что нет
  for(uint8_t j = 0; j < _cmd_size; j++) 
    if(cmd == _cmd_list[j]){
      _last_command = cmd;
      break;
    }//if
}//_read_command
