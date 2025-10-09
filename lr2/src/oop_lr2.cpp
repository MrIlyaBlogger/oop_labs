#include "oop_lr2.hpp"
#include <algorithm>

Eleven::Eleven() : data(nullptr), size(0) {}

Eleven::Eleven(size_t n, unsigned char t) {
    if (t >= 11) {
        throw std::invalid_argument("Digit must be < 11");
    }
    size = n;
    if (size == 0) { 
        data = nullptr; 
        return;
    }
    data = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = t;
    }
}

Eleven::Eleven(const std::initializer_list<unsigned char>& t) {
    size = t.size();
    if (size == 0) {
        data = nullptr;
        return;
    }
    data = new unsigned char[size];
    size_t i = 0;
    for (auto v : t) {
        if (v >= 11) {
            throw std::invalid_argument("Digit must be < 11");
        }
        data[i++] = v;
    }
}

Eleven::Eleven(const std::vector<unsigned char>& v) {
    size = v.size();
    if (size == 0) { 
        data = nullptr;
        return;
    }
    data = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        if (v[i] >= 11) {
            throw std::invalid_argument("Digit must be < 11");
        }
        data[i] = v[i];
    }
}

Eleven::Eleven(const std::string& t) {
    if (t.empty()) {
        data = nullptr; size = 0;
        return;
    }
    size = t.size();
    data = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        char ch = t[size - 1 - i];
        unsigned char v = 0;
        if (ch >= '0' && ch <= '9') {
            v = ch - '0';
        } else if (ch == 'A' || ch == 'a') {
            v = 10;
        } else {
            throw std::invalid_argument("Invalid eleven digit");
        }
        if (v >= 11) {
            throw std::invalid_argument("Invalid eleven digit");
        }
        data[i] = v;
    }
}

Eleven::Eleven(const Eleven& other) {
    size = other.size;
    if (size == 0) {
        data = nullptr;
        return;
    }
    data = new unsigned char[size];
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

Eleven::Eleven(Eleven&& other) noexcept : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
}

Eleven::~Eleven() noexcept {
    delete[] data;
}

size_t Eleven::length() const noexcept {
    return size;
}

std::string Eleven::toString() const {
    if (size == 0) {
        return "0";
    }
    std::string s;
    s.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        unsigned char d = data[size - 1 - i];
        if (d < 10) {
            s.push_back(static_cast<char>('0' + d));
        } else {
            s.push_back('A');
        }
    }
    return s;
}

Eleven Eleven::add(const Eleven& other) const {
    size_t maxLen = std::max(size, other.size);
    std::vector<unsigned char> result(maxLen + 1, 0);
    unsigned char carry = 0;
    for (size_t i = 0; i < maxLen; ++i) {
        unsigned char a = (i < size) ? data[i] : 0;
        unsigned char b = (i < other.size) ? other.data[i] : 0;
        unsigned char sum = a + b + carry;
        result[i] = sum % 11;
        carry = sum / 11;
    }
    if (carry) {
        result[maxLen] = carry;
    } else {
        result.pop_back();
    }
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    return Eleven(result);
}

Eleven Eleven::sub(const Eleven& other) const {
    if (this->less(other)) {
        throw std::underflow_error("Negative result not supported");
    }
    std::vector<unsigned char> result(size, 0);
    int borrow = 0;
    for (size_t i = 0; i < size; ++i) {
        int a = data[i];
        int b = (i < other.size) ? other.data[i] : 0;
        int diff = a - b - borrow;
        if (diff < 0) {
            diff += 11;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result[i] = static_cast<unsigned char>(diff);
    }
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    return Eleven(result);
}

Eleven Eleven::copy() const {
    return Eleven(*this);
}

bool Eleven::equal(const Eleven& other) const noexcept {
    if (size != other.size) {
        return false;
    }
    for (size_t i = 0; i < size; ++i) {
        if (data[i] != other.data[i]) {
            return false;
        }
    }
    return true;
}

bool Eleven::greater(const Eleven& other) const noexcept {
    if (size != other.size) {
        return size > other.size;
    }
    for (size_t i = size; i > 0; i--) {
        if (data[i] != other.data[i]) return data[i] > other.data[i];
    }
    return false;
}

bool Eleven::less(const Eleven& other) const noexcept {
    if (size != other.size) {
        return size < other.size;
    }
    for (size_t i = size; i > 0; i--) {
        if (data[i] != other.data[i]) {
            return data[i] < other.data[i];
        }
    }
    return false;
}
