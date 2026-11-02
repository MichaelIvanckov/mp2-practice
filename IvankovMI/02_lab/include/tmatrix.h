// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <utility>
#include <stdexcept>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
    size_t sz;
    T* pMem;
public:
    TDynamicVector(size_t size = 1);
    TDynamicVector(T* arr, size_t s);
    TDynamicVector(const TDynamicVector& v);
    TDynamicVector(TDynamicVector&& v) noexcept;
    ~TDynamicVector();

    TDynamicVector& operator=(const TDynamicVector& v);
    TDynamicVector& operator=(TDynamicVector&& v) noexcept;

    size_t size() const noexcept { return sz; }

    // индексация
    T& operator[](size_t ind);
    const T& operator[](size_t ind) const;

    // индексация с контролем
    T& at(size_t ind);
    const T& at(size_t ind) const;

    // сравнение
    bool operator==(const TDynamicVector& v) const noexcept;
    bool operator!=(const TDynamicVector& v) const noexcept;

    // скалярные операции
    TDynamicVector operator+(T val);
    TDynamicVector operator-(T val);
    TDynamicVector operator*(T val);

    // векторные операции
    TDynamicVector operator+(const TDynamicVector& v);
    TDynamicVector operator-(const TDynamicVector& v);
    T operator*(const TDynamicVector& v);      // удалено noexcept т. к. был тест с именем «cant_multiply_vectors_with_not_equal_size»

    friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
    {
        std::swap(lhs.sz, rhs.sz);
        std::swap(lhs.pMem, rhs.pMem);
    }

    // ввод/вывод
    friend istream& operator>>(istream& istr, TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; i++)
            istr >> v.pMem[i]; // требуется оператор>> для типа T
        return istr;
    }
    friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
    {
        for (size_t i = 0; i < v.sz; i++)
            ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
        return ostr;
    }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
// Хранит только верхний треугольник: строка i имеет длину sz - i
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
    using TDynamicVector<TDynamicVector<T>>::pMem;
    using TDynamicVector<TDynamicVector<T>>::sz;
private:
    static size_t checkSize(size_t s);

    // Прокси-строка для доступа к элементам верхнего треугольника с сохранением синтаксиса m[i][j]
    class RowProxy {
    public:
        TDynamicVector<T>& row;
        size_t offset;
        size_t n;
        RowProxy(TDynamicVector<T>& r, size_t off, size_t size) : row(r), offset(off), n(size) {}

        size_t size() const { return n; }

        T& operator[](size_t j) { return row[j - offset]; }
        const T& operator[](size_t j) const { return row[j - offset]; }

        T& at(size_t j) {
            if (j < offset) throw std::out_of_range("Index below main diagonal");
            if (j >= n) throw std::out_of_range("Index out of range");
            return row.at(j - offset);
        }
        const T& at(size_t j) const {
            if (j < offset) { static const T def = T(); return def; }
            if (j >= n) throw std::out_of_range("Index out of range");
            return row.at(j - offset);
        }

        // Конверсия в TDynamicVector<T>: создаёт полную логическую строку
        // длины n, где элементы ниже главной диагонали равны T().
        operator TDynamicVector<T>() const {
            TDynamicVector<T> result(n);
            for (size_t j = 0; j < n; ++j)
                result[j] = at(j);
            return result;
        }

        // Доступ к исходному физическому вектору-строке (длины n - offset)
        TDynamicVector<T>& getVector() noexcept { return row; }
        const TDynamicVector<T>& getVector() const noexcept { return row; }
    };

    class ConstRowProxy {
    public:
        const TDynamicVector<T>& row;
        size_t offset;
        size_t n;
        ConstRowProxy(const TDynamicVector<T>& r, size_t off, size_t size) : row(r), offset(off), n(size) {}

        size_t size() const { return n; }

        const T& operator[](size_t j) const { return row[j - offset]; }

        const T& at(size_t j) const {
            if (j < offset) { static const T def = T(); return def; }
            if (j >= n) throw std::out_of_range("Index out of range");
            return row.at(j - offset);
        }

        // Конверсия в TDynamicVector<T>: создаёт полную логическую строку
        // длины n, где элементы ниже главной диагонали равны T().
        operator TDynamicVector<T>() const {
            TDynamicVector<T> result(n);
            for (size_t j = 0; j < n; ++j)
                result[j] = at(j);
            return result;
        }

        // Доступ к исходному физическому вектору-строке (длины n - offset)
        const TDynamicVector<T>& getVector() const noexcept { return row; }
    };

public:
    TDynamicMatrix(size_t s = 1);

    using TDynamicVector<TDynamicVector<T>>::size;

    // Первый индекс: без проверок, UB при i >= sz (стандартная семантика operator[])
    RowProxy operator[](size_t i) {
        return RowProxy(pMem[i], i, sz);
    }
    ConstRowProxy operator[](size_t i) const {
        return ConstRowProxy(pMem[i], i, sz);
    }

    // Первый индекс: с проверкой границы, возвращает прокси строки
    RowProxy at(size_t i) {
        if (i >= sz) throw std::out_of_range("Index out of range");
        return RowProxy(pMem[i], i, sz);
    }
    ConstRowProxy at(size_t i) const {
        if (i >= sz) throw std::out_of_range("Index out of range");
        return ConstRowProxy(pMem[i], i, sz);
    }

    // доступ к элементу с контролем по двум индексам
    T& at(size_t i, size_t j) {
        return at(i).at(j);
    }
    const T& at(size_t i, size_t j) const {
        return at(i).at(j);
    }

    // сравнение
    bool operator==(const TDynamicMatrix& m) const noexcept;

    // матрично-скалярные операции
    TDynamicMatrix operator*(const T& val);

    // матрично-векторные операции
    TDynamicVector<T> operator*(const TDynamicVector<T>& v);

    // матрично-матричные операции
    TDynamicMatrix operator+(const TDynamicMatrix& m);
    TDynamicMatrix operator-(const TDynamicMatrix& m);
    TDynamicMatrix operator*(const TDynamicMatrix& m);

    // ввод/вывод
    friend istream& operator>>(istream& istr, TDynamicMatrix& v)
    {
        for (size_t i = 0; i < v.sz; ++i)
            for (size_t j = 0; j < v.sz; ++j) {
                T val;
                istr >> val;
                if (j >= i)
                    v[i][j] = val;
                // значения ниже диагонали игнорируются
            }
        return istr;
    }
    friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
    {
        for (size_t i = 0; i < v.sz; ++i) {
            for (size_t j = 0; j < v.sz; ++j)
                ostr << v.at(i, j) << ' ';
            ostr << '\n';
        }
        return ostr;
    }
};


// ---------- Реализация: -------------

// ---------- TDynamicVector ----------

template<typename T>
TDynamicVector<T>::TDynamicVector(size_t size) : sz(size)
{
    if (sz == 0 || sz > MAX_VECTOR_SIZE)
        throw std::out_of_range("Vector size should be in (0, MAX_VECTOR_SIZE]");
    pMem = new T[sz]();
}

template<typename T>
TDynamicVector<T>::TDynamicVector(T* arr, size_t s) : sz(s)
{
    if (arr == nullptr)
        throw std::invalid_argument("TDynamicVector ctor requires non-null arg");
    if (sz == 0 || sz > MAX_VECTOR_SIZE)
        throw std::out_of_range("Vector size should be in (0, MAX_VECTOR_SIZE]");
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
}

template<typename T>
TDynamicVector<T>::TDynamicVector(const TDynamicVector& v) : sz(v.sz)
{
    pMem = new T[sz];
    std::copy(v.pMem, v.pMem + sz, pMem);
}

template<typename T>
TDynamicVector<T>::TDynamicVector(TDynamicVector&& v) noexcept
    : sz(v.sz), pMem(v.pMem)
{
    v.sz = 0;
    v.pMem = nullptr;
}

template<typename T>
TDynamicVector<T>::~TDynamicVector()
{
    delete[] pMem;
}

template<typename T>
TDynamicVector<T>& TDynamicVector<T>::operator=(const TDynamicVector& v)
{
    if (this != &v)
    {
        TDynamicVector tmp(v);
        swap(*this, tmp);
    }
    return *this;
}

template<typename T>
TDynamicVector<T>& TDynamicVector<T>::operator=(TDynamicVector&& v) noexcept
{
    if (this != &v)
    {
        delete[] pMem;
        sz = v.sz;
        pMem = v.pMem;
        v.sz = 0;
        v.pMem = nullptr;
    }
    return *this;
}

template<typename T>
T& TDynamicVector<T>::operator[](size_t ind)
{
    return pMem[ind];
}

template<typename T>
const T& TDynamicVector<T>::operator[](size_t ind) const
{
    return pMem[ind];
}

template<typename T>
T& TDynamicVector<T>::at(size_t ind)
{
    if (ind >= sz)
        throw std::out_of_range("Index out of range");
    return pMem[ind];
}

template<typename T>
const T& TDynamicVector<T>::at(size_t ind) const
{
    if (ind >= sz)
        throw std::out_of_range("Index out of range");
    return pMem[ind];
}

template<typename T>
bool TDynamicVector<T>::operator==(const TDynamicVector& v) const noexcept
{
    if (sz != v.sz)
        return false;
    for (size_t i = 0; i < sz; ++i)
        if (pMem[i] != v.pMem[i])
            return false;
    return true;
}

template<typename T>
bool TDynamicVector<T>::operator!=(const TDynamicVector& v) const noexcept
{
    return !(*this == v);
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator+(T val)
{
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = pMem[i] + val;
    return result;
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator-(T val) // todo v+(-val)
{
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = pMem[i] - val;
    return result;
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator*(T val)
{
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = pMem[i] * val;
    return result;
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator+(const TDynamicVector& v)
{
    if (sz != v.sz)
        throw std::out_of_range("Vector sizes must match");
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = pMem[i] + v.pMem[i];
    return result;
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator-(const TDynamicVector& v) //todo v+v1*(-1)
{
    if (sz != v.sz)
        throw std::out_of_range("Vector sizes must match");
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = pMem[i] - v.pMem[i];
    return result;
}

template<typename T>
T TDynamicVector<T>::operator*(const TDynamicVector& v)
{
    if (sz != v.sz)
        throw std::out_of_range("Vector sizes must match");
    T result = T();
    for (size_t i = 0; i < sz; ++i)
        result = result + pMem[i] * v.pMem[i];
    return result;
}

// Явные инстанцирования для типов, используемых в тестах и матрице
template class TDynamicVector<int>;
template class TDynamicVector<double>;
template class TDynamicVector<float>;
template class TDynamicVector<long long>;

template class TDynamicVector<TDynamicVector<int>>;
template class TDynamicVector<TDynamicVector<double>>;
template class TDynamicVector<TDynamicVector<float>>;
template class TDynamicVector<TDynamicVector<long long>>;



// ---------- TDynamicMatrix ----------

template<typename T>
size_t TDynamicMatrix<T>::checkSize(size_t s)
{
    if (s == 0 || s > MAX_MATRIX_SIZE)
        throw std::out_of_range("Matrix size should be in (0, MAX_MATRIX_SIZE]");
    return s;
}

template<typename T>
TDynamicMatrix<T>::TDynamicMatrix(size_t s)
    : TDynamicVector<TDynamicVector<T>>(checkSize(s))
{
    for (size_t i = 0; i < sz; ++i)
        pMem[i] = TDynamicVector<T>(sz - i);
}

template<typename T>
bool TDynamicMatrix<T>::operator==(const TDynamicMatrix& m) const noexcept
{
    if (sz != m.sz)
        return false;
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = i; j < sz; ++j)
            if (at(i, j) != m.at(i, j))
                return false;
    return true;
}

template<typename T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator*(const T& val)
{
    TDynamicMatrix result(sz);
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = i; j < sz; ++j)
            result[i][j] = at(i, j) * val;
    return result;
}

template<typename T>
TDynamicVector<T> TDynamicMatrix<T>::operator*(const TDynamicVector<T>& v)
{
    if (v.size() != sz)
        throw std::out_of_range("Matrix and vector sizes must match");
    TDynamicVector<T> result(sz);
    for (size_t i = 0; i < sz; ++i)
    {
        T sum = T();
        for (size_t j = i; j < sz; ++j)
            sum = sum + at(i, j) * v[j];
        result[i] = sum;
    }
    return result;
}

template<typename T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator+(const TDynamicMatrix& m)
{
    if (sz != m.sz)
        throw std::out_of_range("Matrix sizes must match");
    TDynamicMatrix result(sz);
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = i; j < sz; ++j)
            result[i][j] = at(i, j) + m.at(i, j);
    return result;
}

template<typename T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator-(const TDynamicMatrix& m)
{
    if (sz != m.sz)
        throw std::out_of_range("Matrix sizes must match");
    TDynamicMatrix result(sz);
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = i; j < sz; ++j)
            result[i][j] = at(i, j) - m.at(i, j);
    return result;
}

template<typename T>
TDynamicMatrix<T> TDynamicMatrix<T>::operator*(const TDynamicMatrix& m)
{
    if (sz != m.sz)
        throw std::out_of_range("Matrix sizes must match");
    TDynamicMatrix result(sz);
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = i; j < sz; ++j)
        {
            T sum = T();
            for (size_t k = i; k <= j; ++k)
                sum = sum + at(i, k) * m.at(k, j);
            result[i][j] = sum;
        }
    return result;
}

// Явные инстанцирования для TDynamicMatrix
template class TDynamicMatrix<int>;
template class TDynamicMatrix<double>;
template class TDynamicMatrix<float>;
template class TDynamicMatrix<long long>;

#endif
