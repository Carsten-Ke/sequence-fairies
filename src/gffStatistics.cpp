#include "gffStatistics.hpp"
#include "SummaryStatistics.hpp"

#include "../libs/BioSeqDataLib/src/gff/GFFRecord.hpp"
#include "../libs/BioSeqDataLib/src/utility/Input.hpp"

#include <vector>
#include <ranges>


SummaryStatistics
createGFFStatistics(const std::filesystem::path &fileName)
{
    SummaryStatistics stats;
    BioSeqDataLib::Input gffFile(fileName);
    std::string line;
    std::map<std::string, std::vector<BioSeqDataLib::Interval> > CDScollection;
    while (BioSeqDataLib::getline(gffFile, line))
    {
        if (line.empty() || (line[0] == '#'))
        {
            continue;
        }
        BioSeqDataLib::GFFRecord record(line);
        if (record.type == "exon")
        {
            stats.addExonLength(record.length());
        }
        else if (record.type == "gene")
        {
            stats.addGeneLength(record.length());
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
    
    for (auto mRNA : CDScollection)
    {
        size_t length = 0;
        std::ranges::sort(mRNA.second);
        for (auto CDS : mRNA.second)
        {
            length += CDS.length();
        }
        stats.addProteinLength(length);
    }

    return stats;
}
