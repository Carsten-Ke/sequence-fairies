#!/usr/bin/env bats

@test "simple_concatenation" {
    run ../../build/concatenator -i data/concatenator/aln1.fa data/concatenator/aln2.fa -o concat_aln.fasta
    [ $status == 0 ]
	
	run diff concat_aln.fasta data/concatenator/concat_aln_result.fasta
	[ $status == 0 ]
	
	rm concat_aln.fasta
}

@test "simple_directory_concatenation" {
    run ../../build/concatenator -d data/concatenator -o concat_aln.fasta
    [ $status == 0 ]

        run diff concat_aln.fasta data/concatenator/concat_aln_result.fasta
        [ $status == 0 ]

        rm concat_aln.fasta
}

