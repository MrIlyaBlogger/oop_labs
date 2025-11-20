#include "include/oop_lr5.hpp"
#include <algorithm>
#include <iostream>
#include <string>

namespace {

void print_stack(const char* title, oop::PmrStack<int>& stack) {
    std::cout << title << ": ";
    for (const auto& value : stack) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}

void demo_int_stack(oop::VectorTrackingMemoryResource& resource) {
    oop::PmrStack<int> int_stack(&resource);
    int_stack.push(10);
    int_stack.push(20);
    int_stack.push(30);
    print_stack("After pushes", int_stack);

    int_stack.pop();
    print_stack("After one pop", int_stack);

    int_stack.emplace(40);
    std::cout << "Top element: " << int_stack.top() << '\n';
    print_stack("Final state", int_stack);
}

void demo_complex_stack(oop::VectorTrackingMemoryResource& resource) {
    oop::PmrStack<oop::DemoComplexType> complex_stack(&resource);

    complex_stack.emplace(oop::DemoComplexType{"alpha", 1, 3.14});
    complex_stack.emplace(oop::DemoComplexType{"beta", 2, 2.71});
    complex_stack.push(oop::DemoComplexType{"gamma", 3, 1.41});

    std::cout << "Complex stack contents:\n";
    for (const auto& element : complex_stack) {
        std::cout << "  name=" << element.name << ", id=" << element.id << ", value=" << element.value << '\n';
    }

    complex_stack.pop();
    std::cout << "After pop, top element: " << complex_stack.top().name << '\n';
}

}  // namespace

int main() {
    oop::VectorTrackingMemoryResource resource;

    demo_int_stack(resource);
    demo_complex_stack(resource);

    std::cout << "Tracked blocks: " << resource.tracked_blocks() << '\n';
    std::cout << "Reusable blocks: " << resource.reusable_blocks() << '\n';
    std::cout << "Active blocks: " << resource.active_blocks() << '\n';

    return 0;
}
