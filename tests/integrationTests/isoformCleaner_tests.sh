#!/usr/bin/env bats
bats_require_minimum_version 1.5.0

@test "split-cleaning" {
    run -0 ../../build/isoformCleaner -i data/isoformCleaner/splitChar.fasta -o splitChar.fa --summary
	[[ ${lines[0]} = "# seqs in input: 8" ]]
    [[ ${lines[1]} = "# seqs in output: 4" ]]
    [[ ${lines[2]} = "# seqs removed: 4" ]]
    [[ ${lines[3]} = "# warnings: 0" ]]

	run diff splitChar.fa data/isoformCleaner/splitChar_result.fasta
	[ $status == 0 ]
	
	run -0 ../../build/isoformCleaner -i data/isoformCleaner/splitChar_fail.fasta -o splitChar_fail.fa
    [ "$output" == "Warning! Sequence 'seq3' retained because no isoform scheme matched" ]
	
    run diff splitChar_fail.fa data/isoformCleaner/splitChar_fail_result.fasta
	[ $status == 0 ]
	
	rm splitChar_fail.fa
	rm splitChar.fa
}


@test "regex-cleaning" {
    run ../../build/isoformCleaner -i data/isoformCleaner/regex.fasta -o regex.fa -p gene
	[ $status == 0 ]
	
	run diff regex.fa data/isoformCleaner/regex_result.fasta
	[ $status == 0 ]
	
	rm regex.fa

    run ../../build/isoformCleaner -i data/isoformCleaner/regex.fasta -o regex.fa -r "gene[:=]\\s*([\\S]+)[\\s]*"
	[ $status == 0 ]
	
	run diff regex.fa data/isoformCleaner/regex_result.fasta
	[ $status == 0 ]
	
	rm regex.fa
}

@test "gff-cleaning" {
    run ../../build/isoformCleaner -i data/isoformCleaner/gff3.fasta -g data/isoformCleaner/gff3.gff3 -o gff.fa
	[ $status == 0 ]
	
	run diff gff.fa data/isoformCleaner/gff3_result.fasta
	[ $status == 0 ]
	
	rm gff.fa

	run ! ../../build/isoformCleaner -i data/isoformCleaner/gff3.fasta -g data/isoformCleaner/missing.gff3
	 [ "$output" == "Error: GFF file contains entries not found in the sequence file!" ]

}


@test "file-opening-errors" {
    run ../../build/isoformCleaner -i xxx -o splitChar.fa
    [ "$output" == "Error opening file 'xxx': No such file or directory: iostream error" ]
	[ $status != 0 ]

	run ../../build/isoformCleaner -i data/isoformCleaner/regex.fasta -r "gene[:=]\\s*([\\S]+)[\\s]*" -o doesnotexist/x.fa
    [ "$output" = "Error opening file 'doesnotexist/x.fa': No such file or directory: iostream error" ]
}


@test "error messages" {
    run ../../build/isoformCleaner -i data/isoformCleaner/regex.fasta -o regexFail.fa -p flybase
	[[ ${lines[0]} = "Warning! Sequence '1' retained because no isoform scheme matched" ]]
	[[ ${lines[1]} = "Warning! Sequence '2' retained because no isoform scheme matched" ]]
	[[ ${lines[2]} = "Warning! Sequence '3' retained because no isoform scheme matched" ]]
	[[ ${lines[3]} = "Warning! Sequence '4' retained because no isoform scheme matched" ]]
	[[ ${lines[4]} = "Warning! Sequence '5' retained because no isoform scheme matched" ]]
	[ $status == 0 ]
	
	run diff regexFail.fa data/isoformCleaner/regex.fasta
	[ $status == 0 ]

	rm regexFail.fa

	run -1 ../../build/isoformCleaner -i data/isoformCleaner/regex.fasta -o regexFail.fa -p doesnotexit
	[ "$output" == "Error: Preset 'doesnotexit' is unknown!" ]

	run -1 ../../build/isoformCleaner --doesnotexist
    [[ ${lines[0]} = "An error occurred parsing the command line: unrecognised option '--doesnotexist'" ]]
	[[ ${lines[1]} = "Please use -h/--help for more information." ]]

}

@test "help messages" {
    run ../../build/isoformCleaner -h
	[[ ${lines[2]} =~ "Allowed options are displayed below:" ]]
	[ $status == 0 ]
}

