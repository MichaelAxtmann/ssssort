/*******************************************************************************
 * sort_strings.cpp
 *
 * Test runner for strings
 *
 *******************************************************************************
 * Copyright (C) 2016 Lorenz Hübschle-Schneider <lorenz@4z2.de>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/


#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "benchmark.h"

int main(int argc, char *argv[]) {
    size_t outer_its = 5, inner_its = 3;
    if (argc > 1) outer_its = static_cast<size_t>(atol(argv[1]));
    if (argc > 2) inner_its = static_cast<size_t>(atol(argv[2]));

    std::string input_file = "input.txt";
    if (argc > 3) input_file = std::string{argv[3]};
    // Read input file
    std::ifstream input(input_file);
    std::vector<std::string> lines;
    std::string line;
    while (getline(input, line)) {
        if (line.empty()) continue;
        lines.emplace_back(line);
    }

    std::string stat_file = "stats_strings.txt";
    if (argc > 3) stat_file = std::string{argv[3]};
    std::ofstream *stat_stream = nullptr;
    if (stat_file != "-") {
        stat_stream = new std::ofstream;
        stat_stream->open(stat_file);
    }

    using data_t = std::string;

    sized_benchmark_generator<data_t>([&lines](auto data, size_t size){
            size_t num_lines = std::min(size, lines.size());
            std::copy(lines.cbegin(), lines.cbegin() + num_lines, data);
            return num_lines;
        }, "file", outer_its, inner_its, stat_stream, true);

/*
    benchmark_generator<data_t>([](auto data, size_t size){
            using T = std::remove_reference_t<decltype(*data)>;
            std::mt19937 rng{ std::random_device{}() };
            for (size_t i = 0; i < size; ++i) {
                data[i] = "";//static_cast<T>(rng());
            }
        }, "random", iterations, stat_stream);
*/


    if (stat_stream != nullptr) {
        stat_stream->close();
        delete stat_stream;
    }
}
