// --------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2023, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2023, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/raptor/blob/main/LICENSE.md
// --------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides raptor::prepare_arguments.
 * \author Enrico Seiler <enrico.seiler AT fu-berlin.de>
 */

#pragma once

#include <filesystem>
#include <vector>

#include <seqan3/search/kmer_index/shape.hpp>

#include <raptor/argument_parsing/memory_usage.hpp>
#include <raptor/argument_parsing/timer.hpp>
#include <raptor/strong_types.hpp>

namespace raptor
{

struct prepare_arguments
{
    uint8_t kmer_size{20u};
    uint32_t window_size{kmer_size};
    std::string shape_string{};
    seqan3::shape shape{seqan3::ungapped{kmer_size}};
    bool use_filesize_dependent_cutoff{false};
    uint8_t kmer_count_cutoff{1u};

    std::filesystem::path out_dir{"./"};

    std::vector<std::vector<std::string>> bin_path{};
    std::filesystem::path bin_file{};
    uint8_t threads{1u};
    bool verbose{false};

    // Timers do not copy the stored duration upon copy construction/assignment
    mutable timer<concurrent::yes> wall_clock_timer{};
    mutable timer<concurrent::yes> compute_minimiser_timer{};
    mutable timer<concurrent::yes> write_minimiser_timer{};
    mutable timer<concurrent::yes> write_header_timer{};

    void print_timings() const
    {
        if (!verbose)
            return;
        std::cerr << std::fixed << std::setprecision(2) << "============= Timings =============\n";
        std::cerr << "Wall clock time [s]: " << wall_clock_timer.in_seconds() << '\n';
        std::cerr << "Peak memory usage " << formatted_peak_ram() << '\n';
        std::cerr << "Compute minimiser [s]: " << compute_minimiser_timer.in_seconds() / threads << '\n';
        std::cerr << "Write minimiser files [s]: " << write_minimiser_timer.in_seconds() / threads << '\n';
        std::cerr << "Write header files [s]: " << write_header_timer.in_seconds() / threads << '\n';
    }
};

} // namespace raptor
