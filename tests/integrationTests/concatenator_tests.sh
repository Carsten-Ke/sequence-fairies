#!/usr/bin/env bats

@test "simple_concatenation" {
    run ../../build/concatenator -i data/concatenator/aln1.fa data/concatenator/aln2.fa
    [ ${lines[0]} = ">A" ]
    [ ${lines[1]} = "aaaAAA" ]
    [ ${lines[2]} = ">C" ]
    [ ${lines[3]} = "cccCCC" ]
    [ $status == 0 ]
}

@test "simple_directory_concatenation" {
    run ../../build/concatenator -d data/concatenator -o concat_aln.fasta
    [ $status == 0 ]

    run -0 diff concat_aln.fasta data/concatenator/concat_aln_result.fasta

    rm concat_aln.fasta
}

@test "simple messages and errors" {
    run -0 ../../build/concatenator -h


    run ../../build/concatenator --doesnotexist
    [[ ${lines[0]} = "An error occurred parsing the command line: unrecognised option '--doesnotexist'" ]]
	[[ ${lines[1]} = "Please use -h/--help for more information." ]]
    [ $status == 1 ]

    run ../../build/concatenator -i notexisting1 notexisting2
    [ "$output" = "Error opening file 'notexisting1': No such file or directory: iostream error" ]

    run ../../build/concatenator -i data/format.txt
    [ "$output" = "Error: Format of file 'data/format.txt' could not be identified or is not supported." ]
}

