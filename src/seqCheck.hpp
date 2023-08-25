/**
 * @file seqCheck.hpp
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

#include<string>
#include<vector>

#include "../libs/BioSeqDataLib/src/utility/Output.hpp"

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

void
printReport(const std::vector<Problems> &problems, size_t nSeqs, BioSeqDataLib::Output &outF);