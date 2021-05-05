/*
    a simple demonstration of the strategy design pattern
    -slightly redundant, duplicates code, verbose, and not optimal (copies everywhere)
    but demonstrates the idea
    -revision will correct some of the above
*/

// expose our interface
#include "strategy.h"

// std includes
#include <vector>
#include <algorithm>
#include <iostream>

// develop some concrete strategies
namespace int_vec_strategy {

    // for this particular example, extend interface for client use
    class extended_strategy : public blotter::strategy_base {
    public:
        ~extended_strategy() {}
        virtual std::vector<int> data() = 0;
    };

    // implement stategy interface(s) for each concrete strategy
    class sort_asc : public extended_strategy {
    public:
        sort_asc() {}
        sort_asc(const std::vector<int>& input)
            : _v(input) {}
        void execute()
        {
            auto comp = [](const int lhs, const int rhs)
            {
                return lhs < rhs;
            };
            std::sort(std::begin(_v), std::end(_v), comp);
        }
        std::vector<int> data() { return _v; }
    private:
        std::vector<int> _v;    
    };
    class sort_desc : public extended_strategy {
    public:
        sort_desc() {}
        sort_desc(const std::vector<int>& input)
            : _v(input) {}
        void execute()
        {
            auto comp = [](const int lhs, const int rhs)
            {
                return lhs >= rhs;
            };
            std::sort(std::begin(_v), std::end(_v), comp);
        }
        std::vector<int> data() { return _v; }
    private: 
        std::vector<int> _v;
    };
};

// develop a client which uses some concrete strategies
class int_vector {
public:
    int_vector()
        : _int_vec(std::vector<int>{}), _sort_strat(nullptr) {}
    int_vector(const std::vector<int>& input)
        : _int_vec(input), _sort_strat(nullptr) {}
    ~int_vector()
    {
        if (_sort_strat)
        {
            delete _sort_strat;
            _sort_strat = nullptr;
        }
    }
    void sort() 
    {
        if (_sort_strat) 
        {
            _sort_strat->execute();
            _int_vec = _sort_strat->data();
        }
    }
    void set_sort_strategy(int_vec_strategy::extended_strategy* strategy)
    {
        if (_sort_strat)
        {
            delete _sort_strat;
            _sort_strat = nullptr;
        }
        _sort_strat = strategy;
    }
    std::vector<int> data() { return _int_vec; }
private:
    int_vec_strategy::extended_strategy* _sort_strat;
    std::vector<int> _int_vec;
};

int main(void)
{
    // start
    int_vector int_vec{ std::vector<int> {10, 1, 9, 2, 8, 3, 7, 4, 6, 5} };
    std::cout << "executing no strategy..." << std::endl;
    std::vector<int> result = int_vec.data();
    std::cout << "int_vec = {";
    for (std::size_t i = 0; i < result.size(); ++i)
    {
        if (i < result.size() - 1)
        {
            std::cout << result[i] << ',' << ' ';
        }
        else
        {
            std::cout << result[i];
        }
    }
    std::cout << '}' << std::endl;
    // strategies
    std::cout << "executing sort_asc strategy..." << std::endl;
    int_vec.set_sort_strategy(new int_vec_strategy::sort_asc{int_vec.data()});
    int_vec.sort();
    result = int_vec.data();
    std::cout << "int_vec = {";
    for (std::size_t i = 0; i < result.size(); ++i)
    {
        if (i < result.size() - 1)
        {
            std::cout << result[i] << ',' << ' ';
        }
        else
        {
            std::cout << result[i];
        }
    }
    std::cout << '}' << std::endl;

    std::cout << "executing sort_desc strategy..." << std::endl;
    int_vec.set_sort_strategy(new int_vec_strategy::sort_desc{int_vec.data()});
    int_vec.sort();
    result = int_vec.data();
    std::cout << "int_vec = {";
    for (std::size_t i = 0; i < result.size(); ++i)
    {
        if (i < result.size() - 1)
        {
            std::cout << result[i] << ',' << ' ';
        }
        else
        {
            std::cout << result[i];
        }
    }
    std::cout << '}' << std::endl;
    return 0;
}
