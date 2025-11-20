#include "include/oop_lr5.hpp"
#include <iostream>
#include <string>

using namespace oop;

int main() {
    VectorHeapMemoryResource memory;
    pmr_stack<std::string> st(&memory);

    st.push("first");
    st.push("second");
    st.emplace("third");

    std::cout << "Stack size: " << st.size() << "\n";
    std::cout << "Top: " << st.top() << "\n";

    std::cout << "Iterating:\n";
    for (auto& v : st)
        std::cout << "  " << v << "\n";

    return 0;
}
