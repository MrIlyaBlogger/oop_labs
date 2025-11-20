namespace oop {

template <typename T>
PmrStack<T>::PmrStack(std::pmr::memory_resource* resource)
    : allocator_(resource) {}

template <typename T>
PmrStack<T>::PmrStack(const PmrStack& other)
    : allocator_(other.allocator_.resource()) {
    copy_from(other);
}

template <typename T>
PmrStack<T>::PmrStack(PmrStack&& other) noexcept
    : allocator_(other.allocator_.resource()) {
    move_from(std::move(other));
}

template <typename T>
PmrStack<T>& PmrStack<T>::operator=(const PmrStack& other) {
    if (this == &other) {
        return *this;
    }

    clear();
    allocator_ = node_allocator(other.allocator_.resource());
    copy_from(other);
    return *this;
}

template <typename T>
PmrStack<T>& PmrStack<T>::operator=(PmrStack&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    clear();
    allocator_ = node_allocator(other.allocator_.resource());
    move_from(std::move(other));
    return *this;
}

template <typename T>
PmrStack<T>::~PmrStack() {
    clear();
}

template <typename T>
void PmrStack<T>::push(const T& value) {
    Node* new_node = allocator_.allocate(1);
    allocator_.construct(new_node, head_, value);
    head_ = new_node;
    ++size_;
}

template <typename T>
void PmrStack<T>::push(T&& value) {
    Node* new_node = allocator_.allocate(1);
    allocator_.construct(new_node, head_, std::move(value));
    head_ = new_node;
    ++size_;
}

template <typename T>
template <typename... Args>
T& PmrStack<T>::emplace(Args&&... args) {
    Node* new_node = allocator_.allocate(1);
    allocator_.construct(new_node, head_, std::forward<Args>(args)...);
    head_ = new_node;
    ++size_;
    return head_->value;
}

template <typename T>
void PmrStack<T>::pop() {
    if (empty()) {
        throw std::out_of_range("PmrStack::pop on empty stack");
    }
    Node* node = head_;
    head_ = head_->next;
    allocator_.destroy(node);
    allocator_.deallocate(node, 1);
    --size_;
}

template <typename T>
T& PmrStack<T>::top() {
    if (empty()) {
        throw std::out_of_range("PmrStack::top on empty stack");
    }
    return head_->value;
}

template <typename T>
const T& PmrStack<T>::top() const {
    if (empty()) {
        throw std::out_of_range("PmrStack::top on empty stack");
    }
    return head_->value;
}

template <typename T>
bool PmrStack<T>::empty() const noexcept {
    return size_ == 0;
}

template <typename T>
typename PmrStack<T>::size_type PmrStack<T>::size() const noexcept {
    return size_;
}

template <typename T>
void PmrStack<T>::clear() {
    while (!empty()) {
        pop();
    }
}

template <typename T>
typename PmrStack<T>::iterator& PmrStack<T>::iterator::operator++() {
    if (current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
typename PmrStack<T>::iterator PmrStack<T>::iterator::operator++(int) {
    iterator tmp(*this);
    ++(*this);
    return tmp;
}

template <typename T>
typename PmrStack<T>::const_iterator& PmrStack<T>::const_iterator::operator++() {
    if (current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
typename PmrStack<T>::const_iterator PmrStack<T>::const_iterator::operator++(int) {
    const_iterator tmp(*this);
    ++(*this);
    return tmp;
}

template <typename T>
void PmrStack<T>::copy_from(const PmrStack& other) {
    if (other.empty()) {
        return;
    }

    std::vector<T> buffer;
    buffer.reserve(other.size_);
    for (const auto& value : other) {
        buffer.push_back(value);
    }

    for (auto it = buffer.rbegin(); it != buffer.rend(); ++it) {
        push(*it);
    }
}

template <typename T>
void PmrStack<T>::move_from(PmrStack&& other) noexcept {
    head_ = other.head_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.size_ = 0;
}

}  // namespace oop

