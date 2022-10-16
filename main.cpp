#include "IFSNumber.hpp"
#include "Number.hpp"
#include "Variable.hpp"
#include "Expression.hpp"
#include <regex>
#include "Function.hpp"


void removeSpaces(std::string& _expr){
    _expr = std::regex_replace(_expr,std::regex(" "),"");
}

template <class T>
Number<T> getNeg(Number<T> operand){
    return -operand;
}

template <class T>
Number<T> abs(Number<T> operand){
    Number<T> test("0"); 
    bool smaller = operand < Number<T>("0");
    return operand < Number<T>("0") ? -operand : operand;
}

template <class T>
Number<T> IFSsqrt(Number<T> operand){
    return sqrt(operand); 
} //evil implementation


int main(int argc, char *argv[]) {

    std::cout << "CS205 Proj#2 by Xiaohan Liu." << std::endl;

    //Register constants here
    VariableManager<IFSNumber>::Instance()->registerVariable("PI","3.1415926");

    //Register functions here
    FunctionManager<IFSNumber>::Instance()->registerFunction("abs",abs<IFSNumber>);
    FunctionManager<IFSNumber>::Instance()->registerFunction("getNeg",getNeg<IFSNumber>);
    FunctionManager<IFSNumber>::Instance()->registerFunction("sqrt",IFSsqrt<IFSNumber>);

    std::cout << "Functions & constants loaded. Enter 'exit()' to quit." << std::endl;


    std::string input;
    std::cout << ">> ";
    std::getline(std::cin,input);
    try{
        while(input != "exit()"){
            removeSpaces(input);    
            if(input.find('=')==std::string::npos){ //is expression
                Expression<IFSNumber> result(input);
                std::cout << "ans = " << result.getValue() << std::endl; //store answer as a variable
                VariableManager<IFSNumber>::Instance()->registerVariable("ans",result.getValue().toString());
            }else{ //is assignment
                VariableManager<IFSNumber>::Instance()->HandleAssignment(input);
            }
            std::cout << ">> ";
            std::getline(std::cin,input);
        }
    }catch(std::exception e){
        std::cout << "An error occured, please check your input." << std::endl;
    }

}