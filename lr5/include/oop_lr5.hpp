#ifndef OOP_LR5_HPP
#define OOP_LR5_HPP

#include <cstddef>
#include <iterator>
#include <memory_resource>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace oop {

class VectorTrackingMemoryResource : public std::pmr::memory_resource {
public:
    VectorTrackingMemoryResource() = default;
    ~VectorTrackingMemoryResource() override;

    VectorTrackingMemoryResource(const VectorTrackingMemoryResource&) = delete;
    VectorTrackingMemoryResource& operator=(const VectorTrackingMemoryResource&) = delete;

    std::size_t tracked_blocks() const noexcept;
    std::size_t reusable_blocks() const noexcept;
    std::size_t active_blocks() const noexcept;

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:
    struct BlockInfo {
        void* ptr{};
        std::size_t size{};
        std::size_t alignment{};
        bool in_use{};
    };

    std::vector<BlockInfo> blocks_{};

    void release_all() noexcept;
};

template <typename T>
class PmrStack {
public:
    using value_type = T;
    using size_type = std::size_t;

    explicit PmrStack(std::pmr::memory_resource* resource = std::pmr::get_default_resource());
    PmrStack(const PmrStack& other);
    PmrStack(PmrStack&& other) noexcept;
    PmrStack& operator=(const PmrStack& other);
    PmrStack& operator=(PmrStack&& other) noexcept;
    ~PmrStack();

    void push(const T& value);
    void push(T&& value);

    template <typename... Args>
    T& emplace(Args&&... args);

    void pop();

    T& top();
    const T& top() const;

    bool empty() const noexcept;
    size_type size() const noexcept;

    void clear();

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator() = default;
        explicit iterator(typename PmrStack::Node* node) : current(node) {}

        reference operator*() const { return current->value; }
        pointer operator->() const { return &current->value; }

        iterator& operator++();
        iterator operator++(int);

        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return lhs.current == rhs.current;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return !(lhs == rhs);
        }

    private:
        typename PmrStack::Node* current = nullptr;
    };

    struct const_iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() = default;
        explicit const_iterator(const typename PmrStack::Node* node) : current(node) {}
        const_iterator(const iterator& other) : current(other.current) {}

        reference operator*() const { return current->value; }
        pointer operator->() const { return &current->value; }

        const_iterator& operator++();
        const_iterator operator++(int);

        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) {
            return lhs.current == rhs.current;
        }

        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) {
            return !(lhs == rhs);
        }

    private:
        const typename PmrStack::Node* current = nullptr;
    };

    iterator begin() noexcept { return iterator(head_); }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator begin() const noexcept { return const_iterator(head_); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }
    const_iterator cbegin() const noexcept { return const_iterator(head_); }
    const_iterator cend() const noexcept { return const_iterator(nullptr); }

private:
    struct Node {
        template <typename... Args>
        explicit Node(Node* next_node, Args&&... args)
            : value(std::forward<Args>(args)...), next(next_node) {}

        T value;
        Node* next;
    };

    using node_allocator = std::pmr::polymorphic_allocator<Node>;

    Node* head_{nullptr};
    size_type size_{0};
    node_allocator allocator_;

    void copy_from(const PmrStack& other);
    void move_from(PmrStack&& other) noexcept;
};

struct DemoComplexType {
    std::string name;
    int id{};
    double value{};

    bool operator==(const DemoComplexType& other) const {
        return name == other.name && id == other.id && value == other.value;
    }
};

}  // namespace oop

#include "oop_lr5.tpp"

#endif
