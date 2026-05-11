#include <iostream>
#include <chrono>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <string>

#include <MTape.hpp>
#include <MTapeSorter.hpp>

struct Config {
    std::chrono::milliseconds read_delay;
    std::chrono::milliseconds write_delay;
    std::chrono::milliseconds move_delay;
};

bool ParseEnv(Config& config) {
    auto parse_delay = [](const char* env_name) {
        const char* value = std::getenv(env_name);

        if (value == nullptr) {
            return std::chrono::milliseconds{0};
        }

        std::string text{value};
        std::size_t parsed_chars{0};
        long long delay{};

        try {
            delay = std::stoll(text, &parsed_chars);
        } catch (const std::exception&) {
            throw std::invalid_argument{
                std::string{"invalid integer value for "} + env_name
            };
        }

        if (parsed_chars != text.size()) {
            throw std::invalid_argument{
                std::string{"invalid integer value for "} + env_name
            };
        }

        if (delay < 0) {
            throw std::invalid_argument{
                std::string{"negative delay for "} + env_name
            };
        }

        if (delay > std::numeric_limits<std::chrono::milliseconds::rep>::max()) {
            throw std::out_of_range{
                std::string{"delay is too large for "} + env_name
            };
        }

        return std::chrono::milliseconds{delay};
    };

    try {
        config.read_delay = parse_delay("MTAPE_READ_DELAY");
        config.write_delay = parse_delay("MTAPE_WRITE_DELAY");
        config.move_delay = parse_delay("MTAPE_MOVE_DELAY");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: mtape [input tape filename] [output tape filename]" << std::endl;

        return 1;
    }

    const char* input_filename = argv[1];
    const char* output_filename = argv[2];

    Config config;

    if (!ParseEnv(config)) {
        return 1;
    }

    MTape mtape{
        input_filename,
        config.read_delay,
        config.write_delay,
        config.move_delay,
        std::ios::in | std::ios::out
    };
    
    MTape out_mtape{
        output_filename,
        config.read_delay,
        config.write_delay,
        config.move_delay,
        std::ios::in | std::ios::out | std::ios::trunc
    };

    MTapeSorter sorter{103, "temp"};

    sorter.Sort(mtape, out_mtape);

    return 0;
}
