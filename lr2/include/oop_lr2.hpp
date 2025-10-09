#pragma once
#include <cstddef>
#include <stdexcept>
#include <string>
#include <initializer_list>
#include <vector>

class Eleven {
private:
    unsigned char* data;
    size_t size;

public:
    Eleven();
    Eleven(size_t n, unsigned char t = 0);
    Eleven(const std::initializer_list<unsigned char>& t);
    Eleven(const std::vector<unsigned char>& v);
    Eleven(const std::string& t);
    Eleven(const Eleven& other);
    Eleven(Eleven&& other) noexcept;
    ~Eleven() noexcept;

    size_t length() const noexcept;
    std::string toString() const;

    Eleven add(const Eleven& other) const;
    Eleven sub(const Eleven& other) const;
    Eleven copy() const;

    bool equal(const Eleven& other) const noexcept;
    bool greater(const Eleven& other) const noexcept;
    bool less(const Eleven& other) const noexcept;
};
