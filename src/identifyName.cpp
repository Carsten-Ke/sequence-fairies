/**
 * @file IdentifyName.hpp
 * @author Carsten Kemena (c.kemena@wwu.de)
 * @brief
 * @version 0.1
 * @date 2022-09-20
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "identifyName.hpp"

#include <utility>

namespace BSDL = BioSeqDataLib;

std::pair<std::string, ISOFORM_STATUS>
splitCharIdentifier(const BSDL::Sequence &seq, char splitChar)
{
    std::string seqName       = seq.name();
    size_t pos                = seqName.rfind(splitChar);
    ISOFORM_STATUS splitFound = ISOFORM_STATUS::MATCH;
    if (pos == std::string::npos)
    {
        splitFound = ISOFORM_STATUS::NOMATCH;
    }
    return std::pair<std::string, ISOFORM_STATUS>(seqName.substr(0, pos), splitFound);
}

std::pair<std::string, ISOFORM_STATUS>
regexIdentifier(const BSDL::Sequence &seq, const std::regex regEx, bool searchComment, bool searchName)
{
    std::smatch what;
    const std::string seqName = seq.name();
    std::string target;
    if (searchComment)
    {
        target = seq.comment();
    }
    else
    {
        if (searchName)
        {
            target = seqName;
        }
        else
        {
            target = seqName + " " + seq.comment();
        }
    }
    ISOFORM_STATUS status = regex_search(target, what, regEx) ? ISOFORM_STATUS::MATCH : ISOFORM_STATUS::NOMATCH;
    std::string geneName;
    if (status == ISOFORM_STATUS::MATCH)
    {
        geneName = what[1];
    }
    else
    {
        geneName = seqName;
    }
    return std::pair<std::string, ISOFORM_STATUS>(geneName, status);
}

std::pair<std::string, ISOFORM_STATUS>
nameIdentifier(const BSDL::Sequence &seq, const std::set<std::string> &names)
{
    std::string seqName = seq.name();
    auto itEnd          = names.end();
    return std::pair<std::string, ISOFORM_STATUS>(seqName, (names.find(seqName) != itEnd) ? ISOFORM_STATUS::KEEP
                                                                                          : ISOFORM_STATUS::REMOVE);
}
