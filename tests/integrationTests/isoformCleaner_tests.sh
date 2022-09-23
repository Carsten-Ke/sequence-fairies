#!/usr/bin/env bats

@test "split-cleaning" {
    run ../../build/isoformCleaner -i data/isoformCleaner/splitChar.fasta -o splitChar.fa
	[ $status == 0 ]
	
	run diff splitChar.fa data/isoformCleaner/splitChar_result.fasta
	[ $status == 0 ]
	
	run ../../build/isoformCleaner -i data/isoformCleaner/splitChar_fail.fasta -o splitChar_fail.fa
    [ "$output" == "Warning! Sequence 'seq3' retained because no isoform scheme matched" ]
	[ $status == 0 ]
	
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


@test "file-opening-errors" {
    run ../../build/isoformCleaner -i xxx -o splitChar.fa
    [ "$output" == "Error opening file 'xxx': No such file or directory: iostream error" ]
	[ $status != 0 ]
}


@test "error messages" {
    run ../../build/isoformCleaner -i data/isoformCleaner/regex.fasta -o regexFail.fa -p flybase
	[[ ${lines[0]} =~ "Warning! Sequence '1' retained because no isoform scheme matched" ]]
	[[ ${lines[1]} =~ "Warning! Sequence '2' retained because no isoform scheme matched" ]]
	[[ ${lines[2]} =~ "Warning! Sequence '3' retained because no isoform scheme matched" ]]
	[[ ${lines[3]} =~ "Warning! Sequence '4' retained because no isoform scheme matched" ]]
	[[ ${lines[4]} =~ "Warning! Sequence '5' retained because no isoform scheme matched" ]]
	[ $status == 0 ]
	
	run diff regexFail.fa data/isoformCleaner/regex.fasta
	[ $status == 0 ]

	rm regexFail.fa

	run ../../build/isoformCleaner -i data/isoformCleaner/regex.fasta -o regexFail.fa -p doesnotexit
	[ $status != 0 ]
	[ "$output" == "Error: Preset 'doesnotexit' is unknown!" ]

}

@test "help messages" {
    run ../../build/isoformCleaner -h
	[[ ${lines[2]} =~ "Allowed options are displayed below:" ]]
	[ $status == 0 ]
}

