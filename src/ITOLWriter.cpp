#include "ITOLWriter.hpp"


ITOLWriter::ITOLWriter(std::filesystem::path &file)
{
	file_.open(file);

}

void
ITOLWriter::createHeader(std::string &color)
{
	file_ << "DATASET_SIMPLEBAR\n";
	file_ << "SEPARATOR COMMA\n";
	file_ << "DATASET_LABEL,GFFStatistics\n";
	file_ << "COLOR,#375e97\n";
	file_ << "FIELD_LABELS,Number of genes\n";
	file_ << "DATA\n";
}


