#!/usr/bin/env bats

@test "stop1-cleaning" {
    run ../../build/seqCheck --fix-end --remove-stop -i ./data/seqCheck/input.fasta --set-alphabet DNA -o stop1.fasta
	[ $status == 0 ]
	
	run diff stop1.fasta ./data/seqCheck/stop1-result.fasta
	[ $status == 0 ]

	rm stop1.fasta
}

@test "stop2-cleaning" {
    run ../../build/seqCheck --remove-stop -i ./data/seqCheck/input.fasta --set-alphabet DNA -o stop2.fasta
	[ $status == 0 ]
	
	run diff stop2.fasta ./data/seqCheck/stop2-result.fasta
	[ $status == 0 ]

	rm stop2.fasta
}


@test "fix-remove" {
    run ../../build/seqCheck --fix-and-remove -i ./data/seqCheck/input.fasta --set-alphabet DNA -o fix-remove.fasta -r fix-keep-report.txt 
	[ $status == 0 ]
	
	run diff fix-remove.fasta ./data/seqCheck/fix-remove-result.fasta
	[ $status == 0 ]

	run diff fix-keep-report.txt ./data/seqCheck/fix-keep-report-result.txt
	[ $status == 0 ]

	rm fix-remove.fasta  fix-keep-report.txt
}

@test "fix-keep" {
    run ../../build/seqCheck --fix-and-keep -i ./data/seqCheck/input.fasta --set-alphabet DNA -o fix-keep.fasta
	[ $status == 0 ]
	
	run diff fix-keep.fasta ./data/seqCheck/fix-keep-result.fasta
	[ $status == 0 ]

	rm fix-keep.fasta
}