/**
 * @file ITOLWriter.hpp
 * @author Carsten Kemena (c.kemena@uni-muenster.de)
 * @brief 
 * @version 1.0
 * @date 2024-01-30
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

#include <filesystem>
#include <string>

#include "../libs/BioSeqDataLib/src/utility/Output.hpp"

#ifndef ITOLWRITER_HPP_
#define ITOLWRITER_HPP_


struct ITOLValueSet
{
	ITOLValueSet(const std::string &i, long v, const std::string &l) : id(i), value(v), label(l)
	{}

	std::string id;
	long value;	
	std::string label;
};

class ITOLWriter
{
	public:

		ITOLWriter(const std::filesystem::path &file, const std::string &label, const std::string &color);
		ITOLWriter(ITOLWriter&&) = default;
		ITOLWriter& operator=(ITOLWriter&&) = default;
		~ITOLWriter()
		{}

		void
		writeValues(const std::vector<ITOLValueSet> &values);

		void 
		close()
		{
			file_.close();
		}

	private:
		void
		writeHeader(const std::string &label, const std::string &color);

		std::string
		calculateScale(long maxValue);

		BioSeqDataLib::Output file_;
};




#endif