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
#include <string>

#include "../libs/BioSeqDataLib/src/sequence/Sequence.hpp"


namespace BSDL = BioSeqDataLib;

/**
 * @brief 
 * 
 * @param seq 
 * @param splitChar 
 * @return std::pair<std::string, bool> 
 */
std::pair<std::string, bool>  splitCharIdentifier(const BSDL::Sequence<> &seq, char splitChar);

/**
 * @brief 
 * 
 * @param seq 
 * @param regexStr 
 * @param searchComment 
 * @param searchName 
 * @return std::pair<std::string, bool> 
 */
std::pair<std::string, bool>  regexIdentifier(const BSDL::Sequence<> &seq, const std::regex regEx, bool searchComment, bool searchName);

#endif
