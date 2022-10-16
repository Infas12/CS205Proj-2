#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <regex>
#include "Number.hpp"
#include "Variable.hpp"
#include "Function.hpp"

static bool isNumberElementAtIndex(std::string expr, int char_idx){
    char c = expr.at(char_idx);
    bool isNum = ('0' <= c && c <= '9') || c=='e' || c=='.';
    bool isNegativeSign = c == '-';
    if(char_idx > 0){
        char prev_c = expr.at(char_idx-1);
        isNegativeSign &= !('0' <= prev_c && prev_c <= '9');
    }
    return isNum || isNegativeSign; 
};

static std::string getRightOperand(std::string expr, int operator_idx){
    std::string rightOp;
    for(int j = operator_idx+1; j < expr.length(); j++){
        bool isNum = isNumberElementAtIndex(expr,j);
        if(!isNum) break;
        rightOp.push_back(expr.at(j));
    }
    return rightOp;
};

static std::string getLeftOperand(std::string expr, int operator_idx){
        std::string leftOp;
        for(int j = operator_idx-1; j >= 0; j--){
            bool isNum = isNumberElementAtIndex(expr,j);
            if(!isNum) break;
            leftOp.insert(0,1,expr.at(j));
        }
        return leftOp;
    };

template <class T>
class Expression : public Number<T>
{
    private:
        std::string m_expr;

    public:

        Expression(std::string str) : Number<T>("0"), m_expr(str){
            evaluate();
        }
        
        virtual T getValue() {
            evaluate();
            return this->m_value;
        };

        void evaluate(){
            //update the expression
            replaceFunction();
            replaceVariable();
            replaceBracket();
            processMulAndDiv();
            processAddAndMin();
            this->m_value = T(m_expr);
        };

        void replaceFunction(){
            FunctionManager<T>* _manager = FunctionManager<T>::Instance();
            for(auto iter = _manager->m_functionTable.begin();iter != _manager->m_functionTable.end(); iter++){
                std::string _funcName = iter->first;
                int _funcBeginIdx = m_expr.find(_funcName);
                if(_funcBeginIdx==std::string::npos) continue; //do not evalueate the functions that do not exist
                
                //get function param
                int _funcEndIdx = _funcBeginIdx + _funcName.length() - 1;
                int bracketCnt = 0;
                int bracketBeginIdx = _funcEndIdx + 1;
                int bracketEndIdx = -1;
                for(int i = bracketBeginIdx; i < m_expr.length(); i++){
                    char _c = m_expr.at(i);
                    if(_c=='(') bracketCnt++;
                    if(_c==')') bracketCnt--;
                    bracketEndIdx = i;
                    if(bracketCnt==0) break;
                }

                std::string sub_expr = m_expr.substr(bracketBeginIdx+1,bracketEndIdx-bracketBeginIdx-1);
                
                Number<T> (*func)(Number<T>) = iter->second;
                Number<T> result = func(Expression<T>(sub_expr));
                T value = result.getValue();
                std::string result_str = value.toString();
                std::string var = m_expr.substr(_funcBeginIdx,bracketEndIdx-_funcBeginIdx+1);

                m_expr.replace(_funcBeginIdx,bracketEndIdx-_funcBeginIdx+1,result_str);

            }            
        }

        void replaceVariable(){
            VariableManager<T>* _manager = VariableManager<T>::Instance();
            for(auto iter = _manager->m_varibleTable.begin();iter != _manager->m_varibleTable.end(); iter++){
                std::string var = iter->first;
                if(m_expr.find(var)==std::string::npos) continue; //do not evalueate the values that do not exist
                std::string value = Expression<T>(iter->second).getValue().toString();
                value.insert(0,1,'(');
                value.push_back(')');
                m_expr = std::regex_replace(m_expr,std::regex(var),value);
            }
        }

        void replaceBracket(){
            bool _clearing = true;
            while(_clearing){
                int _leftBracketIdx = m_expr.find_first_of("(");
                int _rightBracketIdx = -1;
                for(int i = _leftBracketIdx + 1; i < m_expr.length(); i++){
                    if(m_expr.at(i) == '('){
                        _leftBracketIdx = i;
                    }
                    if(m_expr.at(i) == ')'){
                        _rightBracketIdx = i;
                        int _len = _rightBracketIdx-_leftBracketIdx+1;
                        std::string sub_expr = m_expr.substr(_leftBracketIdx,_len);
                        sub_expr.erase(0,1);
                        sub_expr.erase(sub_expr.length()-1,1);
                        m_expr.erase(_leftBracketIdx,_len);
                        auto result =  Expression<T>(sub_expr).getValue().toString();
                        m_expr.insert(_leftBracketIdx,result);
                        break;
                    }
                    if(i==m_expr.length()-1){
                        _clearing = false;
                    }
                }
            }
        };

        void processMulAndDiv(){
            bool _clearing = true;
            while(_clearing){
                for(int i = 0; i < m_expr.length(); i++){
                    if(m_expr.at(i) == '*'|| m_expr.at(i) == '/'){

                        char _operator = m_expr.at(i);

                        std::string leftOp = getLeftOperand(m_expr,i);
                        std::string rightOp = getRightOperand(m_expr,i);
                        
                        if(_operator=='*'){
                            T result = T(leftOp) * T(rightOp);
                            std::string toBeReplaced = leftOp + "\\" + _operator + rightOp;
                            std::string aaa = result.toString();
                            m_expr = std::regex_replace(m_expr,std::regex(toBeReplaced),result.toString());
                        }

                        if(_operator=='/'){
                            std::string toBeReplaced = leftOp + "\\" + _operator + rightOp;
                            T result = T(leftOp) / T(rightOp);
                            std::string a = result.toString();
                            m_expr = std::regex_replace(m_expr,std::regex(toBeReplaced),result.toString());
                        }
                        break;
                    }
                    
                    if(i==m_expr.length()-1){
                        _clearing = false;
                    }
                }
            }            
        };

        void processAddAndMin(){

            bool _clearing = true;

            while(_clearing){
                for(int i = 0; i < m_expr.length(); i++){
                    bool is_minus_sign = m_expr.at(i) == '-' && !isNumberElementAtIndex(m_expr,i);
                    bool is_operand = m_expr.at(i) == '+' || is_minus_sign;

                    if(is_operand){
                        char _operator = m_expr.at(i);
                        std::string leftOp = getLeftOperand(m_expr,i);
                        std::string rightOp = getRightOperand(m_expr,i);
                        
                        if(_operator=='+'){
                            std::string toBeReplaced = leftOp + "\\" + _operator + rightOp;
                            T result = T(leftOp) + T(rightOp);
                            m_expr = std::regex_replace(m_expr,std::regex(toBeReplaced),result.toString());
                        }
                        if(_operator=='-'){
                            std::string toBeReplaced = leftOp + _operator + rightOp;
                            T result = T(leftOp) - T(rightOp);
                            m_expr = std::regex_replace(m_expr,std::regex(toBeReplaced),result.toString());
                        }

                        break;
                    }
                    
                    if(i==m_expr.length()-1){
                        _clearing = false;
                    }
                }
            }            
        };

}; 


#endif