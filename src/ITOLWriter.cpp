#include "ITOLWriter.hpp"


ITOLWriter::ITOLWriter(const std::filesystem::path &file, const std::string &label, const std::string &color)
{
	file_.open(file);
	writeHeader(label, color);
}

void
ITOLWriter::writeHeader(const std::string &label, const std::string &color)
{
	file_ << "DATASET_SIMPLEBAR\n";
	file_ << "SEPARATOR COMMA\n";
	file_ << "DATASET_LABEL," << label << "\n";
	file_ << "COLOR," << color << "\n";
	file_ << "#OPTIONAL\n";
	file_ << "FIELD_LABELS,Number of genes\n";
	file_ << "WIDTH,200\n";
	file_ << "SHOW_VALUE,1\n";
	file_ << "LABEL_POSITION,left\n";
	file_ << "LEGEND_TITLE," << "legend" << "\n";
	file_ << "LEGEND_SHAPES,1\n";
	file_ << "LEGEND_COLORS," << color << "\n";
	file_ << "LEGEND_LABELS," << label << "\n";

}

int roundTo100(long numToRound)
{
    if (100 == 0)
        return numToRound;

    long remainder = numToRound % 100;
    if (remainder == 0)
        return numToRound;

    return numToRound + 100 - remainder;
}

std::string
ITOLWriter::calculateScale(long maxValue)
{
	auto stepWidth = roundTo100(maxValue)/5;
	std::string scale = "DATASET_SCALE";
	for (int i=1; i<=5; ++i)
	{
		scale += "," + std::to_string(i*stepWidth);
	}
	return scale;
}

void
ITOLWriter::writeValues(const std::vector<ITOLValueSet> &values)
{
	long maxValue = 0;
	for (const auto &elem : values)
	{
		maxValue = std::max(elem.value, maxValue);
	}
	file_ << calculateScale(maxValue) << "\n";
	file_ << "DATA\n";
	for (const auto &elem : values)
	{
		file_ << elem.id << "," << std::to_string(elem.value) << "\n";//"," << elem.label << "\n";
	}
}
