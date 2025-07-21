#include "bigint.hpp"
#include <cmath>

void    bigint::remove_zeros() {
    while (vec.size() > 1 && vec.back() == 0) {
        vec.pop_back();
    }
}

bigint::bigint() {
    vec.push_back(0);
}

bigint::bigint(unsigned long long num) {
    if (num == 0)
        vec.push_back(0);
    else {
        while (num > 0) {
            vec.push_back(num%10);
            num = num / 10;
        }
    }
}

bigint::~bigint(){}
bigint::bigint(const bigint& cpy) : vec(cpy.vec) {}

bigint& bigint::operator=(const bigint& cpy) {
    if (this != &cpy) {
        vec = cpy.vec;
    }
    return *this;
}
bigint bigint::operator+(const bigint& cpy) const {
    bigint res;
    res.vec.clear();
    int carry = 0;
    size_t max = std::max(vec.size(), cpy.vec.size());

    for (size_t i = 0; i < max || carry ; ++i) {
        int sum = carry;
        if (i < vec.size()) 
            sum += vec[i];
        if (i < cpy.vec.size())
            sum += cpy.vec[i];
        
        res.vec.push_back(sum % 10);
        carry = sum /10;
    }
    return res;
}

bigint& bigint::operator+=(const bigint& cpy) {
    *this = *this + cpy;
    return *this;
}

bigint& bigint::operator++() {
    *this += bigint(1);
    return *this;
}
bigint bigint::operator++(int) {
    bigint tmp = *this;
    ++(*this);
    return tmp;
}

bigint bigint::operator<<(int i) const {
    if (*this == bigint(0))
        return *this;
    bigint res(*this);
    res.vec.insert(res.vec.begin(), i, 0);
    return res;
}
bigint bigint::operator>>(int i) const {
    bigint res(*this);
    if (i >= vec.size()) {
        res.vec.clear();
        res.vec.push_back(0);
    }
    else {
        res.vec.erase(vec.begin(), vec.begin() + i);
    }
    res.remove_zeros();
    return res;
}

bigint& bigint::operator>>=(int i) {
    if (i >= vec.size()) {
        vec.clear();
        vec.push_back(0);

    }
    else {
        vec.erase(vec.begin(), vec.begin() + i);
    }
    remove_zeros();
    return *this;
}

bigint& bigint::operator<<=(int i) {
    if (*this == bigint(0)) return *this;
    vec.insert(vec.begin(), i, 0);
    return *this;
}

bool bigint::operator<(const bigint& cpy) const {
    if (vec.size() != cpy.vec.size())
        return vec.size() < cpy.vec.size();
    for (size_t i = vec.size(); i-- > 0;) {
        if (vec[i] != cpy.vec[i])
            return vec[i] < cpy.vec[i];
    }
    return false;
}
bool bigint::operator<=(const bigint& cpy) const {
    return !(*this > cpy);
}
bool bigint::operator>(const bigint& cpy) const {
    return cpy < *this;
}
bool bigint::operator>=(const bigint& cpy) const {
    return !(*this < cpy);
}
bool bigint::operator!=(const bigint& cpy) const{
    return vec != cpy.vec;
}
bool bigint::operator==(const bigint& cpy) const {
    return vec == cpy.vec;
}

bigint bigint::operator>>(const bigint& cpy) const {
    unsigned int num;

    for (int i = 0; i < cpy.vec.size(); i++) {
        num = cpy.vec[i] * std::pow(10, i);
    }
    return *this >> num;
}

bigint& bigint::operator>>=(const bigint& cpy) {
    unsigned int num;

    for (int i = 0; i < cpy.vec.size(); i++) {
        num = cpy.vec[i] * std::pow(10, i);
    }
    return *this >>= num;
}

bigint bigint::operator<<(const bigint& cpy) const {
    unsigned int num;

    for (int i = 0; i < cpy.vec.size(); i++) {
        num = cpy.vec[i] * std::pow(10, i);
    }
    return *this << num;
}
bigint& bigint::operator<<=(const bigint& cpy) {
    unsigned int num;

    for (int i = 0; i < cpy.vec.size(); i++) {
        num = cpy.vec[i] * std::pow(10, i);
    }
    return *this <<= num;
}

std::ostream& operator<<(std::ostream& os, const bigint& b) {
    for (size_t i = b.vec.size(); i-- > 0;) { // reverse loop safely
        os << static_cast<char>('0' + b.vec[i]);
    }
    return os;
}