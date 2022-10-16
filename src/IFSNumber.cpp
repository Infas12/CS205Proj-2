#include "IFSNumber.hpp"

int floorMod(int a, int b)
{
    return a % b < 0 ? (a % b + b) : a % b;
}

int floorDiv(int a, int b)
{
    return a % b < 0 ? a / b - 1 : a / b;
}

IFSNumber::IFSNumber(std::string num) : m_isNegative(false),
                                        m_exponent(0),
                                        m_numbers{0},
                                        m_length(0)
{
    IFSNumber::setValue(num);
}

IFSNumber::IFSNumber(bool isNegative, int exponent, const int *numbers, int length) : m_isNegative(isNegative),
                                                                                      m_exponent(exponent),
                                                                                      m_numbers{0},
                                                                                      m_length(length)
{
    for (int i = 0; i < length; i++)
    {
        m_numbers[i] = numbers[i];
    }
}

void IFSNumber::setValue(std::string num)
{
    // negative sign
    int negative_pos = num.rfind("-", 1);

    if (negative_pos == 0)
    {
        m_isNegative = true;
        num.erase(negative_pos, 1);
    }

    // exponential part
    int e_pos = num.rfind("e");
    if (e_pos != -1)
    {
        m_exponent += std::stoi(num.substr(e_pos + 1, num.length() - e_pos).c_str());
        num = num.substr(0, e_pos);
    }

    // remove dot
    int dot_pos = num.rfind(".");
    if (dot_pos != -1)
    {
        m_exponent -= (num.length() - 1 - dot_pos);
        num.erase(dot_pos, 1);
    }

    // start with multiple '0's, e.g: 00.123
    if (dot_pos > 1)
    {
        if (num.c_str()[0] == '0' && num.c_str()[1] == '0')
            throw std::exception();
    }

    // if the head start with 0 then remove them
    while (num.at(0) == '0' && num.length() != 1)
    {
        num.erase(0, 1);
    }

    // body part
    const char *numarr = num.c_str();

    // convert to int array
    for (int i = 0; i < num.length(); i++)
    {
        int j = num.length() - 1 - i;
        if (numarr[j] > '9' || numarr[j] < '0')
        {
            throw std::exception();
        }
        m_numbers[i] = numarr[j] - '0';
        m_length++;
    }
}

IFSNumber operator+(const IFSNumber &c1, const IFSNumber &c2)
{

    // minus
    if (c1.m_isNegative ^ c2.m_isNegative)
    {
        if (c1.m_isNegative)
        {
            return c2 - (-c1);
        }
        else
        {
            return c1 - (-c2);
        }
    }

    // align the two operands
    int _buf_c1[MAX_SIZE] = {0};
    int _buf_c2[MAX_SIZE] = {0};
    int _exponential = 0;

    if (c1.m_exponent <= c2.m_exponent)
    {
        memcpy(_buf_c2 + c2.m_exponent - c1.m_exponent, c2.m_numbers, c2.m_length * sizeof(int));
        memcpy(_buf_c1, c1.m_numbers, c1.m_length * sizeof(int));
        _exponential = c1.m_exponent;
    }
    else
    {
        memcpy(_buf_c1 + c1.m_exponent - c2.m_exponent, c1.m_numbers, c1.m_length * sizeof(int));
        memcpy(_buf_c2, c2.m_numbers, c2.m_length * sizeof(int));
        _exponential = c2.m_exponent;
    }

    // add them together.
    int _result_buf[MAX_SIZE] = {0};
    int _result_len = MAX_SIZE;
    int _result_idx = 0;
    for (int i = 0; i < MAX_SIZE; i++)
    {
        int p = _buf_c1[i];
        int q = _buf_c2[i];
        _result_buf[i] += p + q;
        _result_buf[i + 1] += _result_buf[i] / 10;
        _result_buf[i] %= 10;
    }

    // Find highest bit
    while (_result_buf[_result_len - 1] == 0 && _result_len > 0)
    {
        _result_len--;
    };

    bool _isNegative = c1.m_isNegative;

    return IFSNumber(_isNegative, _exponential, _result_buf, _result_len);
}

IFSNumber operator-(const IFSNumber &c1)
{
    return IFSNumber(!c1.m_isNegative, c1.m_exponent, c1.m_numbers, c1.m_length);
}

IFSNumber operator-(const IFSNumber &c1, const IFSNumber &c2)
{

    // plus
    if (c1.m_isNegative ^ c2.m_isNegative)
    {
        if (c1.m_isNegative)
        {
            return -(c2 + (-c1));
        }
        else
        {
            return c1 + (-c2);
        }
    }

    // 2. align the two numbers by adding additional 0s.
    int _buf_c1[MAX_SIZE] = {0};
    int _buf_c2[MAX_SIZE] = {0};
    int _exponential = 0;
    int _highestBitIdx = 0;
    int _c1_highestBit = 0;
    int _c2_highestBit = 0;

    if (c1.m_exponent <= c2.m_exponent)
    {
        memcpy(_buf_c2 + c2.m_exponent - c1.m_exponent, c2.m_numbers, c2.m_length * sizeof(int));
        memcpy(_buf_c1, c1.m_numbers, c1.m_length * sizeof(int));
        _c1_highestBit = c1.m_length - 1;
        _c2_highestBit = c2.m_length + c2.m_exponent - c1.m_exponent - 1;
        _exponential = c1.m_exponent;
    }
    else
    {
        memcpy(_buf_c1 + c1.m_exponent - c2.m_exponent, c1.m_numbers, c1.m_length * sizeof(int));
        memcpy(_buf_c2, c2.m_numbers, c2.m_length * sizeof(int));
        _c1_highestBit = c1.m_length + c1.m_exponent - c2.m_exponent - 1;
        _c2_highestBit = c2.m_length - 1;
        _exponential = c2.m_exponent;
    }
    _highestBitIdx = _c1_highestBit > _c2_highestBit ? _c1_highestBit : _c2_highestBit;

    // 3. compare the highest bit.
    int _result_buf[MAX_SIZE] = {0};
    int _result_len = MAX_SIZE;
    int _result_idx = 0;

    bool _c1Larger = _buf_c1[_highestBitIdx] > _buf_c2[_highestBitIdx];

    for (int i = 0; i < MAX_SIZE; i++)
    {
        int p = _buf_c1[i];
        int q = _buf_c2[i];
        if (_c1Larger)
        {
            _result_buf[i] += p - q;
        }
        else
        {
            _result_buf[i] += q - p;
        }
        _result_buf[i + 1] += floorDiv(_result_buf[i], 10);
        _result_buf[i] = floorMod(_result_buf[i], 10);
    }

    // 4. Find highest bit
    while (_result_buf[_result_len - 1] == 0 && _result_len > 0)
    {
        _result_len--;
    };

    bool _isNegative = _c1Larger ^ !c1.m_isNegative;

    return IFSNumber(_isNegative, _exponential, _result_buf, _result_len);
}

IFSNumber operator*(const IFSNumber &c1, const IFSNumber &c2)
{

    int _result_buf[MAX_SIZE] = {0};
    int _result_len = MAX_SIZE;

    for (int i = 0; i < c1.m_length; i++)
    {
        for (int j = 0; j < c2.m_length; j++)
        {
            int p = c1.m_numbers[i];
            int q = c2.m_numbers[j];
            _result_buf[i + j] += p * q;
            _result_buf[i + j + 1] = _result_buf[i + j + 1] + _result_buf[i + j] / 10;
            _result_buf[i + j] %= 10;
        }
    }

    while (_result_buf[_result_len - 1] == 0 && _result_len > 0)
    {
        _result_len--;
    };

    bool _isNegative = c1.m_isNegative ^ c2.m_isNegative;
    int _exponential = c1.m_exponent + c2.m_exponent;

    return IFSNumber(_isNegative, _exponential, _result_buf, _result_len);
}

IFSNumber operator/(const IFSNumber &c1, const IFSNumber &c2)
{

    // Forgive me
    long divisor = 0;
    long res = 0;
    for (int i = 0; i < c2.m_length; i++)
    {
        divisor += c2.m_numbers[i] * pow(10, i);
    }

    int _result_buf[MAX_SIZE] = {0};
    int _result_len = MAX_SIZE;
    int _result_idx = 0;

    int dividendBuf[MAX_SIZE] = {0};
    memcpy(dividendBuf + DIVISION_PRECION, c1.m_numbers, c1.m_length * sizeof(int));

    for (int i = c1.m_length + DIVISION_PRECION - 1; i >= 0; i--)
    {
        res = res * 10 + dividendBuf[i];
        _result_buf[i] = res / divisor;
        res = res % divisor;
    }

    bool _isNegative = c1.m_isNegative ^ c2.m_isNegative;
    while (_result_buf[_result_len - 1] == 0 && _result_len > 0)
    {
        _result_len--;
    };    
    // int _length = c1.m_length + DIVISION_PRECION;
    int _exponential = c1.m_exponent - c2.m_exponent - DIVISION_PRECION;

    return IFSNumber(_isNegative, _exponential, _result_buf, _result_len);
}

IFSNumber sqrt(const IFSNumber &c1)
{
    double operand = 0;
    for (int i = 0; i < c1.m_length; i++)
    {
        operand += c1.m_numbers[i] * pow(10, i);
    }
    operand *= pow(10, c1.m_exponent);
    double result = sqrt(operand);
    if (c1.m_isNegative)
        result = -result;
    return IFSNumber(std::to_string(result));
}

void operator+=(IFSNumber &c1, const IFSNumber &c2)
{
    c1 = c1 + c2;
}

void operator-=(IFSNumber &c1, const IFSNumber &c2)
{
    c1 = c1 - c2;
}

void operator*=(IFSNumber &c1, const IFSNumber &c2)
{
    c1 = c1 * c2;
}

void operator/=(IFSNumber &c1, const IFSNumber &c2)
{
    c1 = c1 / c2;
}

bool operator==(const IFSNumber &c1, const IFSNumber &c2)
{
    IFSNumber result = c1 - c2;
    for (int i = 0; i < result.m_length; i++)
    {
        if (result.m_numbers[i] != 0)
        {
            return false;
        }
    }
    return true;
}

bool operator>(const IFSNumber &c1, const IFSNumber &c2)
{
    IFSNumber result = c1 - c2;
    return result.m_isNegative == false && !(c1 == c2);
}

bool operator<(const IFSNumber &c1, const IFSNumber &c2)
{
    IFSNumber result = c1 - c2;
    return result.m_isNegative == true && !(c1 == c2);
}

bool operator>=(const IFSNumber &c1, const IFSNumber &c2)
{
    return !(c1 < c2);
}

bool operator<=(const IFSNumber &c1, const IFSNumber &c2)
{
    return !(c1 > c2);
}

std::ostream &IFSNumber::write(std::ostream &dest) const
{

    bool isAllZero = true;

    for (int j = 0; j < 64; j++)
    {
        isAllZero &= (m_numbers[j] == 0);
    }

    if (isAllZero)
    {
        dest << "0";
        return dest;
    }

    int exp = m_exponent + m_length - 1;
    bool use_sci = (exp < -5 || exp > 5) && (exp != m_length - 1);

    if (use_sci)
    {
        writeSciFormat(dest);
    }
    else
    {
        writeFpFormat(dest);
    }

    return dest;
}

std::ostream &IFSNumber::writeFpFormat(std::ostream &dest) const
{

    if (m_isNegative)
        dest << "-";

    int _number_buf[64];
    int _num_buf_idx = 0;

    int _point_pos = m_length + m_exponent - 1;

    if (_point_pos < 0)
    {
        for (int i = _point_pos; i < 0; i++)
        {
            dest << "0";
            if (i == _point_pos)
                dest << ".";
        }
    }

    for (int i = 0; i < m_length; i++)
    {
        
        int j = m_length - 1 - i;
        dest << m_numbers[j];
        if (i == _point_pos && i != (m_length - 1))
            dest << ".";
    }

    for (int i = 0; i < m_exponent; i++)
    {
        dest << "0";
    }

    return dest;
}

std::ostream &IFSNumber::writeSciFormat(std::ostream &dest) const
{

    if (m_isNegative)
        dest << "-";

    for (int i = 0; i < m_length; i++)
    {
        if (i == 1)
            dest << ".";
        int j = m_length - 1 - i;
        dest << m_numbers[j];
    }

    dest << "e";
    dest << m_exponent + m_length - 1;

    return dest;
}
