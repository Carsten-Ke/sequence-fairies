#!/usr/bin/env bats

@test "extract_domains" {
    run ../../build/extractDomains -i data/extractDomains/seqs.fasta -d data/extractDomains/seqs.pfam -o domains.fa
	[ $status == 0 ]
	
	run diff domains.fa data/extractDomains/seqs_result.fasta
	[ $status == 0 ]
	
	rm domains.fa
	
    run ../../build/extractDomains -i data/extractDomains/seqs.fasta data/extractDomains/demo_seq.fasta -d data/extractDomains/seqs.pfam data/extractDomains/demo_seq.pfam -o multi_file_domains.fa
	[ $status == 0 ]
	
	run diff multi_file_domains.fa data/extractDomains/multi_file_domains_result.fa
	[ $status == 0 ]
	
	rm multi_file_domains.fa
}

@test "extract_domains_with_specific_id" {
	run ../../build/extractDomains -i data/extractDomains/specificID.fasta -d data/extractDomains/specificID.pfam --id PF00001 -o specificID.fa
	[ $status == 0 ]
	run diff specificID.fa  data/extractDomains/specificID_result.fa
	[ $status == 0 ]
	rm specificID.fa

	
	../../build/extractDomains -i data/extractDomains/specificID.fasta -d data/extractDomains/specificID.pfam --id PF00001 --min-length 2 -o specificID_minL.fa 
	[ $status == 0 ]
	run diff specificID_minL.fa data/extractDomains/specificID_minL_result.fa
	[ $status == 0 ]
	rm specificID_minL.fa
}

@test "extract_domains_envelope" {
	run ../../build/extractDomains -i data/extractDomains/specificID.fasta -d data/extractDomains/specificID.pfam --id PF00001 -o specificID_env.fa -e
	[ $status == 0 ]
	run diff specificID_env.fa  data/extractDomains/specificID_env_result.fa
	[ $status == 0 ]
	rm specificID_env.fa

	
	../../build/extractDomains -i data/extractDomains/specificID.fasta -d data/extractDomains/specificID.pfam --id PF00001 --min-length 3 -o specificID_minL_env.fa -e
	[ $status == 0 ]
	run diff specificID_minL_env.fa data/extractDomains/specificID_minL_env_result.fa
	[ $status == 0 ]
	rm specificID_minL_env.fa
}



@test "file-opening-errors" {
    run ../../build/extractDomains -i x -d data/extractDomains/seqs.pfam -o aha
    [ "$output" == "Error opening file 'x': No such file or directory: iostream error" ]
	[ $status != 0 ]
	
	run ../../build/extractDomains -i data/extractDomains/seqs.fasta -d x -o aha 
    [ "$output" == "Error opening file 'x': No such file or directory: iostream error" ]
	[ $status != 0 ]
}
