#include <iostream>
#include "include/oop_lr2.hpp"

int main() {
    Eleven a("128");
    Eleven b("94");
    Eleven c = a.add(b);
    Eleven d = a.sub(b);

    std::cout << "a = " << a.toString() << std::endl;
    std::cout << "b = " << b.toString() << std::endl;
    std::cout << "a + b = " << c.toString() << std::endl;
    std::cout << "a - b = " << d.toString() << std::endl;

    Eleven e = a.copy();
    std::cout << "copy of a = " << e.toString() << std::endl;

    std::cout << "a == b: " << (a.equal(b) ? "true" : "false") << std::endl;
    std::cout << "a > b : " << (a.greater(b) ? "true" : "false") << std::endl;
    std::cout << "a < b : " << (a.less(b) ? "true" : "false") << std::endl;

    Eleven x({1,0,10});
    std::cout << "x = " << x.toString() << std::endl;

    return 0;
}
