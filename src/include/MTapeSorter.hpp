#ifndef MTAPE_SORTER_HPP_
#define MTAPE_SORTER_HPP_

#include <cstddef>
#include <queue>
#include <string>
#include <vector>

#include "MTape.hpp"

class MTapeSorter {
public:
    using QueueBuffer = std::priority_queue<int, std::vector<int>, std::greater<int>>;

    MTapeSorter(
        std::size_t max_buffer_size,
        const std::string& temp_dir
    );

    void Sort(MTape& input, MTape& output);

private:
    void Merge(MTape& dest, MTape& temp, QueueBuffer& buffer);

    void Copy(MTape& dest, MTape& src);

    void WriteTemp(QueueBuffer& queue);

    std::size_t max_buffer_size_;
    std::string temp_dir_;
    std::size_t temps_created_;
};

#endif // MTAPE_SORTER_HPP_
