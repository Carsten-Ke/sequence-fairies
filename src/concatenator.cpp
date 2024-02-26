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

#include <algorithm>
#include <set>

void
throw_duplicate(const std::string &seqName, const std::filesystem::path &fileName)
{
    throw std::runtime_error("Error! Duplicated sequence name '" + seqName + "' in file '" + fileName.string() + "' found!");
}

std::map<std::string, int>
createNamesIndex(const BioSeqDataLib::SequenceSet &seqSet, const std::vector<std::string> &patterns)
{
    std::map<std::string, int> namesIndex;
    std::map<std::string, int> occurences;
    std::set<int> indices;
    int id = -1;
    for (auto &seq : seqSet)
    {
        ++id;
        int counter = 0;
        for (auto &pattern : patterns)
        {
            auto position = seq.name().find(pattern);
            if (position != std::string::npos)
            {
                ++occurences[pattern];
                namesIndex[pattern] = id;
                ++counter;
            }
        }
        if (counter > 1)
        {
            throw std::runtime_error("Error! Patterns match same sequence in file '" + seqSet.file().string());
        }
    }
    for (auto &elem : occurences)
    {
        if (elem.second > 1)
        {
            throw_duplicate(elem.first, seqSet.file());
        }
    }

    return namesIndex;
}

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
           throw_duplicate(seq.name(), seqSet.file());
        }
    }

    return namesIndex;
}


std::set<std::pair<int, int> >
createMatches(const std::map<std::string, int> &names1, const fs::path &seqFile1, const std::map<std::string, int> &names2, const fs::path &seqFile2, bool lenient)
{
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
                throw std::runtime_error("Error! Sequence '" + elem.first + "' not contained in file: " + seqFile2.string());
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
                throw std::runtime_error("Error! Sequence '" + elem.first + "' not contained in file: " + seqFile1.string());
            }
        }
    }

    return pairing;
}


void concatenate(BioSeqDataLib::SequenceSet &seqSet1, const BioSeqDataLib::SequenceSet &seqSet2, const std::set<std::pair<int, int> > &matches)
{
    auto currentLength = seqSet1[0].length();
    size_t max_size = 0;
    BioSeqDataLib::Sequence *seq = nullptr;
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
            max_size = seq->length();
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
