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


/**
 * @brief Create a Names Index object
 * 
 * @param seqSet 
 * @param delimiter 
 * @return std::map<std::string, int> 
 */
std::map<std::string, int>
createNamesIndex(const BioSeqDataLib::SequenceSet &seqSet, std::string delimiter);

/**
 * @brief Create a Names Index object
 * 
 * @param seqSet 
 * @param patterns 
 * @return std::map<std::string, int> 
 */
std::map<std::string, int>
createNamesIndex(const BioSeqDataLib::SequenceSet &seqSet, const std::vector<std::string> &patterns);

/**
 * @brief Create a Matches object
 * 
 * @param names1 
 * @param seqFile1 
 * @param names2 
 * @param seqFile2 
 * @param lenient 
 * @return std::set<std::pair<int, int> > 
 */
std::set<std::pair<int, int> >
createMatches(const std::map<std::string, int> &names1, const fs::path &seqFile1, const std::map<std::string, int> &names2, const fs::path &seqFile2, bool lenient);

/**
 * @brief 
 * 
 * @param seqSet1 
 * @param seqSet2 
 * @param matches 
 */
void concatenate(BioSeqDataLib::SequenceSet &seqSet1, const BioSeqDataLib::SequenceSet &seqSet2, const std::set<std::pair<int, int> > &matches);
