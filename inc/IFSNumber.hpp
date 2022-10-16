#ifndef IFSNUMBER_HPP
#define IFSNUMBER_HPP

#include <iostream>
#include <sstream>
#include <cstring>

#define MAX_SIZE 64
#define DIVISION_PRECION 10
#include <cmath>



class IFSNumber
{
    private:

        bool m_isNegative;
        int m_numbers[MAX_SIZE];
        int m_length;
        int m_exponent;

    public:
 
        //Basic constructor with string
        IFSNumber(std::string num);
        void setValue(std::string num);

        //Overriding operators
        friend IFSNumber operator + (const IFSNumber &c1,const IFSNumber &c2);
        friend IFSNumber operator - (const IFSNumber &c1,const IFSNumber &c2);
        friend IFSNumber operator - (const IFSNumber &c1);
        friend IFSNumber operator * (const IFSNumber &c1,const IFSNumber &c2);
        friend IFSNumber operator / (const IFSNumber &c1,const IFSNumber &c2);

        friend IFSNumber sqrt(const IFSNumber &c1);

        friend void operator += (IFSNumber &c1,const IFSNumber &c2);
        friend void operator -= (IFSNumber &c1,const IFSNumber &c2);
        friend void operator *= (IFSNumber &c1,const IFSNumber &c2);
        friend void operator /= (IFSNumber &c1,const IFSNumber &c2);

        friend bool operator > (const IFSNumber &c1,const IFSNumber &c2);
        friend bool operator < (const IFSNumber &c1,const IFSNumber &c2);
        friend bool operator >= (const IFSNumber &c1,const IFSNumber &c2);
        friend bool operator <= (const IFSNumber &c1,const IFSNumber &c2);
        friend bool operator == (const IFSNumber &c1,const IFSNumber &c2);

        //using default assignment method

        //using default copy constructor

        //using default destructor

        //output
        std::ostream &write(std::ostream &dest) const;
        std::ostream &writeFpFormat(std::ostream &dest) const;
        std::ostream &writeSciFormat(std::ostream &dest) const;
        
        std::string toString()
        {
            std::ostringstream buffer; 
            this->write(buffer);
            return buffer.str();
        }
        
    private:

        IFSNumber(bool isNegative, int exponent, const int * numbers, int length);


};


inline std::ostream& operator << (std::ostream& output,const IFSNumber& c) {
    c.write(output);
}      


#endif