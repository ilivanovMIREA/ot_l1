#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include "../bin/version.h"

#include <algorithm>    // std::copy_if, std::distance

using ip_v = std::vector<std::string>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]


auto split(const std::string &str, char d)
{
    ip_v r;

    auto start = 0;
    auto stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void ip_print_int(std::vector<std::vector<int>> &ip_filter){
     for(auto ip = 0; ip < ip_filter.size(); ip++){
        for(std::vector<int>::const_iterator ip_part = ip_filter[ip].cbegin(); ip_part != ip_filter[ip].cend(); ++ip_part)
        {
            if (ip_part != ip_filter[ip].cbegin())
            {
                std::cout << ".";
            }
            std::cout << *ip_part;
        }
        std::cout << std::endl;
    }
}


template<typename T, typename ... Args>
auto filter(T &ip_pool, Args ... args)
{   
    std::vector<int> ab  = { args... };
    bool moreone = false;

    if(sizeof...(args) > 1){
        moreone = true;
    }
    
    std::vector<std::vector<int>> ip_filter(ip_pool.size());
    

    if(moreone == true){
        int a = ab[0], b = ab[1];
        auto it = std::copy_if (ip_pool.begin(), ip_pool.end(), ip_filter.begin(), 
            [a,b](std::vector<int> ip){

                int myints[] = {a,b,0,0};
                std::pair<std::vector<int>::iterator,int*> mypair;
                mypair = std::mismatch (ip.begin(), ip.end(), myints);
                return *mypair.second == 0;

            });
        
        ip_filter.resize(std::distance(ip_filter.begin(),it));  // shrink container to new size
    }
    else
    {
        int a = ab[0];
        auto it = std::copy_if (ip_pool.begin(), ip_pool.end(), ip_filter.begin(), 
            [a](std::vector<int> ip){
                int myints[] = {a,0,0,0};
                std::pair<std::vector<int>::iterator,int*> mypair;
                mypair = std::mismatch (ip.begin(), ip.end(), myints);
                return *mypair.second !=a;
            });
        
        ip_filter.resize(std::distance(ip_filter.begin(),it));
    };

    return ip_filter;
}

auto filter_any(std::vector<std::vector<int>> &ip_pool, int a){
    std::vector<std::vector<int>> ip_filter(ip_pool.size());
    auto it = std::copy_if (ip_pool.begin(), ip_pool.end(), ip_filter.begin(), [a](std::vector<int> ip){return std::any_of((ip).cbegin(), (ip).cend(), [a](int i){return i == a;});});
    ip_filter.resize(std::distance(ip_filter.begin(),it));
    return ip_filter;
}



int main(int argc, char const *argv[])
{
    try
    {
        std::vector<std::vector<std::string>> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
                ip_v v = split(line, '\t');                
                ip_pool.push_back(split(v.at(0), '.'));
        }
        
        std::vector<std::vector<int>> ip_pool_int;
        
        for(auto i=ip_pool.begin(); i!=ip_pool.end(); ++i){
            std::vector<int> temp_int;
            temp_int.push_back(std::stoi((*i)[0],nullptr,10));
            temp_int.push_back(std::stoi((*i)[1],nullptr,10));
            temp_int.push_back(std::stoi((*i)[2],nullptr,10));
            temp_int.push_back(std::stoi((*i)[3],nullptr,10));
            ip_pool_int.push_back(temp_int);    
        }        
        // TODO reverse lexicographically sort
        
        std::sort(begin(ip_pool_int), end(ip_pool_int), std::greater<std::vector<int>>());
        ip_print_int(ip_pool_int);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        
        std::vector<std::vector<int>> ip_filter1 = filter(ip_pool_int, 1);
        ip_print_int(ip_filter1);

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        std::vector<std::vector<int>> ip_filter2 = filter(ip_pool_int, 46, 70);
        ip_print_int(ip_filter2);

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        std::vector<std::vector<int>> ip_filter3 = filter_any(ip_pool_int, 46);
        ip_print_int(ip_filter3);


        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
