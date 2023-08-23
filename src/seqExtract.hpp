/**
 * @file seqExtract.hpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2023-08-23
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

#include <filesystem>
#include <map>
#include <vector>


#include "../libs/BioSeqDataLib/src/sequence/Sequence.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"
#include "../libs/BioSeqDataLib/src/sequence/SeqSetIOManager.hpp"


typedef	std::multimap<std::string, std::vector< std::pair<size_t, size_t> > > Targets;

namespace fs = std::filesystem;
namespace BSDL = BioSeqDataLib;

bool
minLength(size_t length, const BSDL::Sequence &seq);

bool
maxLength(size_t length, const BSDL::Sequence &seq);

bool
exactLength(size_t length, const BSDL::Sequence &seq);


bool
parseExtractionLine(const std::vector<std::string> &extractLine, std::vector<std::string> &names, 
    std::map<std::string, size_t> &order, Targets &targets, bool extractSegments, std::string delimiter);


void
randExtract(BSDL::SequenceSet &set, BSDL::SequenceSet &newSet, size_t nSeqs, const std::string &seed);

void
revComp(BSDL::SequenceSet &set);


void
translate(BSDL::SequenceSet &set, const std::string &translationTable);

void
lengthExtract(BSDL::SequenceSet &set, BSDL::SequenceSet &newSet, const std::vector<std::string> &lengthStrings);


void
readNamesFile(const fs::path &namesFile, std::vector<std::string> &extractLines, char delimiter, size_t column);

void
readInputFile(const fs::path &extractFile, std::vector<fs::path> &seqFiles, std::vector<fs::path> &indexFiles);

void
extractNamesFromFasta(const fs::path &namesFile, std::vector<std::string> &names);
