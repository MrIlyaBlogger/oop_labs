#include "include/oop_lr3.hpp"
#include <iostream>
#include <memory>
#include <vector>

using namespace oop;

int main() {
    std::vector<Figure*> arr;

    while (true) {
        std::cout << "Команда: a - добавить, p - печать, s - сумма площадей, d - удалить по индексу, q - выход:\n";
        char cmd;
        if (!(std::cin >> cmd)) {
            break;
        }
        if (cmd == 'a') {
            Figure* f = nullptr;
            std::cout << "Введите тип (1-квадрат, 2-прямоугольник, 3-трапеция) и 4 вершины (x y):\n";
            std::cin >> f;
            if (!f) { 
                std::cout << "Неверный тип фигуры\n"; 
                continue; 
            }
            arr.push_back(f);
        } else if (cmd == 'p') {
            for (size_t i = 0; i < arr.size(); ++i) {
                std::cout << i << ": vertices = " << *arr[i] << ", centroid = (" << arr[i]->centroid().x << ", " << arr[i]->centroid().y << ")";
                std::cout << ", area = " << double(*arr[i]) << "\n";
            }
        } else if (cmd == 's') {
            double total = 0.0;
            for (auto f : arr) total += double(*f);
            std::cout << "Суммарная площадь = " << total << "\n";
        } else if (cmd == 'd') {
            size_t idx; std::cin >> idx;
            if (idx < arr.size()) {
                delete arr[idx];
                arr.erase(arr.begin() + idx);
                std::cout << "Удалено\n";
            } else std::cout << "Неверный индекс\n";
        } else if (cmd == 'q') {
            break;
        }
    }

    for (auto f : arr) {
        delete f;
    }
    return 0;
}
