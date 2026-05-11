#include <sstream>
#include <algorithm>
#include <filesystem>
#include <stdexcept>

#include "include/MTapeSorter.hpp"

MTapeSorter::MTapeSorter(
    std::size_t max_buffer_size,
    const std::string& temp_dir
)
    : max_buffer_size_{max_buffer_size}
    , temp_dir_{temp_dir}
    , temps_created_{0}
{
    if (max_buffer_size_ == 0) {
        throw std::invalid_argument{"max buffer size must be positive"};
    }

    if (!std::filesystem::exists(temp_dir_)) {
        std::filesystem::create_directory(temp_dir_);
    }
}

void MTapeSorter::Sort(MTape& input, MTape& output) {
    std::stringstream ss;
    ss << temp_dir_ << '/' << temps_created_ << ".txt";

    MTape temporary{
        ss.str(),
        input.read_delay(),
        input.write_delay(),
        input.move_delay(),
        std::ios::in | std::ios::out | std::ios::trunc
    };

    MTape* accumulated = &output;
    MTape* destination = &temporary;

    QueueBuffer buffer;

    auto input_it = input.begin();
    auto input_end = input.end();
    
    bool has_accumulated{false};

    output.Clear();

    while (input_it != input_end) {
        std::size_t read_size = std::min<std::size_t>(
            max_buffer_size_,
            input_end - input_it
        );

        for (std::size_t i = 0; i < read_size; ++i) {
            buffer.push(*input_it);
            ++input_it;
        }

        if (!has_accumulated) {
            while (!buffer.empty()) {
                accumulated->push_back(buffer.top());
                buffer.pop();
            }
            has_accumulated = true;
        } else {
            Merge(*destination, *accumulated, buffer);
            std::swap(accumulated, destination);
        }
    }

    if (accumulated != &output) {
        Copy(output, *accumulated);
    }
}

void MTapeSorter::Merge(MTape& dest, MTape& temp, QueueBuffer& buffer) {
    dest.Clear();

    auto temp_it = temp.begin();
    auto temp_end = temp.end();

    while (temp_it != temp_end && !buffer.empty()) {
        int temp_v = *temp_it;
        int buffer_v = buffer.top();

        if (temp_v < buffer_v) {
            dest.push_back(temp_v);
            ++temp_it;
        } else {
            dest.push_back(buffer_v);
            buffer.pop();
        }
    }

    while (temp_it != temp_end) {
        dest.push_back(*temp_it);
        ++temp_it;
    }

    while (!buffer.empty()) {
        dest.push_back(buffer.top());
        buffer.pop();
    }
}

void MTapeSorter::Copy(MTape& dest, MTape& src) {
    dest.Clear();

    for (int value : src) {
        dest.push_back(value);
    }
}

void MTapeSorter::WriteTemp(QueueBuffer& queue) {
    std::stringstream ss;
    ss << temp_dir_ << '/' << temps_created_ << ".txt";

    MTape temp_tape{ss.str(), std::ios::in | std::ios::out | std::ios::trunc};

    while (!queue.empty()) {
        temp_tape.push_back(queue.top());
        queue.pop();
    }

    ++temps_created_;
}
