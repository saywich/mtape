#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <MTape.hpp>
#include <MTapeSorter.hpp>

namespace {

std::filesystem::path MakeCaseDir(const std::string& test_name) {
    const auto dir = std::filesystem::temp_directory_path()
        / "yadrotape-tests"
        / test_name;

    std::filesystem::remove_all(dir);
    std::filesystem::create_directories(dir / "temp");

    return dir;
}

void WriteTape(MTape& tape, const std::vector<int>& values) {
    for (int value : values) {
        tape.push_back(value);
    }
}

std::vector<int> ReadTape(MTape& tape) {
    std::vector<int> values;

    for (int value : tape) {
        values.push_back(value);
    }

    return values;
}

void ExpectSortedOutput(
    const std::string& test_name,
    const std::vector<int>& input_values,
    std::size_t max_buffer_size
) {
    const auto dir = MakeCaseDir(test_name);
    const auto input_path = dir / "input.bin";
    const auto output_path = dir / "output.bin";
    const auto temp_dir = dir / "temp";

    MTape input{
        input_path.string(),
        std::ios::in | std::ios::out | std::ios::trunc
    };
    MTape output{
        output_path.string(),
        std::ios::in | std::ios::out | std::ios::trunc
    };

    WriteTape(input, input_values);

    MTapeSorter sorter{max_buffer_size, temp_dir.string()};
    sorter.Sort(input, output);

    std::vector<int> expected = input_values;
    std::sort(expected.begin(), expected.end());

    EXPECT_EQ(ReadTape(output), expected);
    EXPECT_EQ(input.size(), input_values.size());
    EXPECT_EQ(output.size(), expected.size());
}

} // namespace

TEST(MTapeSorterTests, SortsEmptyTape) {
    ExpectSortedOutput("sorts-empty-tape", {}, 3);
}

TEST(MTapeSorterTests, SortsSingleChunk) {
    ExpectSortedOutput("sorts-single-chunk", {5, -1, 3, 3, 0}, 16);
}

TEST(MTapeSorterTests, SortsAcrossMultipleChunks) {
    ExpectSortedOutput(
        "sorts-across-multiple-chunks",
        {8, 1, 7, 2, 6, 3, 5, 4, 0, 9, -2, -2, 11},
        4
    );
}

TEST(MTapeSorterTests, SortsWhenChunkSizeIsOne) {
    ExpectSortedOutput(
        "sorts-when-chunk-size-is-one",
        {4, 1, 3, 2, 0, -1},
        1
    );
}

TEST(MTapeSorterTests, OverwritesExistingOutput) {
    const auto dir = MakeCaseDir("overwrites-existing-output");
    const auto input_path = dir / "input.bin";
    const auto output_path = dir / "output.bin";
    const auto temp_dir = dir / "temp";

    MTape input{
        input_path.string(),
        std::ios::in | std::ios::out | std::ios::trunc
    };
    MTape output{
        output_path.string(),
        std::ios::in | std::ios::out | std::ios::trunc
    };

    WriteTape(input, {3, 1, 2});
    WriteTape(output, {100, 200, 300, 400});

    MTapeSorter sorter{2, temp_dir.string()};
    sorter.Sort(input, output);

    EXPECT_EQ(ReadTape(output), std::vector<int>({1, 2, 3}));
    EXPECT_EQ(output.size(), 3U);
}

TEST(MTapeSorterTests, RejectsZeroBufferSize) {
    const auto dir = MakeCaseDir("rejects-zero-buffer-size");

    EXPECT_THROW(
        static_cast<void>(MTapeSorter(0, (dir / "temp").string())),
        std::invalid_argument
    );
}
