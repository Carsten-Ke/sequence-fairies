/**
 * @file SummaryStatistics.hpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
 * 
 * This file is part of BioSeqDataLib.
 * 
 * BioSeqDataLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BioSeqDataLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BioSeqDataLib.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef SUMMARYSTATISTICS_HPP_
#define SUMMARYSTATISTICS_HPP_

#include <vector>

class SummaryStatistics
{
public:
        
void addProteinLength(long value)
{
    proteinLengths.emplace_back(value);
}

void addGeneLength(long value)
{
    geneLengths.emplace_back(value);
}

void addExonLength(long value)
{
    exonLengths.emplace_back(value);
}

double averageGeneLength()
{
    return calculateAverage(geneLengths);
}

double medianGeneLength()
{
    return calculateMedian(geneLengths);
}

double averageExonLength()
{
    return calculateAverage(exonLengths);
}

double medianExonLength()
{
    return calculateMedian(exonLengths);
}

double averageProteinLength()
{
    return calculateAverage(proteinLengths);
}

double medianProteinLength()
{
    return calculateMedian(proteinLengths);
}

size_t nProteins()
{
    return proteinLengths.size();
}

size_t nExons()
{
    return exonLengths.size();
}

size_t nGenes()
{
    return geneLengths.size();
}

private:

    double calculateAverage(const std::vector<long> &values);
    double calculateMedian(std::vector<long> &values);

    std::vector<long> geneLengths;
    std::vector<long> proteinLengths;
    std::vector<long> exonLengths;

};

#endif
