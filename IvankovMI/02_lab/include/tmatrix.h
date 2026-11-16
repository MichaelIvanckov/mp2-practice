// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

// в некоторых местах интерфейс изменен студентом, т. к. не соответствует соглашению 
// или не подходит под специфику конкретной лабораторной работы 
// (отмечено символом "!!" перед комментарием и содержит пояснения)

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <cassert>
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
    explicit TDynamicVector(size_t size = 1) : sz(size)        // !! добавлен explicit, т. к. без него некоторые тесты не собирались + это логично с т. з. пользователя и соответствует соглашениям
    {
        if (sz == 0 || sz > MAX_VECTOR_SIZE)
            throw out_of_range("Vector size should be greater than zero and less than MAX_VECTOR_SIZE");
        pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
    }
    TDynamicVector(T* arr, size_t s) : sz(s)
    {
        assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
        pMem = new T[sz];
        std::copy(arr, arr + sz, pMem);
    }
    TDynamicVector(const TDynamicVector<T>& v);                // !! отсутствовал "<T>" после типа аргумента
    TDynamicVector(TDynamicVector<T>&& v) noexcept;            // !! отсутствовал "<T>" после типа аргумента
    ~TDynamicVector();

    TDynamicVector<T>& operator=(const TDynamicVector<T>& v);     // !! отсутствовал "<T>" после типа аргумента и возвр. типа
    TDynamicVector<T>& operator=(TDynamicVector<T>&& v) noexcept; // !! отсутствовал "<T>" после типа аргумента и возвр. типа

    size_t size() const noexcept { return sz; }

    // индексация
    T& operator[](size_t ind);
    const T& operator[](size_t ind) const;

    // индексация с контролем
    T& at(size_t ind);
    const T& at(size_t ind) const;

    // сравнение
    bool operator==(const TDynamicVector<T>& v) const noexcept;  // !! отсутствовал "<T>" после типа аргумента 
    bool operator!=(const TDynamicVector<T>& v) const noexcept;  // !! отсутствовал "<T>" после типа аргумента

    // скалярные операции
    TDynamicVector<T> operator+(T val) const;                       // !! отсутствовал "const" и "<T>" после возвр. типа
    TDynamicVector<T> operator-(T val) const;                       // !! отсутствовал "const" и "<T>" после возвр. типа
    TDynamicVector<T> operator*(T val) const;                       // !! отсутствовал "const" и "<T>" после возвр. типа

    // векторные операции
    TDynamicVector<T> operator+(const TDynamicVector<T>& v) const;    // !! отсутствовал "const" и "<T>"
    TDynamicVector<T> operator-(const TDynamicVector<T>& v) const;    // !! отсутствовал "const" и "<T>"
    T operator*(const TDynamicVector<T>& v) const;    // !! удалено "noexcept(noexcept(T()))" т. к. есть тест с именем «cant_multiply_vectors_with_not_equal_size», предполагающий выбрасывание исключения; отсутствовал "const" и "<T>"

    friend void swap(TDynamicVector<T>& lhs, TDynamicVector<T>& rhs) noexcept   // !! отсутствовал "<T>" после типа аргумента
    {
        std::swap(lhs.sz, rhs.sz);
        std::swap(lhs.pMem, rhs.pMem);
    }

    // ввод/вывод
    friend istream& operator>>(istream& istr, TDynamicVector<T>& v)             // !! отсутствовал "<T>" после типа аргумента
    {
        for (size_t i = 0; i < v.sz; i++)
            istr >> v.pMem[i]; // требуется оператор>> для типа T
        return istr;
    }
    friend ostream& operator<<(ostream& ostr, const TDynamicVector<T>& v)       // !! отсутствовал "<T>" после типа аргумента
    {
        for (size_t i = 0; i < v.sz; i++)
            ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
        return ostr;
    }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
    using TDynamicVector<TDynamicVector<T>>::pMem;
    using TDynamicVector<TDynamicVector<T>>::sz;
public:
    TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
    {
        for (size_t i = 0; i < sz; i++)
            pMem[i] = TDynamicVector<T>(sz);
    }

    using TDynamicVector<TDynamicVector<T>>::operator[];
    using TDynamicVector<TDynamicVector<T>>::size;           // !! добавлено, т. к. тесты предполагают проверку размера матрицы

    // !! индексация с контролем
    T& at(size_t i, size_t j) { throw "Method is not implemented"; }                               // !! добавлен новый метод для корректной работы программы-примера (специфика верхнетреугольных матриц)
    const T& at(size_t i, size_t j) const { throw "Method is not implemented"; }                   // !! добавлен новый метод для корректной работы программы-примера (специфика верхнетреугольных матриц)

    // сравнение
    using TDynamicVector<TDynamicVector<T>>::operator==;     // !! заменено на пробрасывание, это не меняет интерфейс

    // матрично-скалярные операции
    TDynamicMatrix<T> operator*(const T& val) const          // !! отсутствовал "const" и "<T>" после возвр. типа
    {
        throw "Method is not implemented";
    }

    // матрично-векторные операции
    TDynamicVector<T> operator*(const TDynamicVector<T>& v) const    // !! отсутствовал "const"
    {
        throw "Method is not implemented";
    }

    // матрично-матричные операции
    TDynamicMatrix<T> operator+(const TDynamicMatrix<T>& m) const    // !! отсутствовал "const" и "<T>" после типа аргумента и возвр. типа
    {
        throw "Method is not implemented";
    }
    TDynamicMatrix<T> operator-(const TDynamicMatrix<T>& m) const    // !! отсутствовал "const" и "<T>" после типа аргумента и возвр. типа
    {
        throw "Method is not implemented";
    }
    TDynamicMatrix<T> operator*(const TDynamicMatrix<T>& m) const    // !! отсутствовал "const" и "<T>" после типа аргумента и возвр. типа
    {
        throw "Method is not implemented";
    }

    // ввод/вывод
    friend istream& operator>>(istream& istr, TDynamicMatrix<T>& v)         // !! отсутствовал "<T>" после типа аргумента
    {
        throw "Method is not implemented";
    }
    friend ostream& operator<<(ostream& ostr, const TDynamicMatrix<T>& v)   // !! отсутствовал "<T>" после типа аргумента
    {
        throw "Method is not implemented";
    }
};



// ---------- Реализация: -------------

// ---------- TDynamicVector ----------


template<typename T>
TDynamicVector<T>::TDynamicVector(const TDynamicVector<T>& v) : sz(v.sz)
{
    pMem = new T[sz];
    std::copy(v.pMem, v.pMem + sz, pMem);
}

template<typename T>
TDynamicVector<T>::TDynamicVector(TDynamicVector<T>&& v) noexcept
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
TDynamicVector<T>& TDynamicVector<T>::operator=(const TDynamicVector<T>& v)
{
    if (this != &v)
    {
        TDynamicVector tmp(v);
        swap(*this, tmp);
    }
    return *this;
}

template<typename T>
TDynamicVector<T>& TDynamicVector<T>::operator=(TDynamicVector<T>&& v) noexcept
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
bool TDynamicVector<T>::operator==(const TDynamicVector<T>& v) const noexcept
{
    if (sz != v.sz)
        return false;
    for (size_t i = 0; i < sz; ++i)
        if (pMem[i] != v.pMem[i])
            return false;
    return true;
}

template<typename T>
bool TDynamicVector<T>::operator!=(const TDynamicVector<T>& v) const noexcept
{
    return !(*this == v);
}

/*template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator+()
{
    return TDynamicVector(*this);
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator+(T val) const
{
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = -(pMem[i]);
    return result;
}*/

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator+(T val) const
{
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = pMem[i] + val;
    return result;
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator-(T val) const // todo v+(-val)  СДЕЛАНО!
{
    return (*this) + (-val);
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator*(T val) const
{
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = pMem[i] * val;
    return result;
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator+(const TDynamicVector<T>& v) const
{
    if (sz != v.sz)
        throw std::out_of_range("Vector sizes must match");
    TDynamicVector result(sz);
    for (size_t i = 0; i < sz; ++i)
        result.pMem[i] = pMem[i] + v.pMem[i];
    return result;
}

template<typename T>
TDynamicVector<T> TDynamicVector<T>::operator-(const TDynamicVector<T>& v) const //todo v+v1*(-1)  СДЕЛАНО!
{
    return (*this) + (v * static_cast<T>(-1));
}

template<typename T>
T TDynamicVector<T>::operator*(const TDynamicVector<T>& v) const 
{
    if (sz != v.sz)
        throw std::out_of_range("Vector sizes must match");
    T result = T();
    for (size_t i = 0; i < sz; ++i)
        result = result + pMem[i] * v.pMem[i];
    return result;
}

#endif
