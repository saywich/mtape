#ifndef TAPE_HPP_
#define TAPE_HPP_

#include "MTapeIterator.hpp"

#include <fstream>
#include <string>
#include <chrono>

class MTape {
public:
    using iterator = MTapeIterator;

    MTape(
        const std::string& file_name,
        std::ios::openmode flags = std::ios::in | std::ios::out
    );

    MTape(
        const std::string& file_name,
        std::chrono::milliseconds read_delay,
        std::chrono::milliseconds write_delay,
        std::chrono::milliseconds move_delay,
        std::ios::openmode flags = std::ios::in | std::ios::out
    );

    MTape(MTape&& other);

    ~MTape();

    void push_back(int value);

    iterator begin();

    iterator end();

    std::size_t size() const;

    std::chrono::milliseconds read_delay() const;

    std::chrono::milliseconds write_delay() const;

    std::chrono::milliseconds move_delay() const;

    void Clear();

private:
    friend class MTapeIteratorProxy;

    void MoveTo(std::size_t index);

    int ReadCell(std::size_t index);

    void WriteCell(std::size_t index, int value);

    std::size_t size_;
    std::size_t head_index_;
    std::fstream file_stream_;
    std::string file_name_;
    std::chrono::milliseconds read_delay_;
    std::chrono::milliseconds write_delay_;
    std::chrono::milliseconds move_delay_;
};

#endif // TAPE_HPP_
