#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "Number.hpp"
#include <map>

template <class T>
class FunctionManager{

public:

    typedef Number<T> (*FUNC)(Number<T>); //for now we only support single-parameter function

    std::map<std::string, FUNC> m_functionTable;

    static FunctionManager* Instance(){
        static FunctionManager<T> instance;
        return &instance;
    }

    void registerFunction(std::string name, FUNC handler){
        m_functionTable.insert(std::make_pair(name,handler));
    }

private:
    FunctionManager(){};    

};



#endif