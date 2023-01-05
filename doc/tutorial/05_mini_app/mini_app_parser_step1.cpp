#include <sharg/parser.hpp>                     // for the SeqAn sharg::parser

#include <seqan3/core/debug_stream.hpp>

void run_program(std::filesystem::path const & reference_path, std::filesystem::path const & index_path)
{
    seqan3::debug_stream << "reference_file_path: " << reference_path << '\n';
    seqan3::debug_stream << "index_path           " << index_path << '\n';
}

struct cmd_arguments
{
    std::filesystem::path reference_path{};
    std::filesystem::path index_path{"out.index"};
};

void initialise_argument_parser(sharg::parser & parser, cmd_arguments & args)
{
    parser.info.author = "E. coli";
    parser.info.short_description = "Creates a space-efficient pre-filter.";
    parser.info.version = "1.0.0";

    parser.add_option(args.input_path,
                      'i',
                      "input",
                      "The path to the dataset.",
                      seqan3::option_spec::required,
                      seqan3::input_file_validator{{"fa", "fasta"}});

    parser.add_option(arguments.kmer_size,
                      sharg::config{.short_id = '\0',
                                    .long_id = "kmer",
                                    .description = "The k-mer size.",
                                    .validator = sharg::arithmetic_range_validator{1, 32}});
    parser.add_option(arguments.window_size,
                      sharg::config{.short_id = '\0',
                                    .long_id = "window",
                                    .description = "The window size.",
                                    .default_message = "k-mer size",
                                    .hidden = arguments.is_socks,
                                    .validator = positive_integer_validator{}});


    parser.add_option(
        arguments.out_path,
        sharg::config{.short_id = '\0',
                      .long_id = "output",
                      .description =
                          arguments.is_socks
                              ? "Provide an output filepath."
                              : "Provide an output filepath or an output directory if --compute-minimiser is used.",
                      .required = true});

}

//![main]
int main(int argc, char const ** argv)
{
    sharg::parser parser("Mini-App", argc, argv);
    cmd_arguments args{};

    initialise_argument_parser(parser, args);

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

    run_program(args.reference_path, args.index_path);

    return 0;
}
//![main]
