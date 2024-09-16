/**
 * @file seqCheck.cpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2023-08-25
 * 
 * @copyright Copyright (c) 2023
 * 
 * This file is part of BioSeqDataLib.
 * 
 * BioSeqDataLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BioSeqDataLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BioSeqDataLib.  If not, see <http://www.gnu.org/licenses/>.
 */

module;

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

// Boost
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

// BSDL header
#include "../libs/BioSeqDataLib/src/bio/Alphabet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SeqSetIOManager.hpp"

// other
#include "cmake_generated/project_version.h"

export module seqCheck;

using namespace std;
namespace fs = std::filesystem;
namespace BSDL = BioSeqDataLib;


export
struct Problems
{
	Problems(const std::string &seq_name) : name(seq_name), hasProblem(false), stopCodons(false), problemChars(""), duplicate(false), fixed(false), removed(false)
	{}

	virtual ~Problems()
	{}

	std::string name;
	bool hasProblem;
	bool stopCodons;
	std::string problemChars;
	bool duplicate;
	bool fixed;
	bool removed;
};


export
void
printReport(const std::vector<Problems> &problems, size_t nSeqs, BSDL::Output &outF)
{
	size_t totalFixed = 0;
	size_t totalRemoved = 0;
	for (auto &problem : problems)
	{
		if (problem.removed)
		{
			++totalRemoved;
		}
		else if (problem.fixed)
		{
			++totalFixed;
		}
	}

    outF << "--- SUMMARY ---\n";
	outF << "# sequences: " << nSeqs << "\n";
	outF << "# sequences with problems: " << problems.size() << "\n";
	outF << "# sequences fixed:         " << totalFixed << "\n";
	outF << "# sequences removed:       " << totalRemoved << "\n";
	
	outF << "\n\n--- DETAILS ---\n\n";
	for (auto &problem : problems)
	{
		outF << problem.name;
		if (problem.removed)
		{
			outF << " - REMOVED\n";
		}
		else if (problem.fixed)
		{
			outF << " - FIXED\n";
		}
		else
		{
			outF << "\n";
		}

		if (problem.stopCodons)
		{
			outF << "- stopCodons\n";
		}

		if (!problem.problemChars.empty())
		{
			outF << "- symbols: " << problem.problemChars << "\n";
		}

		if (problem.duplicate)
		{
			outF << "- duplicate\n";
		}

		outF << "\n";
	}
	
}

