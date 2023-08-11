#include "concatenator.hpp"

#include <set>

bool 
checkSequenceOccurrence(const BioSeqDataLib::SequenceSet &seqSet1, const BioSeqDataLib::SequenceSet &seqSet2)
{
    if (seqSet1.size() != seqSet2.size())
    {
        return false;
    }
    std::set<std::string> names;
    for (auto &seq : seqSet1)
    {
        names.insert(seq.name());
    }

    auto itEnd = names.end();
    for (auto &seq : seqSet2)
    {
        if (names.find(seq.name()) == itEnd)
        {
            return false;
        }
    }
    return true;
}

void concatenate(BioSeqDataLib::SequenceSet &seqSet1, BioSeqDataLib::SequenceSet &seqSet2)
{
    if (!checkSequenceOccurrence(seqSet1, seqSet2))
    {
        throw std::runtime_error("ERROR! Sequences don't match");
    }

    std::map<std::string, int> names;
    size_t id = -1;
    for (auto &seq : seqSet2)
    {
        names.insert(std::make_pair(seq.name(), ++id));
    }

    for (auto &seq : seqSet1)
    {
        seq.append(seqSet2[names[seq.name()]].seq());
    }

}
