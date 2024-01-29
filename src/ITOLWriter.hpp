/**
 * @file ITOLWriter.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <filesystem>
#include <string>

#include "../libs/BioSeqDataLib/src/utility/Output.hpp"

#ifndef ITOLWRITER_HPP_
#define ITOLWRITER_HPP_


class ITOLWriter
{
	public:
		ITOLWriter(std::filesystem::path &file);

		
	private:

		void
		createHeader(std::string &color);

		BioSeqDataLib::Output file_;

};


#endif