#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <vector>
#include <iostream>
#include <algorithm>

class bigint {
    private:
        std::vector<int> vec;
        void    remove_zeros();
    public:
        bigint();
        bigint(unsigned long long num);
        ~bigint();
        bigint(const bigint& cpy);

        bigint& operator=(const bigint& cpy);
        bigint& operator+=(const bigint& cpy);
        bigint operator+(const bigint& cpy) const;

        bigint& operator++();
        bigint operator++(int);

        bigint operator<<(int i) const;
        bigint operator>>(int i) const;

        bigint& operator>>=(int i);
        bigint& operator<<=(int i);

        bigint operator>>(const bigint& cpy) const;
        bigint& operator>>=(const bigint& cpy);

        bigint operator<<(const bigint& cpy) const;
        bigint& operator<<=(const bigint& cpy);

        bool operator<(const bigint& cpy) const;
        bool operator<=(const bigint& cpy) const;
        bool operator>(const bigint& cpy) const;
        bool operator>=(const bigint& cpy) const;
        bool operator!=(const bigint& cpy) const;
        bool operator==(const bigint& cpy) const;

        friend std::ostream& operator<<(std::ostream& os, const bigint& b);


};


#endif