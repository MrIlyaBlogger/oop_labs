#include <iostream>
#include "include/oop_lr1.h"

int main() {
    std::string input;
    std::cout << "Введите строку скобок: ";
    std::getline(std::cin, input);

    if (isValid(input)) {
        std::cout << "Строка допустима (валидна)\n";
    } else {
        std::cout << "Строка недопустима (невалидна)\n";
    }

    return 0;
}
