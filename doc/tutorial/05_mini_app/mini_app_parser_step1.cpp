#include <raptor/argument_parsing/parse_bin_path.hpp>

#include <sharg/parser.hpp>                     // for the SeqAn sharg::parser

#include <seqan3/core/debug_stream.hpp>

void run_program(std::filesystem::path const & reference_path, std::filesystem::path const & index_path)
{
    seqan3::debug_stream << "reference_file_path: " << reference_path << '\n';
    seqan3::debug_stream << "index_path           " << index_path << '\n';
}

struct build_arguments
{
    // Related to k-mers
    // uint8_t kmer_size{20u};
    // uint32_t window_size{kmer_size};
    // std::string shape_string{};
    // seqan3::shape shape{seqan3::ungapped{kmer_size}};
    // bool compute_minimiser{false};
    // bool disable_cutoffs{false};

    // Related to IBF
    std::filesystem::path out_path{"./"};
    // std::string size{"1k"};
    uint64_t bins{64};
    // uint64_t bits{4096};
    // uint64_t hash{2};
    // uint8_t parts{1u};
    // double fpr{0.05};
    // bool compressed{false};

    // General arguments
    std::vector<std::vector<std::string>> bin_path{};
    std::filesystem::path bin_file{};
    // uint8_t threads{1u};
    // bool is_socks{false};
    // bool is_hibf{false};
    // bool is_minimiser{false};
};


void initialise_argument_parser(sharg::parser & parser, build_arguments & arguments)
{
    parser.info.author = "YOUR NAME";
    parser.info.app_name = "Raptor Mini App";
    parser.info.short_description = "Creates a space-efficient pre-filter.";
    parser.info.version = "1.0.0";

    // Options - Input / Output:
    parser.add_subsection("Input / Output");
    parser.add_option(arguments.bin_file, sharg::config{
                      .short_id = 'i', .long_id = "input",
                      .description = "File containing file names.",
                      .validator = sharg::input_file_validator{} });

    parser.add_option(arguments.out_path, sharg::config{
                      .short_id = 'o', .long_id = "output",
                      .description = "The path of the index output file.",
                      .validator = sharg::output_file_validator{sharg::output_file_open_options::open_or_create, {}}});

    // Options - Other parameters:
    parser.add_subsection("Other parameters");
    // parser.add_option(arguments.kmer_size,
    //                   sharg::config{.short_id = '\0',
    //                                 .long_id = "kmer",
    //                                 .description = "The k-mer size.",
    //                                 .validator = sharg::arithmetic_range_validator{1, 32}});
    // parser.add_option(arguments.window_size,
    //                   sharg::config{.short_id = '\0',
    //                                 .long_id = "window",
    //                                 .description = "The window size.",
    //                                 .default_message = "k-mer size",
    //                                 .hidden = arguments.is_socks,
    //                                 .validator = positive_integer_validator{}});
}

//![main]
int main(int argc, char const ** argv)
{
    sharg::parser parser("Mini-App", argc, argv);
    build_arguments arguments{};

    initialise_argument_parser(parser, arguments);

    // Parse the given arguments and catch possible errors.
    try
    {
        parser.parse();                                             // trigger command line parsing
    }
    catch (sharg::parser_error const & ext)                         // catch user errors
    {
        seqan3::debug_stream << "[Error] " << ext.what() << '\n';   // customise your error message
        return -1;
    }

    // Here you could do further checks for given parameters.

    // This raptor function parses the given paths into a vector of paths.
    parse_bin_path(arguments);
    arguments.bins = arguments.bin_path.size();

    run_program(arguments.reference_path, arguments.index_path);

    return 0;
}
//![main]
