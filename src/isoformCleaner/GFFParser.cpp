

module;

#include <iostream>
#include <filesystem>
#include <map>
#include <set>
#include <string>

#include "../../libs/BioSeqDataLib/src/utility/Input.hpp"
#include "../../libs/BioSeqDataLib/src/gff/GFFRecord.hpp"
#include "../../libs/BioSeqDataLib/src/gff/GFFRecord.hpp"



export module isoformCleaner:GFFParser;

export struct Gene
{
public: 

    Gene(const std::string &identifier) : id(identifier)
    {}

    auto
    addTranscript(const BioSeqDataLib::GFFRecord &record)
    {
        return transcripts.emplace(record.attributes.at("ID"), 0);
    }


    auto
    addTranscriptLength(const std::string &transcriptID, size_t l) -> bool
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


    
    auto
    longestTranscript() const -> std::string
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

export
auto
readGFF(const std::filesystem::path &fileName, std::string level1, std::string level2, std::string level3) ->  std::map<std::string, Gene>
{
    std::transform(level1.begin(), level1.end(), level1.begin(), ::toupper);
    std::transform(level2.begin(), level2.end(), level2.begin(), ::toupper);
    std::transform(level3.begin(), level3.end(), level3.begin(), ::toupper);

    std::string line;
    BioSeqDataLib::Input gffFile(fileName);
    std::map<std::string, Gene> genes;
    std::map<std::string, BioSeqDataLib::GFFRecord> tmpLevel2;
    std::set<std::pair<std::string, size_t> > tmpLevel3;
    std::map<std::string, Gene*> entered;
    while (BioSeqDataLib::getline(gffFile, line))
    {
        if (line.empty() || (line[0] == '#'))
        {
            continue;
        }
        BioSeqDataLib::GFFRecord record(line);
        std::transform(record.type.begin(), record.type.end(), record.type.begin(), ::toupper);
        if (record.type == level1)
        {
            auto result = genes.emplace(record.attributes["ID"], Gene(record.attributes["ID"]));
            if (!result.second)
            {
                std::cerr << "Error! " << level1 << " with ID " << record.attributes["ID"] << " occurred twice!\n";
                exit(EXIT_FAILURE);
            }
        }
        else if (record.type == level2)
        {
            for (const auto &parent : record.parents)
            {
                auto it = genes.find(parent);
                if (it != genes.end())
                {
                    auto res = it->second.addTranscript(record);
                    entered[res.first->first] = &(it->second);
                }
                else
                {
                    tmpLevel2.emplace(record.attributes["ID"], record);
                }
            }
        }
        else if (record.type == level3)
        {
            for (const auto &parent : record.parents)
            {
                auto res = entered.find(parent);
                if (res != entered.end())
                {
                    res->second->addTranscriptLength(parent, record.length());
                } else
                {
                    tmpLevel3.emplace(parent, record.length());
                }
            }
        }
    }

    for (auto &elem : tmpLevel2)
    {
        for (const auto &parent : elem.second.parents)
        {
            auto it = genes.find(parent);
            if (it != genes.end())
            {
                auto res = it->second.addTranscript(elem.second);
                entered[res.first->first] = &(it->second);
            }
            else
            {
                std::cerr << "Error! No parent found for " << parent << "!\n";
                exit(EXIT_FAILURE);
            }
        }
    }

    for (const auto &elem : tmpLevel3)
    {
        auto res = entered.find(elem.first);
        if (res != entered.end())
        {
            res->second->addTranscriptLength(elem.first, elem.second);
        } else
        {
            std::cerr << "Error! No parent found for " << elem.first << "!\n";
            exit(EXIT_FAILURE);
        }
    }
    return genes;
}


export auto
longest(const std::map<std::string, Gene> &genes) -> std::set<std::string> 
{
    std::set<std::string> longestTranscripts;
    for (const auto &gene : genes)
    {
        longestTranscripts.emplace(gene.second.longestTranscript());
    }
    return longestTranscripts;
}

