#!/usr/bin/env bats

@test "name extraction" {
	../../build/seqExtract -i data/seqExtract/test.fa -e seq1 seq5 -o test1.fa
	run diff data/seqExtract/results/test1.fa test1.fa
	[ $status = 0 ]
	rm test1.fa


	../../build/seqExtract -i data/seqExtract/test.fa -e seq1 seq5 seq2 seq3 seq7 seq8 -r -o test2.fa
	run diff data/seqExtract/results/test2.fa test2.fa
	[ $status = 0 ]
	rm test2.fa

	run ../../build/seqExtract -C 2 -N data/seqExtract/namesFile.txt -i data/seqExtract/test.fa -d " "
	[ "$output" = $'>seq1 se\nA\n>seq3 se3\nAAA\n>seq4\nAAAA' ]

    run ../../build/seqExtract --get-names-from-fasta data/seqExtract/names.fasta -i data/seqExtract/test.fa
    [ "$output" = $'>seq1 se\nA\n>seq3 se3\nAAA\n>seq4\nAAAA' ]

	run ../../build/seqExtract -i data/seqExtract/test.fa -e seq5 seq1 --extract-order
	[ "$output" = $'>seq5\nAAAAA\n>seq1 se\nA' ]

	run ../../build/seqExtract -i data/seqExtract/test.fa -e seq1 --remove-comments
	[ "$output" = $'>seq1\nA' ]

	#run ../../build/seqExtract -E -e "^se$" -p comment -i data/seqExtract/test.fa
	#[ "$output" = $'>seq1 se\nA\n>seq2 se\nAA' ]

	#run ../../build/seqExtract -E -e "^se3$" -p comment -i data/seqExtract/test.fa
	#[ "$output" = $'>seq3 se3\nAAA' ]

	#run ../../build/seqExtract -E -e "seq1" -p total -i data/seqExtract/test.fa
	#[ "$output" = $'>seq1 se\nA' ]

	#run ../../build/seqExtract -E -e "seq1" -p name -i data/seqExtract/test.fa
	#[ "$output" = $'>seq1 se\nA' ]

	#run ../../build/seqExtract -E -e "^se3$" -p comment -i data/seqExtract/test.fa --remove-comments
	#[ "$output" = $'>seq3\nAAA' ]


}

@test "multi file extraction" {
	result="$(../../build/seqExtract -i data/seqExtract/test.fa data/seqExtract/test2.fa -e seq1 seqA)"
	val=$'>seq1 se\nA\n>seqA\nA'
	[ "$result" == "$val" ]

	run ../../build/seqExtract -l data/seqExtract/inputList.txt -e seq1 seqA
	[ "$output" = $'>seq1 se\nA\n>seqA\nA' ]

	run ../../build/seqExtract -i data/seqExtract/test.fa data/seqExtract/test2.fa -e seqA seq1 --extract-order
	[ "$output" = $'>seqA\nA\n>seq1 se\nA' ]
}


@test "sub section extraction" {
	result="$(../../build/seqExtract -i data/seqExtract/test.fa -e seq2:1-1 seq9:3-5,8-11 seq9:12-14 -S)"
	val=$'>seq2:1-1\nA\n>seq9:3-5,8-11\nGTAARAI\n>seq9:12-14\nGAR'
	[ "$result" == "$val" ]
}


@test "length dependent extraction" {
	result="$(../../build/seqExtract -i data/seqExtract/test.fa --length '<3')"
	val=$'>seq1 se\nA\n>seq2 se\nAA'
	[ "$result" == "$val" ]

	result="$(../../build/seqExtract -i data/seqExtract/test.fa -L '>7')"
	val=$'>seq8\nAAAAAAAA\n>seq9\nACGTAIGARAIGARIAG'
	[ "$result" == "$val" ]

	result="$(../../build/seqExtract -i data/seqExtract/test.fa -L '>3' '<7')"
	val=$'>seq4\nAAAA\n>seq5\nAAAAA\n>seq6\nAAAAAA'
	[ "$result" == "$val" ]

	result="$(../../build/seqExtract -i data/seqExtract/test.fa -L '=4')"
	val=$'>seq4\nAAAA'
	[ "$result" == "$val" ]
}


@test "index test" {
	result="$(../../build/seqExtract --index -i data/seqExtract/test.fa  -e seq2)"
	val=$'>seq2 se\nAA'
	[ "$result" == "$val" ]

	result="$(../../build/seqExtract --index -i data/seqExtract/test.fa  -e seq3)"
	val=$'>seq3 se3\nAAA'
	[ "$result" == "$val" ]

	rm data/seqExtract/test.sei
}

#@test "regex test" {
#	result="$(../../build/seqExtract -i data/regExtract.fa -E -e 'seq[0-9]')"
#	val=$'>seq1\nA\n>seq2\nAC\n>seq3\nACG'
#	[ "$result" == "$val" ]
#
#	result="$(../../build/seqExtract -i data/regExtract.fa -E -e 'seq[0-9]' -r)"
#	val=$'>seqA\nACGT\n>seqB\nACGTa'
#	[ "$result" == "$val" ]
#
#	result="$(../../build/seqExtract -i data/regExtract.fa -E -e 'seq[0-9]' -I)"
#	val=$'>seq1\nA\n>seq2\nAC\n>seq3\nACG'
#	[ "$result" == "$val" ]
#
#	result="$(../../build/seqExtract -i data/regExtract.fa -E -e 'seq[0-9]' -r -I)"
#	val=$'>seqA\nACGT\n>seqB\nACGTa'
#	[ "$result" == "$val" ]
#}


@test "random test" {
	result="$(../../build/seqExtract -n 2 -i data/seqExtract/test.fa -s 4)"
	val=$'>seq9\nACGTAIGARAIGARIAG\n>seq8\nAAAAAAAA'
	[ "$result" == "$val" ]
}

@test "modify test" {
	result="$(../../build/seqExtract -e seq1 -t -i data/seqExtract/modifyTest.fa)"
	val=$'>seq1\nMTSMSSLNLTPAIIRRLQPA'
	[ "$result" == "$val" ]

	result="$(../../build/seqExtract -e seq2 -R -t -i data/seqExtract/modifyTest.fa)"
	val=$'>seq2:r\nMTSMSSLNLTPAIIRRLQPA'
	[ "$result" == "$val" ]
}



@test "error messages" {
	program=../../build/seqExtract
	run $program
	[ "$status" -eq 1 ]
	[ "$output" = $'Error parsing command line: No input files are given!\nUse -h/--help for option information' ]

	run $program -xyz
	[ "$status" -eq 1 ]
	[ "$output" = $'Error parsing command line: unrecognised option \'-xyz\'\nUse -h/--help for option information' ]

	run $program -h
	[ "$status" -eq 0 ]

	run $program -i notExisting
	[ "$status" -eq 1 ]

	run $program -i data/test.fa -o /trinate
	[ "$status" -eq 1 ]

	run $program -i data/seqExtract/test.fa -S -e seq2:2
	[ "$status" -eq 1 ]
	[ "$output" = $'An error occured when parsing the extract arguments! Is there possibly a problem with the delimiter used for subsection extraction or was no \'-\' used?' ]

}
