

#include "vect2.hpp"

vect2::vect2(): x_(0), y_(0){}
vect2::vect2(int x, int y): x_(x), y_(y){}
vect2::vect2(const vect2& other): x_(other.x_), y_(other.y_){}

int vect2::operator[](int index) const{
	return (index == 0) ? x_ : y_;
}
int& vect2::operator[](int index){
	return (index == 0) ? x_ : y_;
}

vect2& vect2::operator++(){
	++x_;
	++y_;
	return *this;
}
vect2 vect2::operator++(int){
	vect2 temp(*this);
	++(*this);
	return temp;
}
vect2& vect2::operator--(){
	--x_;
	--y_;
	return *this;
}
vect2 vect2::operator--(int){
	vect2 temp(*this);
	--(*this);
	return temp;
}

vect2& vect2::operator=(const vect2& other){
	if (this != &other){
		x_ = other.x_;
		y_ = other.y_;
	}
	return *this;
}
vect2& vect2::operator+=(const vect2& other){
	x_ += other.x_;
	y_ += other.y_;
	return *this;
}
vect2& vect2::operator-=(const vect2& other){
	x_ -= other.x_;
	y_ -= other.y_;
	return *this;
}
vect2& vect2::operator*=(int scalar){
	x_ *= scalar;
	y_ *= scalar;
	return *this;
}

vect2 vect2::operator+(const vect2& other) const{
	return vect2(x_ + other.x_, y_ + other.y_);
}

vect2 vect2::operator-(const vect2& other) const{
	return vect2(x_ - other.x_, y_ - other.y_);
}
vect2 vect2::operator-() const{
	return vect2(-x_, -y_);
}

vect2 vect2::operator*(int scalar) const{
	return vect2(x_ * scalar, y_ * scalar);
}
vect2 operator*(int scalar, const vect2& vec){
	return vect2(vec.x_ * scalar, vec.y_ * scalar);
}

bool vect2::operator==(const vect2& other) const{
	return x_ == other.x_ && y_ == other.y_;
}
bool vect2::operator!=(const vect2& other) const{
	return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const vect2& vec){
	os << "{" << vec[0] << ", " << vec[1] << "}";
	return os;
}
