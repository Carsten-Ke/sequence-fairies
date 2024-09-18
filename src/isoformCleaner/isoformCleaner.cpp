
module;

#include <map>
#include <functional>
#include <string>
#include <utility>

#include "../../libs/BioSeqDataLib/src/sequence/Sequence.hpp"
#include "../../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"

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

    size_t warningCounter_{0};
    size_t nSeqsBeforeCleaning_{0};
    size_t nSeqsAfterCleaning_{0};


void 
identifyIsoforms_(const BSDL::SequenceSet &seqSet)
{
    isoformMap_.clear();
    warningCounter_ = 0;
    size_t nSeqs = seqSet.size();
    for (size_t i=0; i<nSeqs; ++i)
    {
        auto const& seq = seqSet[i];
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



auto 
createNewSeqSet_(BSDL::SequenceSet &seqSet) -> BSDL::SequenceSet
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
    IsoformCleaner()  
    = default;

    /**
     * @brief Returns the number of warnings
     * 
     * @return size_t 
     */
    [[nodiscard]] auto
    warnings() const -> size_t
    {
        return warningCounter_;
    }

    /**
     * @brief Returns the number of sequences before cleaning
     * 
     * @return size_t 
     */
    [[nodiscard]] auto
    nSeqsBeforeCleaning() const -> size_t
    {
        return nSeqsBeforeCleaning_;
    }


    /**
     * @brief returns the number of sequences after cleaning
     * 
     * @return size_t 
     */
    [[nodiscard]] auto
    nSeqsAfterCleaning() const -> size_t
    {
        return nSeqsAfterCleaning_;
    }

    /**
     * @brief Returns the number of sequences removed
     * 
     * @return size_t 
     */
    [[nodiscard]] auto
    nSeqsRemoved() const -> size_t
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
        geneNameIdenification_ = std::move(geneNameID);
    }

auto
clean(BSDL::SequenceSet &seqSet) -> BSDL::SequenceSet
{
    identifyIsoforms_(seqSet);
    return createNewSeqSet_(seqSet);
}


};



