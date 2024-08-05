#ifndef Console_H_
#define Console_H_
#include <windows.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sstream>
void OpenConsole();
void ConsolePrint(std::string str);
void ConsolePrint(WCHAR* str);
#endif