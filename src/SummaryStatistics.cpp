/**
 * @file SummaryStatistics.cpp
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
#include "SummaryStatistics.hpp"

#include<algorithm>
#include<numeric>


double
SummaryStatistics::calculateAverage(const std::vector<long> &values)
{
    if(values.empty())
    {
        return 0;
    }

    auto const count = static_cast<double>(values.size());
    return std::reduce(values.begin(), values.end()) / count;
}

double 
SummaryStatistics::calculateMedian(std::vector<long> &values)
{
    
    auto middleItr = values.begin() + values.size() / 2;
    std::nth_element(values.begin(), middleItr, values.end());
    if (values.size() % 2 == 0)
    {
        const auto leftMiddleItr = std::max_element(values.begin(), middleItr);
        return (*leftMiddleItr + *middleItr)*1.0 / 2;
    }
    else
    {
        return *middleItr;
    }
}
