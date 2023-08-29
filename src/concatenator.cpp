/**
 * @file concatenator.cpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2023-08-28
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
#include "concatenator.hpp"

#include <set>

std::map<std::string, int>
createNamesIndex(const BioSeqDataLib::SequenceSet &seqSet, std::string delimiter)
{
    std::map<std::string, int> namesIndex;
    int id = -1;
    for (auto &seq : seqSet)
    {
        auto elem = namesIndex.emplace(seq.name().substr(0, seq.name().find(delimiter)), ++id);
        if (!elem.second)
        {
            throw std::runtime_error("ERROR! Duplicated sequence name '" + seq.name() + "' in file '" + seqSet.file().string() + "' found!");
        }
    }

    return namesIndex;
}


std::set<std::pair<int, int> >
createMatches(const BioSeqDataLib::SequenceSet &seqSet1, const BioSeqDataLib::SequenceSet &seqSet2, bool lenient, std::string delimiter)
{

    auto names1 = createNamesIndex(seqSet1, delimiter);
    auto names2 = createNamesIndex(seqSet2, delimiter);

    std::set<std::pair<int, int> > pairing;
    for (auto &elem : names1)
    {
        auto it = names2.find(elem.first);
        if (it != names2.end())
        {
            pairing.emplace(elem.second, it->second);
        }
        else
        {
            if (lenient)
            {
                pairing.emplace(elem.second, -1);
            }
            else
            {
                throw std::runtime_error("ERROR! Sequence '" + elem.first + "' not contained in file: " + seqSet2.file().string());
            }
        }
    }
    for (auto &elem : names2)
    {
        auto it = names1.find(elem.first);
        if (it == names1.end())
        {
            if (lenient)
            {
                pairing.emplace(-1, elem.second);
            }
            else
            {
                throw std::runtime_error("ERROR! Sequence '" + elem.first + "' not contained in file: " + seqSet1.file().string());
            }
        }
    }

    return pairing;
}





/*
void
checkSequenceOccurrence(const BioSeqDataLib::SequenceSet &seqSet1, const BioSeqDataLib::SequenceSet &seqSet2, bool fillGaps)
{
    std::map<std::string, int> names;
    for (auto &seq : seqSet1)
    {
        names.emplace(seq.name(), 1);
    }

    for (auto &seq : seqSet2)
    {
        auto it=names.find(seq.name());
        if (it != names.end())
        {
            ++(it->second);
        }
        else
        {
            names.emplace(seq.name(), 1);
        }
    }
    for (auto &elem : names)
    {
        if (elem.second > 2)
        {
            throw std::runtime_error("ERROR! Duplicates of sequence '" + elem.first + "' found!");
        }
        if ((!fillGaps) && (elem.second < 2))
        {
            throw std::runtime_error("ERROR! Sequence '" + elem.first + "' not contained.");
        }
    }
}
*/

void concatenate(BioSeqDataLib::SequenceSet &seqSet1, const BioSeqDataLib::SequenceSet &seqSet2, const std::set<std::pair<int, int> > &matches)
{
    auto currentLength = seqSet1[0].length();
    size_t max_size = 0;
    BioSeqDataLib::Sequence *seq;
    for (auto &match : matches)
    {
        if (match.first == -1)
        {
            seqSet1.emplace_back(BioSeqDataLib::Sequence(seqSet2[match.second].name(), std::string(currentLength, '-'), "", ""));
            seqSet1.back().append(seqSet2[match.second].seq());
            seq = &seqSet1.back();
        }
        else if (match.second != -1)
        {
            seqSet1[match.first].append(seqSet2[match.second].seq());
            seq = &seqSet1[match.first];
        }
        if (seq->length() > max_size)
        {
            max_size = seqSet1.back().length();
        } 
    }

    for (auto &seq : seqSet1)
    {
        if (seq.length() < max_size)
        {
            seq.append(std::string(max_size - seq.length(), '-'));
        }
    }
}

/*
void concatenate(BioSeqDataLib::SequenceSet &seqSet1, BioSeqDataLib::SequenceSet &seqSet2, bool fillGaps)
{
    checkSequenceOccurrence(seqSet1, seqSet2, fillGaps);

    std::map<std::string, int> names;
    int id = -1;
    for (auto &seq : seqSet1)
    {
        names.insert(std::make_pair(seq.name(), ++id));
    }

    size_t max_size = 0;
    size_t old_length = seqSet1[0].size();
    for (auto &seq : seqSet2)
    {
        auto it = names.find(seq.name());
        if (it != names.end())
        {
            seqSet1[it->second].append(seq.seq());
            if (seqSet1[it->second].length() > max_size)
            {
                max_size = seqSet1[it->second].length();
            } 
        }
        else
        {
            if ((seq.length() + old_length) > max_size)
            {
                max_size = seq.size() + old_length;
            }
            seqSet1.emplace_back(BioSeqDataLib::Sequence(seq.name(), std::string(old_length, '-'), "", ""));
            seqSet1.back().append(seq.seq());
        }
    }

    if (fillGaps)
    {
        for (auto &seq : seqSet1)
        {
            if (seq.length() < max_size)
            {
                seq.append(std::string(max_size - seq.length(), '-'));
            }
        }
    }

    


}
*/