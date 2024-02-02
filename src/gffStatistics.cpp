#include "gffStatistics.hpp"
#include "SummaryStatistics.hpp"

#include "../libs/BioSeqDataLib/src/gff/GFFRecord.hpp"
#include "../libs/BioSeqDataLib/src/utility/Input.hpp"

#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>

void
calcIntronLengths(SummaryStatistics &stats, const std::string &mRNA, std::vector<BioSeqDataLib::GFFRecord> &gene)
{
    std::ranges::sort(gene);
    std::string seqName = gene.front().seqID;
    char phase = gene.front().phase;
    for (const auto &exon : gene)
    {
        if ((exon.seqID != seqName) || (exon.phase != phase))
        {
            std::cerr << "Couldn't calculate intron size for mRNA: " << mRNA << "\n";
            return;
        }
    }

    for (size_t i=1; i<gene.size(); ++i)
    {
        auto intronLength = gene[i].positions.start() - gene[i-1].positions.end()-1;
        stats.addValue("intron", intronLength);
    }
}

SummaryStatistics
createGFFStatistics(const std::filesystem::path &fileName)
{
    SummaryStatistics stats;
    BioSeqDataLib::Input gffFile(fileName);
    std::string line;
    std::map<std::string, std::vector<BioSeqDataLib::Interval> > CDScollection;
    std::map<std::string, std::vector<BioSeqDataLib::GFFRecord> > exonCollection;
    while (BioSeqDataLib::getline(gffFile, line))
    {
        if (line.empty() || (line[0] == '#'))
        {
            continue;
        }
        BioSeqDataLib::GFFRecord record(line);
        if (record.type == "exon")
        {
            stats.addValue("exon", record.length());
            for (auto parent : record.parents)
            {
                exonCollection[parent].push_back(record);
            }
        }
        else if (record.type == "gene")
        {
            stats.addValue("gene", record.length());
        }
        else if (record.type == "CDS")
        {
            for (auto parent : record.parents)
            {
                if (!CDScollection.contains(parent))
                {
                    CDScollection[parent] = {};
                }
                CDScollection[parent].push_back(record.positions);
            }
        }
    }
    
    for (auto &mRNA : exonCollection)
    {
        calcIntronLengths(stats, mRNA.first, mRNA.second);
    }
    
    for (auto mRNA : CDScollection)
    {
        size_t length = 0;
        std::ranges::sort(mRNA.second);
        for (auto CDS : mRNA.second)
        {
            length += CDS.length();
        }
        stats.addValue("protein", length);
    }


    return stats;
}
