#!/bin/bash

echo "Test-1: extractDomains"
./extractDomains_tests.sh

echo "Test-2: isoformCleaner"
./isoformCleaner_tests.sh  

echo "Test-3: concatenator"
./concatenator_tests.sh

echo "Test-4: seqCheck"
./seqCheck_tests.sh

echo "Test-5: seqExtract"
./seqExract_tests.sh  

echo "Test-6: gffStatistics"
./gffStatistics_tests.sh