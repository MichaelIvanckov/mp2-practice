#pragma once
#include <iostream>

struct Complex {
	
	float re;
	float im;

	Complex();
	Complex(float, float);
	Complex(float);

	Complex(const Complex&);
	Complex(Complex&&);

	~Complex();

	const Complex& operator=(const Complex&);
	const Complex& operator=(Complex&&);

	const Complex& operator+=(const Complex&);
	const Complex& operator-=(const Complex&);
	const Complex& operator*=(const Complex&);
	const Complex& operator/=(const Complex&);

	Complex operator+(const Complex&) const;
	Complex operator-(const Complex&) const;
	Complex operator*(const Complex&) const;
	Complex operator/(const Complex&) const;

	bool operator==(const Complex&) const;
	bool operator!=(const Complex&) const;

	friend std::ostream& operator<<(std::ostream&, const Complex&);
	friend std::istream& operator>>(std::istream&, Complex&);
};