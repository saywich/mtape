#include <filesystem>
#include <stdexcept>
#include <thread>
#include <utility>

#include "include/MTape.hpp"

MTape::MTape(const std::string& file_name, std::ios::openmode flags)
    : MTape(
        file_name,
        std::chrono::milliseconds{0},
        std::chrono::milliseconds{0},
        std::chrono::milliseconds{0},
        flags
    )
{}

MTape::MTape(
    const std::string& file_name,
    std::chrono::milliseconds read_delay,
    std::chrono::milliseconds write_delay,
    std::chrono::milliseconds move_delay,
    std::ios::openmode flags
)
    : size_{0}
    , head_index_{0}
    , file_stream_{}
    , file_name_{file_name}
    , read_delay_{read_delay}
    , write_delay_{write_delay}
    , move_delay_{move_delay}
{
    if (!std::filesystem::exists(file_name)) {
        std::ofstream f{file_name};

        if (!f.is_open()) {
            throw std::runtime_error{"cannot create tape on disk"};
        }

        f.close();
    }

    file_stream_.open(file_name, flags);

    if (!file_stream_.is_open()) {
        throw std::runtime_error{"cannot open tape on disk"};
    }

    file_stream_.seekg(0, std::ios::end);
    size_ = static_cast<std::size_t>(file_stream_.tellg()) / sizeof(int);
    file_stream_.seekg(0, std::ios::beg);
    file_stream_.clear();
}

MTape::MTape(MTape&& other)
    : size_{other.size_}
    , head_index_{other.head_index_}
    , file_stream_{std::move(other.file_stream_)}
    , file_name_{std::move(other.file_name_)}
    , read_delay_{other.read_delay_}
    , write_delay_{other.write_delay_}
    , move_delay_{other.move_delay_}
{
    other.size_ = 0;
    other.head_index_ = 0;
}

MTape::~MTape() {
    file_stream_.close();
}

void MTape::push_back(int value) {
    WriteCell(size_, value);
    ++size_;
}

MTape::iterator MTape::begin() {
    return MTapeIterator{this};
}

MTape::iterator MTape::end() {
    return MTapeIterator{size_, this};
}

std::size_t MTape::size() const {
    return size_;
}

std::chrono::milliseconds MTape::read_delay() const {
    return read_delay_;
}

std::chrono::milliseconds MTape::write_delay() const {
    return write_delay_;
}

std::chrono::milliseconds MTape::move_delay() const {
    return move_delay_;
}

void MTape::Clear() {
    file_stream_.close();
    file_stream_.open(file_name_, std::ios::in | std::ios::out | std::ios::trunc);

    if (!file_stream_.is_open()) {
        throw std::runtime_error{"cannot clear tape on disk"};
    }

    size_ = 0;
    head_index_ = 0;
}

void MTape::MoveTo(std::size_t index) {
    std::size_t distance = index > head_index_
        ? index - head_index_
        : head_index_ - index;

    if (distance != 0) {
        std::this_thread::sleep_for(move_delay_ * distance);
        head_index_ = index;
    }
}

int MTape::ReadCell(std::size_t index) {
    MoveTo(index);
    std::this_thread::sleep_for(read_delay_);

    file_stream_.clear();
    file_stream_.seekg(sizeof(int) * index, std::ios::beg);

    int value{};
    file_stream_.read(reinterpret_cast<char*>(&value), sizeof(int));

    if (!file_stream_) {
        throw std::runtime_error{"cannot read tape cell"};
    }

    return value;
}

void MTape::WriteCell(std::size_t index, int value) {
    MoveTo(index);
    std::this_thread::sleep_for(write_delay_);

    file_stream_.clear();
    file_stream_.seekp(sizeof(int) * index, std::ios::beg);
    file_stream_.write(reinterpret_cast<const char*>(&value), sizeof(int));
    file_stream_.flush();

    if (!file_stream_) {
        throw std::runtime_error{"cannot write tape cell"};
    }
}
