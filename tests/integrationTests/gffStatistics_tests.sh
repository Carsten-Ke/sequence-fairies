#!/usr/bin/env bats
bats_require_minimum_version 1.5.0

@test "gffStatistics" {
    run -0 ../../build/gffStatistics -i data/gffStatistics/example.gff -o result
    [[ ${lines[0]} = "file	nGenes	avgGene	medianGene	nExons	avgExon	medianExon	nCDS	avgCDS	medianCDS" ]]
    [[ ${lines[1]} = "data/gffStatistics/example.gff	2	100.00	100.00	8	13.12	13.00	4	9.75	9.00" ]]
}
