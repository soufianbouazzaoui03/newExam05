#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>

class bigint {
private:
	std::string value;
	std::string addStrings(const std::string &num1, const std::string &num2) const;
	unsigned long long stringToULL(const std::string &str) const;
	void removeLeadingZeros();
public:
	bigint();
	bigint(unsigned long long num);
	bigint(const std::string &str);
	bigint(const bigint& other);

	bigint& operator=(const bigint& other);

	bigint operator+(const bigint& other) const;
	bigint operator+(unsigned long long num) const;
	bigint& operator+=(const bigint& other);
	bigint& operator+=(unsigned long long num);

	bigint& operator++();
	bigint operator++(int);

	bigint operator<<(int shift) const;
	bigint operator>>(int shift) const;
	bigint& operator<<=(int shift);
	bigint& operator>>=(int shift);

	bigint operator<<(const bigint& shift) const;
	bigint operator>>(const bigint& shift) const;
	bigint& operator<<=(const bigint& shift);
	bigint& operator>>=(const bigint& shift);

	bool operator<(const bigint& shift) const;
	bool operator>(const bigint& shift) const;
	bool operator<=(const bigint& shift) const;
	bool operator>=(const bigint& shift) const;
	bool operator==(const bigint& shift) const;
	bool operator!=(const bigint& shift) const;

	friend std::ostream& operator<<(std::ostream& os, const bigint& bi);
};

#endif
