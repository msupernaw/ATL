#ifndef PROFILE_HPP
#define PROFILE_HPP


#include <unordered_map>
#include <locale>
#include <chrono>
#include <iostream>
#include <stdlib.h>


struct profile_entry___ {
    std::string function_name;
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
    double total;
    
    profile_entry___(std::string function_name);
    virtual ~profile_entry___();
    
};

std::unordered_map<std::string, std::pair<double, size_t> > profiled_entries___;

profile_entry___::profile_entry___(std::string function_name) :
function_name(function_name) {
    start = std::chrono::steady_clock::now();
}

profile_entry___::~profile_entry___() {
    typename std::unordered_map<std::string, std::pair<double, size_t> >::iterator it;
    
    std::cout << std::fixed;
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> exact_time = (end - start);
    total = exact_time.count();
    it = profiled_entries___.find(function_name);
    if (it != profiled_entries___.end()) {
        it->second.first += total;
        it->second.second++;
    } else {
        std::pair<double, size_t> p; // = profiled_entries___[function_name];
        p.first = total;
        p.second = 1;
        profiled_entries___.insert(std::make_pair(function_name, p));
    }
    
}

void dump_profier() {
    
    typename std::unordered_map<std::string, std::pair<double, size_t> >::iterator it;
    std::cout.precision(10);
    std::cout << std::fixed << "\n\nFunction, total time, average time, calls\n";
    for (it = profiled_entries___.begin(); it != profiled_entries___.end(); ++it) {
        std::cout << it->first << ", " << it->second.first << ", " << it->second.first / it->second.second << "," << it->second.second << "\n";
    }
    
}

#define PROFILE() profile_entry___(__func__)

#endif 
