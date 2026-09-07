#include "complex.h"

Complex::Complex() : re(0), im(0) {};
Complex::Complex(float _re, float _im) : re(_re), im(_im) {};
Complex::Complex(float _re) : re(_re), im(0) {};
Complex::Complex(const Complex& c) : Complex(c.re, c.im) {};
Complex::Complex(Complex&& c) : Complex(c) {};

Complex::~Complex() {};

const Complex& Complex::operator=(const Complex& c) {
	re = c.re; im = c.im;
	return *this;
};
const Complex& Complex::operator=(Complex&& c) {
	return operator=(c);
}

const Complex& Complex::operator+=(const Complex& c) {
	re += c.re; im += c.im;
	return *this;
};
const Complex& Complex::operator-=(const Complex& c) {
	re -= c.re; im -= c.im;
	return *this;
};
const Complex& Complex::operator*=(const Complex& c) {
	re *= c.re; im *= c.im;
	return *this;
};
const Complex& Complex::operator/=(const Complex& c) {
	re /= c.re; im /= c.im;
	return *this;
};

const Complex& Complex::operator++();
const Complex& Complex::operator++(int);
const Complex& Complex::operator--();
const Complex& Complex::operator--(int);


Complex Complex::operator+() const { return Complex(*this); };
Complex Complex::operator-() const { return Complex(-re, -im); };

Complex Complex::operator+(const Complex& c) const;
Complex Complex::operator-(const Complex& c) const;
Complex Complex::operator*(const Complex& c) const;
Complex Complex::operator/(const Complex& c) const;

bool Complex::operator==(const Complex& c) const;
bool Complex::operator!=(const Complex& c) const;

std::ostream& Complex::operator<<(std::ostream& os, const Complex& c);
std::istream& Complex::operator>>(std::istream& is, Complex& c);