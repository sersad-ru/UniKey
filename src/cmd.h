/*
* Command interface.
* Работает с UART-ом
*/
#pragma once
#include <Arduino.h>
#include <ssExecutor.h>

//#define CMD_CASE_SENSITIVE // Учитывать регистр при анализе команд
#define CMD_BUF_SIZE 32 // Размер буфера для приема команд с аргументами
#define CMD_MAX_COMMANDS 9 // Максимальное количество обрабатываемых команд

class CMD : public ssExecutor {
  public:
    static const int8_t CMD_NONE  = 0; // Нет команды
    static const int8_t CMD_ERROR = 1; // Команда нераспознана

    CMD(Serial_ &ser);

    // Установить список поддерживаемых команд
    void setCommands(char cmd_list[], const uint8_t list_size);

    // Возвращает последнюю команду cбрасываея ее на CMD_NONE. Так же может вернуть CMD_ERROR и CMD_NONE
    char getCommand(){char cmd = _last_command; _last_command = CMD_NONE; return cmd;};

    // Вернуть ссылку на строку аргументов
    char* getArgs(){return (char*)(_buf + 1);};

    // Периодическая работа
    void run() override {_read_command();};

  private:
    Serial_ _ser;
    uint8_t _buf[CMD_BUF_SIZE]; // Буфер для приема команд
    char _last_command = CMD_NONE; // Последняя полученная команда
    char _cmd_list[CMD_MAX_COMMANDS]; // Массив команд
    uint8_t _cmd_size = 0; // Размер массива команд 

    // Попытаться прочесть команду
    void _read_command();
};//class