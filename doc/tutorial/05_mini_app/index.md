# Implementing your own space-efficient pre-filter with SeqAn {#tutorial_pre-filter}

<b>Learning Objective:</b><br>
In this tutorial, you will learn how to implement your own Mini-App with SeqAn2. You will create your own Mini-Raptor,
that demonstrates the exchange of the counting lemma with other decisions using the minimizer counts.

\tutorial_head{High, 90 Minutes, [<b>Interleaved Bloom Filter (IBF)</b>](https://docs.seqan.de/seqan/3-master-user/classseqan3_1_1interleaved__bloom__filter.html#details)\,}


[TOC]

---

# Introduction

For this SeqAn application you will write your own argument parser using the SeqAn parser Sharg. Create your own index
using an *Interleaved Bloom Filter (IBF)* and finally implement a simple search.
We decided to use an IBF because the HIBF is not yet API stable and the basic process is the same. To keep the app
simple, we will also include some functionalities from the Raptor app.

# Agenda

* Implementing a parser
    * Parse arguments
    * Read input files
* Implementing an indexer
    * Create and store index
* Implementing a search
    * Write final results into a search file

# The data

We provide an example [toy data set](https://ftp.imp.fu-berlin.de/pub/seiler/raptor/) (124 MiB compressed,
983 MiB decompressed).

```bash
wget https://ftp.imp.fu-berlin.de/pub/seiler/raptor/example_data.tar.gz
tar xfz example_data.tar.gz
```

After extraction, the `example_data` will look like:

```console
$ tree -L 2 example_data
example_data
├── 1024
│   ├── bins
│   └── reads
└── 64
    ├── bins
    └── reads
```

# The parser

## Step 1 - Parsing arguments

As a first step, we want to parse command line arguments for our index and search. In order not to blow up this tutorial
too much, we give you the parser code. For more detailed explanations there is an extra tutorial for the
[SeqAn Sharg parser](https://docs.seqan.de/sharg/main_user/tutorial_parser.html).

\include doc/tutorial/05_mini_app/mini_app_parser_step1.cpp










\assignment{Assignment 1: Parsing arguments}
Let's start our application by setting up the argument parser with the following options:
* The path to the reference file
* An output path for the index

Follow the best practice and create:
* A function `run_program` that prints the parsed arguments
* A struct `cmd_arguments` that stores the arguments
* A function `initialise_argument_parser` to add meta data and options to the parser
* A `main` function that parses the arguments and calls `run_program`

Use validators where applicable!

Your `main` may look like this:
\hint
\snippet doc/tutorial/05_mini_app/mini_app_parser_step1.cpp main
\endhint
\endassignment
\solution
\include doc/tutorial/05_mini_app/mini_app_parser_step1.cpp
\endsolution







<!--

# The indexer

## Step 1 - Parsing arguments
As a first step, we want to parse command line arguments for our indexer. If you get into trouble,
you can take a peek at the \ref tutorial_argument_parser "Argument Parser Tutorial" or the API documentation of
the seqan3::argument_parser for help.

\assignment{Assignment 1: Parsing arguments}
Let's start our application by setting up the argument parser with the following options:
* The path to the reference file
* An output path for the index

Follow the best practice and create:
* A function `run_program` that prints the parsed arguments
* A struct `cmd_arguments` that stores the arguments
* A function `initialise_argument_parser` to add meta data and options to the parser
* A `main` function that parses the arguments and calls `run_program`

Use validators where applicable!

Your `main` may look like this:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_indexer_step1.cpp main
\endhint
\endassignment
\solution
\include doc/tutorial/11_read_mapper/read_mapper_indexer_step1.cpp
\endsolution

## Step 2 - Reading the input
As a next step, we want to use the parsed file name to read in our reference data. This will be done
using seqan3::sequence_file_input class. As a guide, you can take a look at the
\ref tutorial_sequence_file "Sequence I/O Tutorial".

\assignment{Assignment 2: Reading the input}
Extend your program to store the sequence information contained in the reference file into a struct.

To do this, you should create:
* A struct `reference_storage_t` that stores the sequence information for both reference and query information
  within member variables
* A function `read_reference` that fills a `reference_storage_t` object with information from the files and
  prints the reference IDs

You should also perform the following changes in `run_program`:
* Construct of an object `storage` of type `reference_storage_t`
* Add a call to `read_reference`

This is the signature of `read_reference`:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_indexer_step2.cpp read_reference
\endhint

This is the `reference_storage_t`:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_indexer_step2.cpp reference_storage_t
\endhint
\endassignment
\solution
\snippet doc/tutorial/11_read_mapper/read_mapper_indexer_step2.cpp solution
Here is the complete program:
\hint
\include doc/tutorial/11_read_mapper/read_mapper_indexer_step2.cpp
\endhint
\endsolution

## Step 3 - Index
Now that we have the necessary sequence information, we can create an index and store it. Read up on the
\ref tutorial_index_search "Index Tutorial" if you have any questions.

\assignment{Assignment 3: Index}
We want to create a new function `create_index`:
* It takes `index_path` and `storage` as parameters
* Creates a bi_fm_index
* Stores the bi_fm_index

We also need to change:
* `run_program` to now also call `create_index`
* `run_program` and `read_reference` to not print the debug output anymore

This is the signature of `create_index`:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_indexer_step3.cpp create_index
\endhint
\endassignment
\solution
\snippet doc/tutorial/11_read_mapper/read_mapper_indexer_step3.cpp solution
Here is the complete program:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_indexer_step3.cpp complete
\endhint
\endsolution

# The read mapper

## Step 1 - Parsing arguments
Again, we want to parse command line arguments for our read mapper as a first step. If you get into trouble,
you can take a peek at the [Argumet Parser Tutorial](#tutorial_argument_parser) or the API documentation of
the seqan3::argument_parser for help.

\assignment{Assignment 4: Parsing arguments}
Let's start our application by setting up the argument parser with the following options:
* The path to the reference file
* The path to the query file
* The path to the index file
* An output path
* The maximum number of errors we want to allow (between 0 and 4)

Follow the best practice and create:
* A function `run_program` that prints the parsed arguments
* A struct `cmd_arguments` that stores the arguments
* A function `initialise_argument_parser` to add meta data and options to the parser
* A `main` function that parses the arguments and calls `run_program`

Use validators where applicable!

Your `main` may look like this:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_step1.cpp main
\endhint
\endassignment
\solution
\include doc/tutorial/11_read_mapper/read_mapper_step1.cpp
\endsolution

## Step 2 - Reading the input and searching
We also want to read the reference in the read mapper. This is done the same way as for the indexer.
We can now load the index and conduct a search. Read up on the \ref tutorial_index_search "Search Tutorial"
if you have any questions.

\assignment{Assignment 5: Reading the input}
Extend your program to read the reference file the same way the indexer does.
After this, you can load the index and print results of a search.

To do this, you should:
* Carry over the `read_reference` function and the `reference_storage_t` struct from the indexer
* Create a function `map_reads` that loads the index and prints the results of the search (allowing all error types)
  for the first 20 queries

You should also perform the following changes in `run_program`:
* Remove the debug output
* Construct an object `storage` of type `reference_storage_t`
* Add a call to `read_reference` and `map_reads`

This is the signature of `map_reads`:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_step2.cpp map_reads
\endhint
\endassignment
\solution
\snippet doc/tutorial/11_read_mapper/read_mapper_step2.cpp solution
Here is the complete program:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_step2.cpp complete
\endhint
\endsolution

## Step 3 - Alignment
We can now use the obtained positions to align each query against the reference. Refer to the
\ref tutorial_pairwise_alignment "Alignment Tutorial" if you have any questions.

\assignment{Assignment 6: Alignment}
We want to extend `map_reads` to:
* Use the output of the search to align the query against the reference
* Print the query ID, alignment score, subrange of the reference sequence and the query (for the first 20 queries)

This is the alignment config:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_step3.cpp alignment_config
\endhint
\endassignment
\solution
\snippet doc/tutorial/11_read_mapper/read_mapper_step3.cpp solution
Here is the complete program:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_step3.cpp complete
\endhint
\endsolution

## Step 4 - Alignment output
Finally, we can write our results into a SAM file.

\assignment{Assignment 7: SAM out}
We further need to extend `map_reads` to write the alignment results into a SAM file.
Additionally, there should be no more debug output.

Try to write all available information into the SAM file. We can introduce a naive mapping quality by using
`mapping quality = 60 + alignment score`.

This is the sam_file_output construction:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_step4.cpp sam_file_output
\endhint
\endassignment
\solution
\snippet doc/tutorial/11_read_mapper/read_mapper_step4.cpp solution
Here is the complete program:
\hint
\snippet doc/tutorial/11_read_mapper/read_mapper_step4.cpp complete
\endhint
\endsolution -->
