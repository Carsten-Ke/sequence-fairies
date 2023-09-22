/**
 * @file IsoformCleaner.hpp
 * @author Carsten Kemena (c.kemena@wwu.de)
 * @brief Header file for IsoformCleaner functions
 * @version 0.1
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef ISOFORMCLEANER_HPP_
#define ISOFORMCLEANER_HPP_

#include <functional>
#include <string>

#include "../libs/BioSeqDataLib/src/sequence/Sequence.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "identifyName.hpp"

namespace BSDL = BioSeqDataLib;
 
/**
 * @brief Class to remove isoforms from a SequenceSet.
 * 
 */
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

    void identifyIsoforms_(const BSDL::SequenceSet &seqSet);
    BSDL::SequenceSet createNewSeqSet_(BSDL::SequenceSet &seqSet);



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

    /**
     * @brief Cleans a sequence set from isoforms
     * 
     * @param seqSet The SequenceSet to clean
     * @return BSDL::SequenceSet  Returns a cleaned sequence Set
     */
    BSDL::SequenceSet
    clean(BSDL::SequenceSet &seqSet);


};

#endif
