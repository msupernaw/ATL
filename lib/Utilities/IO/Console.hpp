/* 
 * File:   Console.hpp
 * Author: matthewsupernaw
 *
 * Created on June 23, 2014, 3:40 PM
 */

#ifndef CONSOLE_HPP
#define	CONSOLE_HPP


#if defined(WIN32) || defined(WIN64)

#include <windows.h>
#include <wincon.h>

#define WINDOWS_OS

#endif

namespace io {

    enum Color {
        DEFAULT,
        BLACK,
        BLUE,
        RED,
        CYAN,
        GREEN,
        MAGENTA,
        WHITE,
        GRAY,
        BOLD
    };

#ifdef WINDOWS_OS

    class DefaultColors {
    private:

    public:
        WORD defaultColor;

        DefaultColors() : defaultColor(GetCurrentColor()) {
        }

        static unsigned GetCurrentColor() {

            CONSOLE_SCREEN_BUFFER_INFO csbi;
            memset(&csbi, 0, sizeof (csbi));
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
                return csbi.wAttributes;
            }

            return 0;
        }

        WORD operator()() const {
            return defaultColor;
        }


    };

    static DefaultColors defaultColors;

#endif

   
    inline std::ostream & operator<<(std::ostream &out, const Color &c) {

#ifdef WINDOWS_OS
        switch (c) {
            case DEFAULT:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), defaultColors.defaultColor);
                break;

            case BLACK:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                break;
            case BLUE:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                break;
            case RED:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                break;
            case CYAN:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                break;
            case GREEN:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                break;
            case MAGENTA:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                break;
            case WHITE:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                break;
            case GRAY:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                break;
            case BOLD:
                WORD colors = DefaultColors::GetCurrentColor();
                colors |= FOREGROUND_INTENSITY;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colors);
                break;
    
        }

#else

        switch (c) {
            case DEFAULT:
                out << "\033[0m" ;
                break;

            case BLACK:
                out << "\033[0;30m";
                break;
            case BLUE:
                out << "\033[0;34m";
                break;
            case RED:
                out << "\033[0;31m";
                break;
            case CYAN:
                out << "\033[0;36m";
                break;
            case GREEN:
                out << "\033[0;32m";
                break;
            case MAGENTA:
                out << "\033[0;35m";
                break;
            case WHITE:
                out << "\033[0;37m";
                break;
            case GRAY:
                out << "\033[0;37m";
                break;
            case BOLD:
                out << "\033[1m";
                break;
        }
#endif

        return out;
    }



}


#endif	/* CONSOLE_HPP */

