#include "isoformCleaner.hpp"

#include <map>

void 
IsoformCleaner::identifyIsoforms_(const BSDL::SequenceSet<BSDL::Sequence<> > &seqSet)
{
    isoformMap_.clear();
    size_t nSeqs = seqSet.size();
    for (size_t i=0; i<nSeqs; ++i)
    {
        auto const seq = seqSet[i];
        auto const result = geneNameIdenification_(seq);
        if (!result.second)
        {
            std::cerr << "Sequence "  << seq.name() << " retained because no isoform scheme matched\n" << std::endl;
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

BSDL::SequenceSet<BSDL::Sequence<> > 
IsoformCleaner::createNewSeqSet_(BSDL::SequenceSet<BSDL::Sequence<> > &seqSet)
{
    BSDL::SequenceSet<BSDL::Sequence<> > outSet;
	for (auto &isoform : isoformMap_)
		outSet.emplace_back(std::move(seqSet[isoform.second.seqId]));
    return outSet;
}


BSDL::SequenceSet<BSDL::Sequence<> >
IsoformCleaner::clean(BSDL::SequenceSet<BSDL::Sequence<> > &seqSet)
{
    identifyIsoforms_(seqSet);
    return std::move(createNewSeqSet_(seqSet));
}

