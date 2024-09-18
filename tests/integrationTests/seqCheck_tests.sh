#!/usr/bin/env bats
bats_require_minimum_version 1.5.0


@test "stop1-cleaning" {
    run ../../build/bin/seqCheck --fix-end --remove-stop -i ./data/seqCheck/input.fasta --set-alphabet DNA -o stop1.fasta
	[ $status == 0 ]
	
	run diff stop1.fasta ./data/seqCheck/stop1-result.fasta
	[ $status == 0 ]

	rm stop1.fasta
}

@test "stop2-cleaning" {
    run ../../build/bin/seqCheck --remove-stop -i ./data/seqCheck/input.fasta --set-alphabet DNA -o stop2.fasta
	[ $status == 0 ]
	
	run diff stop2.fasta ./data/seqCheck/stop2-result.fasta
	[ $status == 0 ]

	rm stop2.fasta
}


@test "fix-remove" {
    run ../../build/bin/seqCheck --fix-and-remove -i ./data/seqCheck/input.fasta --set-alphabet DNA -o fix-remove.fasta -r fix-keep-report.txt 
	[ $status == 0 ]
	
	run diff fix-remove.fasta ./data/seqCheck/fix-remove-result.fasta
	[ $status == 0 ]

	run diff fix-keep-report.txt ./data/seqCheck/fix-keep-report-result.txt
	[ $status == 0 ]

	rm fix-remove.fasta  fix-keep-report.txt
}

@test "fix-keep" {
    run ../../build/bin/seqCheck --fix-and-keep -i ./data/seqCheck/input.fasta --set-alphabet DNA -o fix-keep.fasta
	run diff fix-keep.fasta ./data/seqCheck/fix-keep-result.fasta
	[ $status == 0 ]

	rm fix-keep.fasta
}

@test "replace-char" {
	run -0 ../../build/bin/seqCheck -i ./data/seqCheck/nt.fa --replace-char --set-alphabet DNA
	[ ${lines[0]} = ">seq1" ]
	[ ${lines[1]} = "ACGCTCATCTN" ]

	run -0 ../../build/bin/seqCheck -i ./data/seqCheck/aa.fa --replace-char
	[ ${lines[0]} = ">seq1" ]
	[ ${lines[1]} = "AAXT" ]

}


@test "simple messages and errors" {
    run -0 ../../build/bin/seqCheck -h

    run -1 ../../build/bin/seqCheck --doesnotexist
    [[ ${lines[0]} = "An error occurred parsing the command line: unrecognised option '--doesnotexist'" ]]
	[[ ${lines[1]} = "Please use -h/--help for more information." ]]

	run -1 ../../build/bin/seqCheck -i x
    [ "$output" = "Error opening file 'x': No such file or directory: iostream error" ]

    run -1 ../../build/bin/seqCheck -i x --rename-duplicates --remove-duplicates
    [ "$output" = "Error! The options remove-duplicates and rename-duplicates are mutually exclusive!" ]

    run -1 ../../build/bin/seqCheck -i x --fix-and-keep --fix-and-remove
    [ "$output" = "Error! The options fix-and-keep and fix-and-remove are mutually exclusive!" ]

	run -1 ../../build/bin/seqCheck -i x --replace-char --remove-alpha
    [ "$output" = "Error! The options replace-char and remove-alpha are mutually exclusive!" ]

	run -1 ../../build/bin/seqCheck -i x --remove-stop-genes --replace-stop
    [ "$output" = "Error! The options remove-stop-genes and replaceStop are mutually exclusive!" ]
 
}
