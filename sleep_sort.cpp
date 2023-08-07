/*
    Will this be practical?  No, but seemed fun to try :)
*/
#include <thread>
#include <random>
#include <vector>
#include <span>
#include <cstdlib>
#include <chrono>
#include <iostream>

/*! Sleep sorts the given input span

    An algorithm that should *NEVER* be used in production code,
    this is a fun thought exercise on a O(1) sort routine.
*/
std::vector<uint16_t>   sleep_sort(std::span<const uint16_t> input)
{
    using namespace std::chrono_literals;
    
    if(input.empty())
        return {};

    std::vector<std::thread>    threads;
    std::vector<uint16_t>       output;
    
    threads.reserve(input.size());
    output.reserve(input.size());
    
    for(uint16_t i : input){
        threads.push_back(std::thread([&output,i](){
            std::this_thread::sleep_for(i * 1s);
            output.push_back(i);
        }));
    }
    
    for(auto& t : threads)
        t.join();
    return output;
}

std::vector<uint16_t>   make_random_data(uint16_t ulimit, unsigned int count)
{
    std::vector<uint16_t>                   data;
    std::random_device                      rd;
    std::uniform_int_distribution<uint16_t> dist(0,ulimit);
    
    data.reserve(count);
    for(unsigned int n=0;n<count;++n)
        data.push_back(dist(rd));
    return data;
}

std::ostream&   operator<<(std::ostream&out, const std::vector<uint16_t>& data)
{
    out << '[';
    bool s = false;
    for(uint16_t i : data){
        if(s)
            out << ',';
        s = true;
        out << i;
    }
    out << ']';
    return out;
}

int main(int argc, char* argv[])
{
    unsigned int    cnt = (argc>1) ? atoi(argv[1]) : 10;
    uint16_t        mx  = (argc>2) ? atoi(argv[2]) : 60;
    
    std::vector<uint16_t>   unsorted = make_random_data(mx, cnt);
    
    std::cout << "unsorted> " << unsorted << '\n';
    
    auto start  = std::chrono::steady_clock::now();
    std::vector<uint16_t>   sorted  = sleep_sort(unsorted);
    auto end    = std::chrono::steady_clock::now();
    
    auto seconds    = std::chrono::duration<double>(end-start).count();
    std::cout << "sorted> " << sorted << '\n' 
    << "took " << seconds << " seconds.\n";

    return 0;
}

