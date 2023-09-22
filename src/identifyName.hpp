/**
 * @file IdentifyName.hpp
 * @author Carsten Kemena (c.kemena@wwu.de)
 * @brief Collection of name extracting functions
 * @version 0.1
 * @date 2022-09-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef IDENTIFYNAME_HPP_
#define IDENTIFYNAME_HPP_

#include <regex>
#include <set>
#include <string>

#include "../libs/BioSeqDataLib/src/sequence/Sequence.hpp"


namespace BSDL = BioSeqDataLib;

 enum class ISOFORM_STATUS{MATCH, NOMATCH, KEEP, REMOVE};

/**
 * @brief 
 * 
 * @param seq 
 * @param splitChar 
 * @return std::pair<std::string, ISOFORM_STATUS> 
 */
std::pair<std::string, ISOFORM_STATUS>  splitCharIdentifier(const BSDL::Sequence &seq, char splitChar);

/**
 * @brief 
 * 
 * @param seq 
 * @param regEx 
 * @param searchComment 
 * @param searchName 
 * @return std::pair<std::string, ISOFORM_STATUS> 
 */
std::pair<std::string, ISOFORM_STATUS>  regexIdentifier(const BSDL::Sequence &seq, const std::regex regEx, bool searchComment, bool searchName);

/**
 * @brief 
 * 
 * @param seq 
 * @param names 
 * @return std::pair<std::string, ISOFORM_STATUS> 
 */
std::pair<std::string, ISOFORM_STATUS>
nameIdentifier(const BSDL::Sequence &seq, const std::set<std::string> &names);

#endif
