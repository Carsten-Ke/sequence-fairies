#include "concatenator.hpp"

#include <set>

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
