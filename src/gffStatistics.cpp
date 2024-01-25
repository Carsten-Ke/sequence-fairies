#include "gffStatistics.hpp"
#include "SummaryStatistics.hpp"

#include "../libs/BioSeqDataLib/src/gff/GFFRecord.hpp"
#include "../libs/BioSeqDataLib/src/utility/Input.hpp"


SummaryStatistics
createGFFStatistics(const std::filesystem::path &fileName)
{
    SummaryStatistics stats;
    BioSeqDataLib::Input gffFile(fileName);
    std::string line;
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
        else
        if (record.type == "gene")
        {
            stats.addGeneLength(record.length());
        }
        else if (record.type == "mRNA")
        {
            stats.addProteinLength(record.length());
        }
    }
    return stats;
}
