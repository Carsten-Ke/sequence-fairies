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

namespace BSDL = BioSeqDataLib;
 
/**
 * @brief 
 * 
 */
class IsoformCleaner
{

private:
    /**
     * @brief 
     * 
     */
    struct Isoform
    {
        Isoform(size_t n, int val, bool v=3) : seqId(n), value(val)
        {}
        size_t seqId;
        int value;
    };
    std::function<std::pair<std::string, bool>(BSDL::Sequence<>)> geneNameIdenification_;

    std::map<std::string, Isoform> isoformMap_;

    void identifyIsoforms_(const BSDL::SequenceSet<BSDL::Sequence<> > &seqSet);
    BSDL::SequenceSet<BSDL::Sequence<> > createNewSeqSet_(BSDL::SequenceSet<BSDL::Sequence<> > &seqSet);


public:

    IsoformCleaner()
    {
    }

    /**
     * @brief Set the Gene Name Identifcator object
     * 
     * @param geneNameID 
     */
    void setGeneNameIdentifcator(std::function<std::pair<std::string, bool>(BSDL::Sequence<>)> geneNameID)
    {
        geneNameIdenification_ = geneNameID;
    }

    /**
     * @brief 
     * 
     * @param seqSet 
     * @return BSDL::SequenceSet<BSDL::Sequence<> > 
     */
    BSDL::SequenceSet<BSDL::Sequence<> >
    clean(BSDL::SequenceSet<BSDL::Sequence<> > &seqSet);


};

#endif
