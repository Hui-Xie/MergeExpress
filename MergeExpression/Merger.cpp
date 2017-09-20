#include "stdafx.h"
#include "Merger.h"
#include <algorithm>


Merger::Merger()
{

}


Merger::~Merger()
{
}

void Merger::openOutputFile(){
   SetCurrentDirectory(m_strDir);
   BOOL isOpen = m_writingFile.Open(m_strMergingFileName, CFile::modeCreate | CFile::modeWrite);
   if (!isOpen) {
	   AfxMessageBox(_T("Output File can't be created."));
   }
}


void Merger::printCSVHead(){
	std::string csvHead = ""
			"FileName, Frame#, Time, "
			"FaceboxX, FaceboxY, FaceboxW, FaceboxH, "
			"Pitch, Yaw, Roll, "
			"Glasses(Evidence), Glasses(Probability), "
			"Gender(Evidence), Gender(Probability), "
			"Anger(Evidence), Anger(Intensity), Contempt(Evidence), Contempt(Intensity),	Disgust(Evidence), Disgust(Intensity), Joy(Evidence), Joy(Intensity), "
			"Fear(Evidence), Fear(Intensity), Sadness(Evidence), Sadness(Intensity), Surprise(Evidence), Surprise(Intensity), "
			"Frustration(Evidence), Frustration(Intensity), Confusion(Evidence), Confusion(Intensity), "
			"Negative(Evidence), Negative(Intensity), Neutral(Evidence), Neutral(Intensity), Positive(Evidence), Positive(Intensity), "
			"AU1, AU2, AU4, AU5, AU6, AU7, AU9, AU10, AU12, AU14, "
			"AU15, AU17, AU18, AU20, AU23, AU24, AU25, AU26, AU28, ";
 	csvHead.append("\r\n");
	m_csvHeadLength = csvHead.size();
	m_writingFile.Write(csvHead.c_str(), m_csvHeadLength);
	m_writingFile.Flush();
}


void Merger::mergeFiles(){
	int const NumFiles = m_vectorFiles.size();
	if (NumFiles <= 1) return;

	supplementExtFileName(); //this must come before sortFilesByTime();

	sortFilesByTime();//and generate m_strMergingFileName
	
	openOutputFile();
	printCSVHead();
	
	UINT  fileLength = 0;
	UINT readingLength = 0;
	char *pBuffer;
	BOOL isOpen;

	for (int i = 0; i < NumFiles; i++){
		//read file
		isOpen = m_readingFile.Open(m_vectorFiles.at(i), CFile::modeRead);
		if (!isOpen) {
			AfxMessageBox(m_vectorFiles.at(i) + " can't open.");
		}
		fileLength = (UINT) m_readingFile.GetLength();
		m_readingFile.Seek(m_csvHeadLength, CFile::begin);
		pBuffer = new char[fileLength];
		readingLength = m_readingFile.Read(pBuffer, fileLength);
		pBuffer[readingLength] = '\0';
		m_readingFile.Close();

		//Write file
		m_writingFile.Write(pBuffer, readingLength);
		m_writingFile.Flush();

		delete pBuffer;
	}

	closeOutputFile();

}


void Merger::sortFilesByTime(){
	struct {
		bool operator()(CString file1, CString file2)
		{
			CFileStatus rStatus;
			CTime file1Time, file2Time;
			
			CFile::GetStatus(file1,rStatus);
			file1Time = rStatus.m_mtime; //last modifying time instead of created time
			CFile::GetStatus(file2, rStatus);
			file2Time = rStatus.m_mtime; //last modifying time instead of created time

			return (file1Time < file2Time);
		}
	} CompareCreatedTime;
	std::vector<CString> tempVector = m_vectorFiles;
	std::sort(tempVector.begin(), tempVector.end(), CompareCreatedTime);

	m_vectorFiles.clear();
	std::vector<CString>::iterator it;
	for (it = tempVector.begin(); it != tempVector.end(); ++it){
		m_vectorFiles.push_back(*it);
	}

	m_strMergingFileName.Empty();
	m_strMergingFileName.Append(CString("ExpressMerged")
		+ m_vectorFiles.at(0).Mid(m_vectorFiles.at(0).Find('_'), 16)
		+ m_vectorFiles.at(m_vectorFiles.size()-1).Right(12));
}


void Merger::closeOutputFile(){
	m_writingFile.Close();

}

void Merger::supplementExtFileName(){
	int nSize = m_vectorFiles.size();
	CString const csvExtName(".csv");
	for (int i = 0; i < nSize; i++){
		if (0 != csvExtName.CompareNoCase(m_vectorFiles.at(i).Right(4))){
			m_vectorFiles.at(i) += csvExtName;
		}
	}
}
