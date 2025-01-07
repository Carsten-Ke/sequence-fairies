#!/usr/bin/env bats
bats_require_minimum_version 1.5.0


@test "mergeFiles" {
	run ../../build/fileMerger -i data/fileMerger/test1.fa data/fileMerger/test2.fa -o merged.fa --rename
	[ $status == 0 ]
	run diff merged.fa  data/fileMerger/merged_result.fa
	[ $status == 0 ]
    rm merged.fa
}