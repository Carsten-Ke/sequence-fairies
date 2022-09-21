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

#include <utility>

#include "identifyName.hpp"

namespace BSDL = BioSeqDataLib;

std::pair<std::string, bool>
splitCharIdentifier(const BSDL::Sequence<> &seq, char splitChar)
{
	std::string seqName = seq.name();
	size_t pos = seqName.rfind(splitChar);
	bool splitFound = true;
	if (pos == std::string::npos)
	{
		splitFound = false;
	}
	return std::pair<std::string, bool>(seqName.substr(0, pos), splitFound);
}

std::pair<std::string, bool>
regexIdentifier(const BSDL::Sequence<> &seq, const std::regex regEx, bool searchComment, bool searchName)
{
	std::smatch what;
	const std::string seqName = seq.name();
	std::string target;
	if (searchComment)
		target = seq.comment();
	else
	{
		if (searchName)
			target = seqName;
		else
			target = seqName + " " + seq.comment();
	}
	bool found = regex_search(target, what, regEx);
	std::string geneName;
	if (found)
	{
		geneName = what[1];
	}
	else
	{
		geneName = seqName;
	}
	return std::pair<std::string, bool>(geneName, found);
}
