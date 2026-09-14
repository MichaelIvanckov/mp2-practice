#include "complex.h"

Complex::Complex() : re(0.0f), im(0.0f) {}
Complex::Complex(float _re, float _im) : re(_re), im(_im) {}
Complex::Complex(float _re) : re(_re), im(0.0f) {}
Complex::Complex(const Complex& c) : Complex(c.re, c.im) {}
Complex::Complex(Complex&& c) : Complex(c) {}
Complex::~Complex() {}

const Complex& Complex::operator=(const Complex& c) {
    re = c.re;
    im = c.im;
    return *this;
}

const Complex& Complex::operator=(Complex&& c) {
    return operator=(c);
}

const Complex& Complex::operator+=(const Complex& c) {
    re += c.re;
    im += c.im;
    return *this;
}

const Complex& Complex::operator-=(const Complex& c) {
    re -= c.re;
    im -= c.im;
    return *this;
}

const Complex& Complex::operator*=(const Complex& c) {
    float new_re = re * c.re - im * c.im;
    float new_im = re * c.im + im * c.re;
    re = new_re;
    im = new_im;
    return *this;
}

const Complex& Complex::operator/=(const Complex& c) {
    float denom = c.re * c.re + c.im * c.im;
    float new_re = (re * c.re + im * c.im) / denom;
    float new_im = (im * c.re - re * c.im) / denom;
    re = new_re;
    im = new_im;
    return *this;
}

const Complex& Complex::operator++() {
    re += 1.0f;
    return *this;
}

const Complex& Complex::operator++(int) {
    static Complex old;
    old = *this;
    ++(*this);
    return old;
}

const Complex& Complex::operator--() {
    re -= 1.0f;
    return *this;
}

const Complex& Complex::operator--(int) {
    static Complex old;
    old = *this;
    --(*this);
    return old;
}

Complex Complex::operator+() const {
    return Complex(*this);
}

Complex Complex::operator-() const {
    return Complex(-re, -im);
}

Complex Complex::operator+(const Complex& c) const {
    return Complex(re + c.re, im + c.im);
}

Complex Complex::operator-(const Complex& c) const {
    return Complex(re - c.re, im - c.im);
}

Complex Complex::operator*(const Complex& c) const {
    return Complex(re * c.re - im * c.im,
                   re * c.im + im * c.re);
}

Complex Complex::operator/(const Complex& c) const {
    float denom = c.re * c.re + c.im * c.im;
    return Complex((re * c.re + im * c.im) / denom,
                   (im * c.re - re * c.im) / denom);
}

bool Complex::operator==(const Complex& c) const {
    return re == c.re && im == c.im;
}

bool Complex::operator!=(const Complex& c) const {
    return !(*this == c);
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.re;
    if (c.im >= 0.0f) {
        os << '+';
    }
    os << c.im << 'i';
    return os;
}

std::istream& operator>>(std::istream& is, Complex& c) {
    is >> std::ws;

    if (is.peek() == '(') {
        char ch;
        is.get(ch); // '('

        if (!(is >> c.re)) return is;

        is >> std::ws;
        if (is.peek() == ',') {
            is.get(ch);
        } else {
            is.setstate(std::ios::failbit);
            return is;
        }

        if (!(is >> c.im)) return is;

        is >> std::ws;
        if (is.peek() == ')') {
            is.get(ch);
        } else {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    if (!(is >> c.re)) return is;

    is >> std::ws;
    int next = is.peek();

    if (next == '+' || next == '-') {
        char sign;
        is >> sign;
        if (!(is >> c.im)) return is;
        if (sign == '-') {
            c.im = -c.im;
        }
        if (is.peek() == 'i' || is.peek() == 'I') {
            is.get();
        }
    } else if (next == 'i' || next == 'I') {
        c.im = c.re;
        c.re = 0.0f;
        is.get();
    } else if (next != std::char_traits<char>::eof()) {
        if (!(is >> c.im)) return is;
    } else {
        c.im = 0.0f;
    }

    return is;
}