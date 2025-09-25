#include <iostream>
#include <string>

bool isValid(const std::string& s) {
    int counter = 0;

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == '(') {
            counter++; //открываем скобку
        } else if (c == ')') {
            counter--; //закрываем скобку
            if (counter < 0) {
                return false; //лишняя закрывающая скобка
            }
        }
    }
    return counter == 0; //конечная проверка
}