#ifndef STRINGUTIL_HPP
#define STRINGUTIL_HPP

#include <string>
#include <sstream>
#include <vector>

namespace util {
    
    /* Convert double to string with specified number of places after the decimal. */
    std::string prd(const double &x, const int decDigits) {
        std::stringstream ss;
        ss << std::fixed;
        ss.precision(decDigits); // set # places after decimal
        ss << x;
        return ss.str();
    }
    
    /* Convert double to string with specified number of places after the decimal
     and left padding. */
    std::string prd(const double &x, const int decDigits, const int width) {
        std::stringstream ss;
        ss << std::fixed << std::right;
        ss.fill(' '); // fill space around displayed #
        ss.width(width); // set  width around displayed #
        ss.precision(decDigits); // set # places after decimal
        ss << x;
        return ss.str();
    }
    
    /*! Center-aligns string within a field of width w. Pads with blank spaces
     to enforce alignment. */
    std::string center(const std::string &s, const int w) {
        std::stringstream ss, spaces;
        int padding = w - s.size(); // count excess room to pad
        for (int i = 0; i < padding / 2; ++i)
            spaces << " ";
        ss << spaces.str() << s << spaces.str(); // format with padding
        if (padding > 0 && padding % 2 != 0) // if odd #, add 1 space
            ss << " ";
        return ss.str();
    }
    
    /**
     * Trim the left side.
     *
     * @param s
     * @return
     */
    static inline std::string& LeftTrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }
    
    /**
     * Trim the right side.
     *
     * @param s
     * @return
     */
    static inline std::string& RightTrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }
    
    /**
     * Trim both sides.
     *
     * @param s
     * @return
     */
    static inline std::string& Trim(std::string &s) {
        return LeftTrim(RightTrim(s));
    }
    
    static void Tokenize(const std::string& str, std::vector<std::string>& tokens,
                         const std::string& delimiters = " ", const bool trimEmpty = true) {
        std::string::size_type pos, lastPos = 0;
        while (true) {
            pos = str.find_first_of(delimiters, lastPos);
            if (pos == std::string::npos) {
                pos = str.length();
                
                if (pos != lastPos || !trimEmpty)
                    tokens.push_back(std::vector<std::string>::value_type(str.data() + lastPos,
                                                                          (std::vector<std::string>::value_type::size_type)pos - lastPos));
                
                break;
            } else {
                if (pos != lastPos || !trimEmpty)
                    tokens.push_back(std::vector<std::string>::value_type(str.data() + lastPos,
                                                                          (std::vector<std::string>::value_type::size_type)pos - lastPos));
            }
            
            lastPos = pos + 1;
        }
    }
    
    static bool StartsWith(const std::string &value1, const std::string &value2) {
        return value1.find(value2) == 0;
    }
    
    template <typename T>
    T StringToNumber(const std::string &Text) {
        std::istringstream ss(Text);
        T result;
        return (ss >> result) ? result : 0;
    }
}


#endif