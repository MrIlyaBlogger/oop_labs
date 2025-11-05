#include "include/oop_lr4.hpp"
#include <iostream>

using namespace oop;

int main() {
    Array<double> figures;
    std::unique_ptr<FigureD> fptr;

    std::cout << "Введите фигуры в формате:\n";
    std::cout << "1 — треугольник (3 точки)\n";
    std::cout << "2 — квадрат (4 точки)\n";
    std::cout << "3 — восьмиугольник (8 точек)\n";
    std::cout << "Пример: 1 0 0 3 0 0 4\n";
    std::cout << "Для завершения введите EOF (Ctrl+Z / Ctrl+D)\n\n";

    while (std::cin >> fptr) {
        if (!fptr) {
            std::cerr << "Ошибка при вводе фигуры, пропуск...\n";
            continue;
        }
        figures.add(std::move(fptr));
    }

    std::cout << "\n--- Все фигуры ---\n";
    figures.print(std::cout);

    std::cout << "\nСуммарная площадь: " << figures.total_area() << "\n";
    return 0;
}
