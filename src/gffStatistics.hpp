/**
 * @file gffStatistics.hpp
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

#ifndef GFFSTATISTICS_HPP_
#define GFFSTATISTICS_HPP_

#include <filesystem>
#include <vector>

#include "SummaryStatistics.hpp"



SummaryStatistics
createGFFStatistics(const std::filesystem::path &fileName);


#endif