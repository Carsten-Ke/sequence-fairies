#include "isoformCleaner.hpp"

#include <map>

void 
IsoformCleaner::identifyIsoforms_(const BSDL::SequenceSet &seqSet)
{
    isoformMap_.clear();
    warningCounter_ = 0;
    size_t nSeqs = seqSet.size();
    for (size_t i=0; i<nSeqs; ++i)
    {
        auto const seq = seqSet[i];
        auto const result = geneNameIdenification_(seq);
        if (!result.second)
        {
            std::cerr << "Warning! Sequence '" << seq.name() << "' retained because no isoform scheme matched\n";
            ++ warningCounter_;
        }
        auto it = isoformMap_.find(result.first);
        if (it == isoformMap_.end())
        {
            isoformMap_.emplace(result.first, Isoform(i, seq.size()));
        }
        else
        {
            if (seq.size() > it->second.value)
            {
                it->second.seqId = i;
                it->second.value = seq.size();
            }
        }
    }
}



BSDL::SequenceSet 
IsoformCleaner::createNewSeqSet_(BSDL::SequenceSet &seqSet)
{
    nSeqsBeforeCleaning_ = seqSet.size();
    BSDL::SequenceSet outSet;
	for (const auto &isoform : isoformMap_)
    {
		outSet.emplace_back(std::move(seqSet[isoform.second.seqId]));
    }
    nSeqsAfterCleaning_ = outSet.size();
    return outSet;
}


BSDL::SequenceSet
IsoformCleaner::clean(BSDL::SequenceSet &seqSet)
{
    identifyIsoforms_(seqSet);
    return createNewSeqSet_(seqSet);
}

