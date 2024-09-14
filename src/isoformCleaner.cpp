
module;

#include <map>
#include <functional>
#include <string>

#include "../libs/BioSeqDataLib/src/sequence/Sequence.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"

export module isoformCleaner;
export import :GFFParser;
export import :identifyName;
namespace BSDL = BioSeqDataLib;

export 
class IsoformCleaner
{

private:
   

    struct Isoform
    {
        Isoform(size_t n, size_t val) : seqId(n), value(val)
        {}
        size_t seqId;
        size_t value;
    };

    std::function<std::pair<std::string, ISOFORM_STATUS>(BSDL::Sequence)> geneNameIdenification_;
    std::map<std::string, Isoform> isoformMap_;

    size_t warningCounter_;
    size_t nSeqsBeforeCleaning_;
    size_t nSeqsAfterCleaning_;


void 
identifyIsoforms_(const BSDL::SequenceSet &seqSet)
{
    isoformMap_.clear();
    warningCounter_ = 0;
    size_t nSeqs = seqSet.size();
    for (size_t i=0; i<nSeqs; ++i)
    {
        auto const seq = seqSet[i];
        auto const result = geneNameIdenification_(seq);
        if (result.second == ISOFORM_STATUS::NOMATCH)
        {
            std::cerr << "Warning! Sequence '" << seq.name() << "' retained because no isoform scheme matched\n";
            ++ warningCounter_;
        }
        if (result.second == ISOFORM_STATUS::REMOVE)
        {
            continue;
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
createNewSeqSet_(BSDL::SequenceSet &seqSet)
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

public:

    /**
     * @brief Construct a new IsoformCleaner object
     * 
     */
    IsoformCleaner() : warningCounter_(0), nSeqsBeforeCleaning_(0), nSeqsAfterCleaning_(0)
    {
    }

    /**
     * @brief Returns the number of warnings
     * 
     * @return size_t 
     */
    size_t
    warnings()
    {
        return warningCounter_;
    }

    /**
     * @brief Returns the number of sequences before cleaning
     * 
     * @return size_t 
     */
    size_t
    nSeqsBeforeCleaning()
    {
        return nSeqsBeforeCleaning_;
    }


    /**
     * @brief returns the number of sequences after cleaning
     * 
     * @return size_t 
     */
    size_t
    nSeqsAfterCleaning()
    {
        return nSeqsAfterCleaning_;
    }

    /**
     * @brief Returns the number of sequences removed
     * 
     * @return size_t 
     */
    size_t
    nSeqsRemoved()
    {
        return nSeqsBeforeCleaning_ - nSeqsAfterCleaning_;
    }

    /**
     * @brief Set the Gene Name Identifcator function
     * 
     * @param geneNameID The function to use for geneName identification
     */
    void setGeneNameIdentifcator(std::function<std::pair<std::string, ISOFORM_STATUS>(BSDL::Sequence)> geneNameID)
    {
        geneNameIdenification_ = geneNameID;
    }

BSDL::SequenceSet
clean(BSDL::SequenceSet &seqSet)
{
    identifyIsoforms_(seqSet);
    return createNewSeqSet_(seqSet);
}


};



