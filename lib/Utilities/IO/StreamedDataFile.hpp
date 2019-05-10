#ifndef STREAMEDDATAFILE_HPP
#define STREAMEDDATAFILE_HPP
#include <iostream>
#include <sstream>
#include <limits>
#include <fstream>
#include <cmath>
#include "../StringUtil.hpp"
#include "../../Variable.hpp"
#include "../../Matrix.hpp"
#include "../../Vector.hpp"
//#include "../../Array.hpp"

namespace atl {

    class StreamedDataFile {
        std::ifstream input;

    public:

        StreamedDataFile() {

        }

        StreamedDataFile(const char* path) {
            input.open(path);
        }

        void open(const char* path) {
            input.open(path);
        }

        bool good() {
            return input.good();
        }

        void operator>>(char* v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(unsigned char* v) {
            return operator>>((char*) v);
        }

        void operator>>(signed char*v) {
            return operator>>((char*) v);
        }

        void operator>>(char& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(unsigned char& v) {
            return operator>>((char&) v);
        }

        void operator>>(signed char& v) {
            return operator>>((char&) v);
        }

        void operator>>(int& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(long& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(short& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(unsigned int& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(unsigned long& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(unsigned short& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(bool& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(float& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(double& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(long double& v) {
            for (;;) {

                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail()) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

        void operator>>(std::string& v) {
            for (;;) {

                
                
                input >> v;

                if (input.eof() || input.bad()) {
                    break;
                } else if (input.fail() || util::StartsWith(v,"#")) {
                    input.clear(); // unset failbit
                    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip bad input
                } else {
                    break;
                }
            }
        }

    private:

    };

    template<typename T>
    StreamedDataFile& operator>>(StreamedDataFile& in, atl::Variable<T>& v) {
        T data = 0.0;
        in>>data;
        v = data;
        return in;
    }

//    template<typename T>
//    StreamedDataFile& operator>>(StreamedDataFile& in, atl::Vector<T>& v) {
//        for (int i = 0; i < v.Size(0); i++) {
//            in >> v(i);
//        }
//        return in;
//    }
//
//    template<typename T>
//    StreamedDataFile& operator>>(StreamedDataFile& in, atl::Matrix<T>& m) {
//        for (int i = 0; i < m.Size(0); i++) {
//            for (int j = 0; j < m.Size(1); j++) {
//                in >> m(i, j);
//            }
//        }
//        return in;
//    }

}


#endif