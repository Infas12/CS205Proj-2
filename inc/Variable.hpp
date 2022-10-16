#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <iostream>
#include <map>
#include "Number.hpp"

template <class T>
class VariableManager
{      

    public:

        std::map<std::string, std::string> m_varibleTable;

        static VariableManager<T>* Instance()
        {
            static VariableManager<T> instance;
            return &instance;
        };

        void registerVariable(std::string symbol, std::string value){
            
            auto iter =  m_varibleTable.find(symbol);
            if(iter==m_varibleTable.end()){
                Instance() -> m_varibleTable.insert(std::make_pair(symbol,value));
            }else{
                iter->second = value;
            }

            
        };



        void HandleAssignment(const std::string& expr){
            int _eqnIdx = expr.find('=');
            std::string _value = expr.substr(_eqnIdx+1,expr.length()-_eqnIdx+1);
            std::string _symbol = expr.substr(0,_eqnIdx);
            registerVariable(_symbol,_value);
        };

    private:
        VariableManager(){};

};

template <class T>
class Variable : public Number<T>
{

private:
    std::string m_symbol;
public:
    Variable(std::string sym) : 
        Number<T>(VariableManager<T>::Instance()->getValue(sym)),
        m_symbol(sym)
    {
    };

};

#endif