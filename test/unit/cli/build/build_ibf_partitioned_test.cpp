// --------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2023, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2023, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/raptor/blob/main/LICENSE.md
// --------------------------------------------------------------------------------------------------

#include <raptor/test/cli_test.hpp>

struct build_ibf_partitioned :
    public raptor_base,
    public testing::WithParamInterface<std::tuple<size_t, size_t, size_t, size_t, bool>>
{};

TEST_P(build_ibf_partitioned, pipeline)
{
    auto const [number_of_repeated_bins, window_size, number_of_errors, parts, compressed] = GetParam();

    std::stringstream header{};
    { // generate input file
        std::ofstream file{"raptor_cli_test.txt"};
        size_t usr_bin_id{0};
        for (auto && file_path : get_repeated_bins(number_of_repeated_bins))
        {
            header << '#' << usr_bin_id++ << '\t' << file_path << '\n';
            file << file_path << '\n';
        }
        header << "#QUERY_NAME\tUSER_BINS\n";
        file << '\n';
    }

    cli_test_result const result1 = execute_app("raptor",
                                                "build",
                                                "--kmer 19",
                                                "--window ",
                                                std::to_string(window_size),
                                                "--output raptor.index",
                                                compressed ? "--compressed" : "--threads 1",
                                                "--parts ",
                                                std::to_string(parts),
                                                "raptor_cli_test.txt");
    EXPECT_EQ(result1.out, std::string{});
    EXPECT_EQ(result1.err, std::string{});
    RAPTOR_ASSERT_ZERO_EXIT(result1);

    cli_test_result const result2 = execute_app("raptor",
                                                "search",
                                                "--output search.out",
                                                "--error ",
                                                std::to_string(number_of_errors),
                                                "--index ",
                                                "raptor.index",
                                                "--query ",
                                                data("query.fq"));
    EXPECT_EQ(result2.out, std::string{});
    EXPECT_EQ(result2.err, std::string{});
    RAPTOR_ASSERT_ZERO_EXIT(result2);

    compare_search(number_of_repeated_bins, number_of_errors, "search.out");
}

TEST_F(build_ibf_partitioned, pipeline_misc)
{
    std::stringstream header{};
    { // generate input files
        std::ofstream file{"raptor_cli_test.txt"};
        std::ofstream file2{"raptor_cli_test.minimiser"};
        size_t usr_bin_id{0};
        for (auto && file_path : get_repeated_bins(16))
        {
            file << file_path << '\n';
            auto line = seqan3::detail::to_string("precomputed_minimisers/",
                                                  std::filesystem::path{file_path}.stem().c_str(),
                                                  ".minimiser");
            header << '#' << usr_bin_id++ << '\t' << line << '\n';
            file2 << line << '\n';
        }
        header << "#QUERY_NAME\tUSER_BINS\n";
        file << '\n';
    }

    cli_test_result const result1 = execute_app("raptor",
                                                "prepare",
                                                "--kmer 19",
                                                "--window 23",
                                                "--output precomputed_minimisers",
                                                "raptor_cli_test.txt");
    EXPECT_EQ(result1.out, std::string{});
    EXPECT_EQ(result1.err, std::string{});
    RAPTOR_ASSERT_ZERO_EXIT(result1);

    cli_test_result const result2 = execute_app("raptor",
                                                "build",
                                                "--fpr 0.016",
                                                "--output raptor.index",
                                                "--parts 4",
                                                "raptor_cli_test.minimiser");
    EXPECT_EQ(result2.out, std::string{});
    EXPECT_EQ(result2.err, std::string{});
    RAPTOR_ASSERT_ZERO_EXIT(result2);

    cli_test_result const result3 = execute_app("raptor",
                                                "search",
                                                "--output search.out",
                                                "--threshold 0.5",
                                                "--index ",
                                                "raptor.index",
                                                "--query ",
                                                data("query.fq"));
    EXPECT_EQ(result3.out, std::string{});
    EXPECT_EQ(result3.err, std::string{});
    RAPTOR_ASSERT_ZERO_EXIT(result3);

    compare_search(16, 1 /* Always finds everything */, "search.out");

    cli_test_result const result4 = execute_app("raptor",
                                                "search",
                                                "--output search2.out",
                                                "--error 1",
                                                "--index ",
                                                "raptor.index",
                                                "--query ",
                                                data("query_empty.fq"));
    EXPECT_EQ(result4.out, std::string{});
    EXPECT_EQ(result4.err, std::string{});
    RAPTOR_ASSERT_ZERO_EXIT(result4);

    compare_search(16, 1, "search2.out", is_empty::yes);
}

INSTANTIATE_TEST_SUITE_P(build_ibf_partitioned_suite,
                         build_ibf_partitioned,
                         testing::Combine(testing::Values(32),
                                          testing::Values(19, 23),
                                          testing::Values(0, 1),
                                          testing::Values(2, 4, 8),
                                          testing::Values(true, false)),
                         [](testing::TestParamInfo<build_ibf_partitioned::ParamType> const & info)
                         {
                             std::string name = std::to_string(std::max<int>(1, std::get<0>(info.param) * 4)) + "_bins_"
                                              + std::to_string(std::get<1>(info.param)) + "_window_"
                                              + std::to_string(std::get<2>(info.param)) + "_error"
                                              + std::to_string(std::get<3>(info.param)) + "_parts"
                                              + (std::get<4>(info.param) ? "compressed" : "uncompressed");
                             return name;
                         });
