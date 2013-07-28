#include "stdafx.h"
#include "LasDataSet.h"
#include "LDSIterator.h"
#include <fstream>
#include <exception>
#include <liblas/liblas.hpp>

liblas::Reader OpenLasFile(std::ifstream &fin)
{
	liblas::ReaderFactory f;
	return f.CreateWithStream(fin);
}

int CLasDataSet::Open(std::string filename)
{
	m_filename = filename;

	try 
	{
		std::ifstream fin;
		fin.open(filename, std::ios::in | std::ios::binary);
		if (!fin.is_open())	return -1;
		
		liblas::Reader reader = OpenLasFile(fin);

 		int cnt = 0;
 		for (LDSIterator ite(reader); ite != LDSIterator::End; ++ite, ++cnt);

		return cnt;
	}
	catch(std::runtime_error)
	{
		return -1;
	}
}

void CLasDataSet::GetXYZ(double *pXYZ)
{
	std::ifstream fin;
	fin.open(m_filename, std::ios::in | std::ios::binary);
	if (!fin.is_open())	return ;
	
	liblas::Reader reader = OpenLasFile(fin);

	for (LDSIterator ite(reader); ite != LDSIterator::End; ++ite)
	{
		const liblas::Point& pnt = *ite;

		*pXYZ++ = pnt.GetX();
		*pXYZ++ = pnt.GetY();
		*pXYZ++ = pnt.GetZ();
	}
}