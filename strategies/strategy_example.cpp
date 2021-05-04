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
    class sort_asc : public strategy {
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
    class sort_desc : public strategy {
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
        : _int_vec(std::vector<int>{}), _sort_asc(nullptr), _sort_desc(nullptr) {}
    int_vector(const std::vector<int>& input)
        : _int_vec(input), _sort_asc(nullptr), _sort_desc(nullptr)
    {
        _sort_asc = new int_vec_strategy::sort_asc{input};
        _sort_desc = new int_vec_strategy::sort_desc{input};
    }
    ~int_vector()
    {
        if (_sort_asc)
        {
            delete _sort_asc;
            _sort_asc = nullptr;
        }
        if (_sort_desc)
        {
            delete _sort_desc;
            _sort_desc = nullptr;
        }
    }
    void sort_asc() 
    {
        if (_sort_asc) 
        {
            _sort_asc->execute();
            _int_vec = _sort_asc->data();
        }
    }
    void sort_desc()
    {
        if (_sort_desc)
        {
            _sort_desc->execute();
            _int_vec = _sort_desc->data();
        }
    }
    std::vector<int> data() { return _int_vec; }
private:
    int_vec_strategy::sort_asc* _sort_asc;
    int_vec_strategy::sort_desc* _sort_desc;
    std::vector<int> _int_vec;
};

int main(void)
{
    int_vector int_vec{ std::vector<int> {10, 1, 9, 2, 8, 3, 7, 4, 6, 5 } };
    std::vector<int> result = int_vec.data();
    std::cout << "executing no strategy..." << std::endl;
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

    std::cout << "executing sort_asc strategy..." << std::endl;
    int_vec.sort_asc();
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
    int_vec.sort_desc();
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
