#ifndef OOP_LR5_HPP
#define OOP_LR5_HPP

#include <memory_resource>
#include <vector>
#include <mutex>
#include <new>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cassert>

namespace oop {

class VectorHeapMemoryResource : public std::pmr::memory_resource {
public:
    struct BlockInfo {
        void* ptr;
        std::size_t size;
        std::size_t alignment;
        bool free;
    };

    VectorHeapMemoryResource() = default;

    ~VectorHeapMemoryResource() override {
        std::lock_guard<std::mutex> lg(mtx_);
        for (auto& b : blocks_) {
            if (b.ptr) {
                if (b.alignment > alignof(std::max_align_t))
                    ::operator delete(b.ptr, std::align_val_t(b.alignment));
                else
                    ::operator delete(b.ptr);
                b.ptr = nullptr;
            }
        }
        blocks_.clear();
    }

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        std::lock_guard<std::mutex> lg(mtx_);
        for (auto& b : blocks_) {
            if (b.free && b.size >= bytes && b.alignment == alignment) {
                b.free = false;
                return b.ptr;
            }
        }

        void* p = nullptr;
        if (alignment > alignof(std::max_align_t))
            p = ::operator new(bytes, std::align_val_t(alignment));
        else
            p = ::operator new(bytes);

        blocks_.push_back(BlockInfo{p, bytes, alignment, false});
        return p;
    }

    void do_deallocate(void* p, std::size_t, std::size_t) override {
        std::lock_guard<std::mutex> lg(mtx_);
        for (auto& b : blocks_) {
            if (b.ptr == p) {
                b.free = true;
                return;
            }
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }

private:
    std::vector<BlockInfo> blocks_;
    mutable std::mutex mtx_;
};

template <typename T>
class pmr_stack {
public:
    struct Node {
        T value;
        Node* next;
    };

    using allocator_type = std::pmr::polymorphic_allocator<Node>;

    pmr_stack(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : alloc_(mr), head_(nullptr), sz_(0) {}

    pmr_stack(const pmr_stack&) = delete;
    pmr_stack& operator=(const pmr_stack&) = delete;

    pmr_stack(pmr_stack&& other) noexcept
        : alloc_(other.alloc_), head_(other.head_), sz_(other.sz_) {
        other.head_ = nullptr;
        other.sz_ = 0;
    }

    pmr_stack& operator=(pmr_stack&& other) noexcept {
        if (this != &other) {
            while (!empty()) pop();
            alloc_ = other.alloc_;
            head_ = other.head_;
            sz_ = other.sz_;
            other.head_ = nullptr;
            other.sz_ = 0;
        }
        return *this;
    }

    ~pmr_stack() {
        while (!empty()) pop();
    }

    void push(const T& value) {
        Node* node = alloc_.allocate(1);
        alloc_.construct(std::addressof(node->value), value);
        node->next = head_;
        head_ = node;
        ++sz_;
    }

    void push(T&& value) {
        Node* node = alloc_.allocate(1);
        alloc_.construct(std::addressof(node->value), std::move(value));
        node->next = head_;
        head_ = node;
        ++sz_;
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        Node* node = alloc_.allocate(1);
        alloc_.construct(std::addressof(node->value), std::forward<Args>(args)...);
        node->next = head_;
        head_ = node;
        ++sz_;
    }

    void pop() {
        assert(head_);
        Node* tmp = head_;
        head_ = head_->next;
        alloc_.destroy(std::addressof(tmp->value));
        alloc_.deallocate(tmp, 1);
        --sz_;
    }

    T& top() {
        assert(head_);
        return head_->value;
    }

    const T& top() const {
        assert(head_);
        return head_->value;
    }

    bool empty() const noexcept {
        return head_ == nullptr;
    }

    std::size_t size() const noexcept {
        return sz_;
    }

    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator() : cur_(nullptr) {}
        explicit iterator(Node* ptr) : cur_(ptr) {}

        reference operator*() const { return cur_->value; }
        pointer operator->() const { return std::addressof(cur_->value); }

        iterator& operator++() {
            cur_ = cur_->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            cur_ = cur_->next;
            return tmp;
        }

        bool operator==(const iterator& o) const { return cur_ == o.cur_; }
        bool operator!=(const iterator& o) const { return cur_ != o.cur_; }

        Node* cur_;
    };

    iterator begin() noexcept { return iterator(head_); }
    iterator end() noexcept { return iterator(nullptr); }

private:
    allocator_type alloc_;
    Node* head_;
    std::size_t sz_;
};

}

#endif
