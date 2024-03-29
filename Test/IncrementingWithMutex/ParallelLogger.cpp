#include "ParallelLogger.h"

#include <fstream>
#include <string>

std::unordered_map<std::string, std::mutex> ParallelLogger::g_mutex_map;

ParallelLogger::ParallelLogger(const std::string & fileName)
	:m_file_name{ fileName }
{}

void ParallelLogger::SetFileName(const std::string & fileName)
{
	m_file_name = fileName;
}

void ParallelLogger::Log(const std::string & label)
{
	lock_g lock{ g_mutex_map[m_file_name] };
	std::ofstream output_file{ m_file_name.c_str(), std::ios_base::app };
	output_file << label << std::endl;
}