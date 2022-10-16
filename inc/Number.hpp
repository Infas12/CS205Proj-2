#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <iostream>

template <class T>
class Number
{

protected:

    T m_value;

public:

    Number(std::string num) : m_value(num){};
    Number(T value): m_value(value){};
    const T& getValue() const {return m_value;};
    T setValue(std::string num) { m_value = T(num);}
    T setValue(T val) { m_value = val;}

    friend Number<T> operator + (const Number<T> &c1,const Number<T> &c2){
        return Number<T>(c1.getValue() + c2.getValue());
    };
    
    friend Number<T> operator - (const Number<T> &c1,const Number<T> &c2){
        return Number<T>(c1.getValue() - c2.getValue());
    };
    
    friend Number<T> operator - (const Number<T> &c1){
        return Number<T>(-c1.getValue());
    };

    friend Number<T> operator * (const Number<T> &c1,const Number<T> &c2){
        return Number<T>(c1.getValue() * c2.getValue());
    };

    friend Number<T> operator / (const Number<T> &c1,const Number<T> &c2){
        return Number<T>(c1.getValue() / c2.getValue());
    };

    friend Number<T> sqrt(const Number<T> &c1){
        return Number<T>(sqrt(c1.getValue()));
    }

    friend bool operator == (const Number<T> &c1,const Number<T> &c2){
        return (c1.getValue() == c2.getValue());
    };    

    friend bool operator < (const Number<T> &c1,const Number<T> &c2){
        return (c1.getValue() < c2.getValue());
    };

    friend bool operator > (const Number<T> &c1,const Number<T> &c2){
        return (c1.getValue() > c2.getValue());
    };

    friend bool operator <= (const Number<T> &c1,const Number<T> &c2){
        return (c1.getValue() <= c2.getValue());
    };

    friend bool operator >= (const Number<T> &c1,const Number<T> &c2){
        return (c1.getValue() >= c2.getValue());
    };



};





#endif