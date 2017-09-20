#pragma once
#include <vector>

class Merger
{
public:
	Merger();
	~Merger();

	CString m_strDir;
	CString m_strMergingFileName;
	CFile m_readingFile;
	CFile m_writingFile;
	int m_csvHeadLength;
	std::vector<CString> m_vectorFiles;


	void printCSVHead();
	void openOutputFile();
	void closeOutputFile();

	void mergeFiles();

	void sortFilesByTime(); //and generate m_strMergingFileName

	void supplementExtFileName();

};

