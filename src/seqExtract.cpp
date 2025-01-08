/**
 * @file seqExtract.cpp
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

#include "seqExtract.hpp"

#include "../libs/BioSeqDataLib/src/bio/GeneticCode.hpp"
#include "../libs/BioSeqDataLib/src/utility/utility.hpp"

#include <functional>
#include <random>
#include <set>

typedef std::multimap<std::string, std::vector<std::pair<size_t, size_t>>> Targets;

namespace fs   = std::filesystem;
namespace BSDL = BioSeqDataLib;

bool
minLength(size_t length, const BSDL::Sequence &seq)
{
    return (seq.size() > length);
}

bool
maxLength(size_t length, const BSDL::Sequence &seq)
{
    return (seq.size() < length);
}

bool
exactLength(size_t length, const BSDL::Sequence &seq)
{
    return (seq.size() == length);
}

bool
parseExtractionLine(const std::vector<std::string> &extractLine, std::vector<std::string> &names,
                    std::map<std::string, size_t> &order, Targets &targets, bool extractSegments, std::string delimiter)
{
    bool subsection = false;
    std::map<std::string, Targets>::iterator it;
    Targets t;
    size_t num = 0;
    std::set<std::string> nameSet;
    for (auto elem : extractLine)
    {
        std::vector<std::pair<size_t, size_t>> positions;
        if (!extractSegments) //(elem.find(delimiter) == std::string::npos)
        {
            nameSet.insert(elem);
            if (order.count(elem) == 0)
            {
                order[elem] = num++;
            }
            targets.emplace(elem, positions);
        }
        else
        {
            auto tokens   = BSDL::split(elem, ",-" + delimiter);
            size_t nElems = tokens.size();
            if ((nElems % 2) != 1)
            {
                throw std::runtime_error("Not the correct number of coordinates!");
            }

            for (size_t i = 1; i < nElems; i += 2)
            {
                subsection = true;
                positions.emplace_back(stoi(tokens[i]) - 1, stoi(tokens[i + 1]) - 1);
            }
            nameSet.insert(tokens[0]);
            if (order.count(tokens[0]) == 0)
            {
                order[tokens[0]] = num++;
            }
            targets.emplace(tokens[0], positions);
        }
    }
    for (auto &elem : nameSet)
    {
        names.emplace_back(std::move(elem));
    }
    return subsection;
}

void
randExtract(BSDL::SequenceSet &set, BSDL::SequenceSet &newSet, size_t nSeqs, const std::string &seed)
{
    std::random_device rd;
    std::mt19937 generator;
    if (seed.empty())
    {
        generator.seed(rd());
    }
    else
    {
        generator.seed(stoi(seed));
    }

    std::vector<int> numbers;
    size_t setSize = set.size();
    numbers.reserve(setSize);
    for (size_t i = 0; i < setSize; ++i)
    {
        numbers.push_back(i);
    }

    for (size_t i = 0; i < nSeqs; ++i)
    {
        std::uniform_int_distribution<int> dist(0, numbers.size() - 1);
        size_t num = dist(generator);
        newSet.emplace_back(set[numbers[num]]);
        numbers[num] = numbers[numbers.size() - 1];
        numbers.resize(numbers.size() - 1);
    }
}

void
revComp(BSDL::SequenceSet &set)
{
    size_t nSeqs = set.size();
    for (size_t i = 0; i < nSeqs; ++i)
    {
        BSDL::reverseComplement(set[i]);
        set[i].name(set[i].name() + ":r");
    }
}

void
translate(BSDL::SequenceSet &set, const std::string &translationTable)
{
    std::map<std::string, short> tTable = {
        {  "standard", 0},
        {  "mitVerte", 1},
        {  "mitYeast", 2},
        { "protozoan", 3},
        {"mitInverte", 4}
    };
    BSDL::GeneticCode translator(tTable[translationTable]);
    size_t nSeqs = set.size();
    for (size_t i = 0; i < nSeqs; ++i)
    {
        if ((set[i].size() % 3) != 0)
        {
            throw std::runtime_error("Length of sequence '" + set[i].name() + "' is not a multiple of three!");
        }
        translator.translate(set[i]);
    }
}

void
lengthExtract(BSDL::SequenceSet &set, BSDL::SequenceSet &newSet, const std::vector<std::string> &lengthStrings)
{
    // scan
    std::vector<std::function<bool(const BSDL::Sequence &)>> functions;
    size_t val;
    for (const auto &func : lengthStrings)
    {
        if (func[0] == '>')
        {
            val = stoi(func.substr(1));
            functions.push_back(std::bind(minLength, val, std::placeholders::_1));
        }
        if (func[0] == '<')
        {
            val = stoi(func.substr(1));
            functions.push_back(std::bind(maxLength, val, std::placeholders::_1));
        }

        if (func[0] == '=')
        {
            val = stoi(func.substr(1));
            functions.push_back(std::bind(exactLength, val, std::placeholders::_1));
        }
    }
    size_t nSeqs = set.size();
    for (size_t i = 0; i < nSeqs; ++i)
    {
        bool keep = true;
        for (const auto &func : functions)
        {
            if (!func(set[i]))
            {
                keep = false;
            }
        }
        if (keep)
        {
            newSet.emplace_back(set[i]);
        }
    }
}

void
extractNamesFromFasta(const fs::path &namesFile, std::vector<std::string> &names)
{
    BSDL::Input in(namesFile);
    std::string line;
    while (getline(in, line))
    {
        if (!line.empty() && line[0] == '>')
        {
            auto tokens = BSDL::split(line, " >");
            names.emplace_back(std::move(tokens[0]));
        }
    }
}

void
readNamesFile(const fs::path &namesFile, std::vector<std::string> &extractLines, char delimiter, size_t column)
{
    BSDL::Input in(namesFile);
    std::string line;
    std::string delims = "";
    delims.push_back(delimiter);
    while (getline(in, line))
    {
        if (!line.empty())
        {
            auto tokens = BSDL::split(line, delims);
            extractLines.emplace_back(tokens[column]);
        }
    }
}

void
readInputFile(const fs::path &extractFile, std::vector<fs::path> &seqFiles, std::vector<fs::path> &indexFiles)
{
    BSDL::Input in(extractFile);
    std::string line;
    while (getline(in, line))
    {
        if (!line.empty())
        {
            auto tokens = BSDL::split(line, " \t");
            seqFiles.emplace_back(tokens[0]);
            if (tokens.size() == 2)
            {
                indexFiles.emplace_back(tokens[1]);
            }
        }
    }
}
