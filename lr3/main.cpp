#include "include/oop_lr3.hpp"
#include <iostream>
#include <memory>
#include <vector>

using namespace oop;

int main() {
    std::cout << "Введите фигуры по типу: 1 - квадрат, 2 - прямоугольник, 3 - трапеция\n";
    std::vector<Figure*> arr;

    while (true) {
        std::cout << "Команда: a - добавить, p - печать, s - сумма площадей, d - удалить по индексу, q - выход: ";
        char cmd;
        if (!(std::cin >> cmd)) break;
        if (cmd == 'a') {
            int t; std::cin >> t;
            Figure *f = nullptr;
            if (t == 1) f = new Square();
            else if (t == 2) f = new Rectangle();
            else if (t == 3) f = new Trapezoid();
            else { std::cout << "Неверный тип\n"; continue; }
            std::cout << "Введите 4 вершины (x y) по порядку:\n";
            f->read(std::cin);
            arr.push_back(f);
        } else if (cmd == 'p') {
            for (size_t i = 0; i < arr.size(); ++i) {
                std::cout << i << ": vertices = " << *arr[i] << ", centroid = (" << arr[i]->centroid().x << ", " << arr[i]->centroid().y << ")";
                std::cout << ", area = " << double(*arr[i]) << "\n";
            }
        } else if (cmd == 's') {
            double total = 0.0;
            for (auto f : arr) total += double(*f);
            std::cout << "Total area = " << total << "\n";
        } else if (cmd == 'd') {
            size_t idx; std::cin >> idx;
            if (idx < arr.size()) {
                delete arr[idx];
                arr.erase(arr.begin() + idx);
                std::cout << "Deleted\n";
            } else std::cout << "Bad index\n";
        } else if (cmd == 'q') break;
    }

    for (auto f : arr) delete f;
    return 0;
}
