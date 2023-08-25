#!/usr/bin/env bats
bats_require_minimum_version 1.5.0

@test "simple_concatenation" {
    run -0 ../../build/concatenator -i data/concatenator/aln1.fa data/concatenator/aln2.fa
    [ ${lines[0]} = ">A" ]
    [ ${lines[1]} = "aaaAAA" ]
    [ ${lines[2]} = ">C" ]
    [ ${lines[3]} = "cccCCC" ]
}

@test "fill_gaps" {
    run -0 ../../build/concatenator -i data/concatenator/aln1.fa data/concatenator/aln3.fasta -f
    [ ${lines[0]} = ">A" ]
    [ ${lines[1]} = "aaaaaaaaa" ]
    [ ${lines[2]} = ">C" ]
    [ ${lines[3]} = "ccccccccc" ]
    [ ${lines[4]} = ">G" ]
    [ ${lines[5]} = "---gggggg" ]

    run -0 ../../build/concatenator -i data/concatenator/aln3.fasta data/concatenator/aln1.fa -f
    [ ${lines[0]} = ">A" ]
    [ ${lines[1]} = "aaaaaaaaa" ]
    [ ${lines[2]} = ">G" ]
    [ ${lines[3]} = "gggggg---" ]
    [ ${lines[4]} = ">C" ]
    [ ${lines[5]} = "ccccccccc" ]

    run -1 ../../build/concatenator -i data/concatenator/aln3.fasta data/concatenator/aln1.fa
    [[  "$output" = "ERROR! Sequence 'G' not contained." ]]

     run -1 ../../build/concatenator -i data/concatenator/aln1.fa data/concatenator/aln3.fasta
    [[  "$output" = "ERROR! Sequence 'G' not contained." ]]
}

@test "simple_directory_concatenation" {
    run ../../build/concatenator -d data/concatenator -o concat_aln.fasta
    [ $status == 0 ]

    run -0 diff concat_aln.fasta data/concatenator/concat_aln_result.fasta

    rm concat_aln.fasta
}

@test "simple messages and errors" {
    run -0 ../../build/concatenator -h

    run -1 ../../build/concatenator --doesnotexist
    [[ ${lines[0]} = "An error occurred parsing the command line: unrecognised option '--doesnotexist'" ]]
	[[ ${lines[1]} = "Please use -h/--help for more information." ]]

    run -1 ../../build/concatenator -i notexisting1 notexisting2
    [ "$output" = "Error opening file 'notexisting1': No such file or directory: iostream error" ]

    run -1 ../../build/concatenator -i data/format.txt
    [ "$output" = "Error: Format of file 'data/format.txt' could not be identified or is not supported." ]

    run -1 ../../build/concatenator -i data/concatenator/aln1.fa data/concatenator/aln2.fa -o doesnotexist/x.fa
    [ "$output" = "Error opening file 'doesnotexist/x.fa': No such file or directory: iostream error" ]

    run -1 ../../build/concatenator -i data/concatenator/aln1.fa data/concatenator/aln4.fasta
    [ "$output" = "ERROR! Duplicates of sequence 'A' found!" ]
}

