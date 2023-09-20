/**
 * @file GFFParser.hpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2023-09-20
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
#include <set>
#include <string>

#include "../libs/BioSeqDataLib/src/gff/GFFRecord.hpp"


struct Gene
{
public: 

    Gene(const std::string &identifier) : id(identifier)
    {}

    auto
    addTranscript(const BioSeqDataLib::GFFRecord &record)
    {
        return transcripts.emplace(record.attributes.at("ID"), 0);
    }


    bool
    addTranscriptLength(const std::string &transcriptID, size_t l)
    {
        auto res = transcripts.find(transcriptID);
        if (res != transcripts.end())
        {
            res->second += l;
            return true;
        }
        else
        {
            return false;
        }
    }


    std::string
    longestTranscript() const
    {
        size_t maxLength = 0;
        std::string id = "";
        for (auto &elem : transcripts)
        {
            if (elem.second > maxLength)
            {
                maxLength = elem.second;
                id = elem.first;
            }
        }
        return id;
    }

    private:
        std::string id;
        std::map<std::string, size_t> transcripts;
};



std::map<std::string, Gene>
readGFF(const std::filesystem::path &fileName, std::string level1, std::string level2, std::string level3);

std::set<std::string>
longest(const std::map<std::string, Gene> &genes);




