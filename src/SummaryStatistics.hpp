/**
 * @file SummaryStatistics.hpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2024-01-25
 * 
 * @copyright Copyright (c) 2024
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


#ifndef SUMMARYSTATISTICS_HPP_
#define SUMMARYSTATISTICS_HPP_

#include <map>
#include <string>
#include <vector>


class SummaryStatistics
{
public:

    
    void
    addValue(const std::string &id, long value)
    {
        values[id].emplace_back(value);
    }

    size_t
    nValues(const std::string &id) const
    {
        return values.at(id).size();
    }

    const std::vector<long>
    getValues(const std::string &id) const
    {
        return values.at(id);
    }

    double
    average(const std::string &id) const
    {
        return calculateAverage(values.at(id));
    }

    double
    median(const std::string &id)
    {
        return calculateMedian(values[id]);
    }

private:

    double calculateAverage(const std::vector<long> &values) const;
    double calculateMedian(std::vector<long> &values);

    std::map<std::string, std::vector<long> > values;
};

#endif
