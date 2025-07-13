#include "bigint.hpp"

std::string bigint::addStrings(const std::string &num1, const std::string &num2) const {
	std::string result;
	int carry = 0;
	int i = num1.size() - 1;
	int j = num2.size() - 1;

	while (i >= 0 || j >= 0 || carry)
	{
		int sum = carry;
		if (i >= 0)
			sum += num1[i--] - '0';
		if (j >= 0)
			sum += num2[j--] - '0';
		result += (sum % 10) + '0';
		carry = sum / 10;
	}
	std::reverse(result.begin(), result.end());
	return result;
}
unsigned long long bigint::stringToULL(const std::string &str) const {
	unsigned long long result = 0;
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		char c = str[i];
		if (!isdigit(c))
			throw std::invalid_argument("Invalid number format");
		result = result * 10 + (c - '0');
	}
	return result;
}
void bigint::removeLeadingZeros() {
	while (value.size() > 1 && value[0] == '0')
		value.erase(0,1);
}


bigint::bigint() : value ("0") {}
bigint::bigint(unsigned long long num) : value(std::to_string(num)) {}
bigint::bigint(const std::string &str) : value(str)
{
	removeLeadingZeros();
}
bigint::bigint(const bigint& other) : value(other.value) {}


bigint& bigint::operator=(const bigint& other)
{
	if (this != &other)
		value = other.value;
	return *this;
}


bigint bigint::operator+(const bigint& other) const {
	return bigint(addStrings(value, other.value));
}
bigint bigint::operator+(unsigned long long num) const {
	return *this + bigint(num);
}
bigint& bigint::operator+=(const bigint& other) {
	value = addStrings(value, other.value);
	return *this;
}
bigint& bigint::operator+=(unsigned long long num) {
	return *this += bigint(num);
}


bigint& bigint::operator++() {
	*this +=1;
	return *this;
}
bigint bigint::operator++(int) {
	bigint temp(*this);
	++(*this);
	return temp;
}


bigint bigint::operator<<(int shift) const {
	if (shift <= 0)
		return *this;
	return bigint(value + std::string(shift, '0'));
}
bigint bigint::operator>>(int shift) const {
	if (shift <= 0 || shift >= static_cast<int>(value.size()))
		return bigint(0);
	return bigint(value.substr(0, value.size() - shift));
}
bigint& bigint::operator<<=(int shift) {
	if (shift > 0)
		value += std::string(shift, '0');
	removeLeadingZeros();
	return *this;
}
bigint& bigint::operator>>=(int shift) {
	if (shift <= 0 || shift >= static_cast<int>(value.size()))
		value = "0";
	else
		value = value.substr(0, value.size() - shift);
	removeLeadingZeros();
	return *this;
}


bigint bigint::operator<<(const bigint& shift) const {
	return *this << stringToULL(shift.value);
}
bigint bigint::operator>>(const bigint& shift) const {
	return *this >> stringToULL(shift.value);
}
bigint& bigint::operator<<=(const bigint& shift) {
	removeLeadingZeros();
	return *this <<= stringToULL(shift.value);
}
bigint& bigint::operator>>=(const bigint& shift) {
	removeLeadingZeros();
	return *this >>= stringToULL(shift.value);
}


bool bigint::operator<(const bigint& other) const {
	if (value.size() != other.value.size())
		return value.size() < other.value.size();
	return value < other.value;
}
bool bigint::operator>(const bigint& other) const {
	return other < *this;
}
bool bigint::operator<=(const bigint& other) const {
	return !(other < *this);
}
bool bigint::operator>=(const bigint& other) const {
	return !(*this < other);
}
bool bigint::operator==(const bigint& other) const {
	return value == other.value;
}
bool bigint::operator!=(const bigint& other) const {
	return value != other.value;
}


std::ostream& operator<<(std::ostream& os, const bigint& bi) {
	os << bi.value;
	return os;
}
