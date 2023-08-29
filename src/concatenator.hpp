/**
 * @file concatenator.hpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief Contains the concatenator functions
 * @version 1.0
 * @date 2023-08-11
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

#include "../libs/BioSeqDataLib/src/sequence/SequenceSet.hpp"



std::map<std::string, int>
createNamesIndex(const BioSeqDataLib::SequenceSet &seqSet, std::string delimiter);


std::set<std::pair<int, int> >
createMatches(const BioSeqDataLib::SequenceSet &seqSet1, const BioSeqDataLib::SequenceSet &seqSet2, bool lenient, std::string delimiter);


void concatenate(BioSeqDataLib::SequenceSet &seqSet1, const BioSeqDataLib::SequenceSet &seqSet2, const std::set<std::pair<int, int> > &matches);
