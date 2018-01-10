#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include "../bin/version.h"



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

void ip_print(std::vector<ip_v> &ip_filter){
     for(auto ip = 0; ip < ip_filter.size(); ip++){
            for(std::vector<std::string>::const_iterator ip_part = ip_filter[ip].cbegin(); ip_part != ip_filter[ip].cend(); ++ip_part)
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

auto filter(std::vector<ip_v> &ip_pool, int a){
    std::vector<ip_v> ip_filter;
    for(auto ip = 0; ip < ip_pool.size(); ip++){
      if(std::stoi(ip_pool[ip][0],nullptr,10) == a){
          ip_filter.push_back(ip_pool[ip]);              
      }      
    }
    return ip_filter;
}

auto filter(std::vector<ip_v> &ip_pool, int a, int b){
    std::vector<ip_v> ip_filter;
    for(auto ip = 0; ip < ip_pool.size(); ip++){
        if(std::stoi(ip_pool[ip][0],nullptr,10) == a && std::stoi(ip_pool[ip][1],nullptr,10) == b){
            ip_filter.push_back(ip_pool[ip]);              
        }      
    }
    return ip_filter;
}

auto filter_any(std::vector<ip_v> &ip_pool, int a){
    std::vector<ip_v> ip_filter;
    for(auto ip = 0; ip < ip_pool.size(); ip++){
        if(std::stoi(ip_pool[ip][0],nullptr,10) == a || std::stoi(ip_pool[ip][1],nullptr,10) == a || std::stoi(ip_pool[ip][2],nullptr,10) == a || std::stoi(ip_pool[ip][3],nullptr,10) == a){
            ip_filter.push_back(ip_pool[ip]);              
        }      
    }
    return ip_filter;
}

void reverse_sort(std::vector<ip_v> &ip_pool){
    for(auto i = 0; i<4; i++){
        for(auto ip = 0; ip < ip_pool.size()-1; ip++){
          
          int min1 = std::stoi(ip_pool[ip][0],nullptr,10);
          int min2 = std::stoi(ip_pool[ip][1],nullptr,10);
          int min3 = std::stoi(ip_pool[ip][2],nullptr,10);
          int min4 = std::stoi(ip_pool[ip][3],nullptr,10);
          int imin = ip;
          auto ipmin = ip_pool[ip];
          
          for(auto ip_b = ip+1; ip_b < ip_pool.size(); ip_b++){
              if(i == 0 && std::stoi(ip_pool[ip_b][0],nullptr,10) > min1){
                  min1 = std::stoi(ip_pool[ip_b][0],nullptr,10);
                  ipmin = ip_pool[ip_b];
                  imin = ip_b;
              }

              if(i == 1 
                && std::stoi(ip_pool[ip_b][0],nullptr,10) == min1
                && std::stoi(ip_pool[ip_b][1],nullptr,10) > min2){
                  min2 = std::stoi(ip_pool[ip_b][1],nullptr,10);
                  ipmin = ip_pool[ip_b];
                  imin = ip_b;
                }

              if(i == 2 
                && std::stoi(ip_pool[ip_b][0],nullptr,10) == min1
                && std::stoi(ip_pool[ip_b][1],nullptr,10) == min2
                && std::stoi(ip_pool[ip_b][2],nullptr,10) > min3){
                  min3 = std::stoi(ip_pool[ip_b][2],nullptr,10);
                  ipmin = ip_pool[ip_b];
                  imin = ip_b;
                }

              if(i == 3 
                &&std::stoi(ip_pool[ip_b][0],nullptr,10) == min1
                && std::stoi(ip_pool[ip_b][1],nullptr,10) == min2
                && std::stoi(ip_pool[ip_b][2],nullptr,10) == min3
                && std::stoi(ip_pool[ip_b][3],nullptr,10) > min4){
                  min4 = std::stoi(ip_pool[ip_b][3],nullptr,10);
                  ipmin = ip_pool[ip_b];
                  imin = ip_b;
    
              }
          }

          ip_pool[imin] = ip_pool[ip];
          ip_pool[ip] = ipmin;
        }
    }
}

int main(){
  
  try
    {
        std::vector<ip_v> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
                ip_v v = split(line, '\t');
                ip_pool.push_back(split(v.at(0), '.'));
        }
        
        // TODO reverse lexicographically sort
        reverse_sort(ip_pool);  
        ip_print(ip_pool);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        

        std::vector<ip_v> ip_filter1 = filter(ip_pool, 1);
        ip_print(ip_filter1);


        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        std::vector<ip_v> ip_filter2 = filter(ip_pool, 46, 70);
        ip_print(ip_filter2);

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        std::vector<ip_v> ip_filter3 = filter_any(ip_pool, 46);
        ip_print(ip_filter3);


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
