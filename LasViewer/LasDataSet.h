/********************************************************************
	created:	2013/07/28
	created:	28:7:2013   11:08
	filename: 	D:\My Programs\LasViewer\LasViewer\LasDataSet.h
	file path:	D:\My Programs\LasViewer\LasViewer
	file base:	LasDataSet
	file ext:	h
	author:		Gao Guang
	
	purpose:	
*********************************************************************/

#ifndef LasDataSet_h__
#define LasDataSet_h__

#include <fstream>
#include <boost/noncopyable.hpp>

class CLasDataSet : private boost::noncopyable
{
public:
	CLasDataSet(){}

	int					Open(std::string filename);

	void				GetXYZ(double *pXYZ);
	
private:
	bool				Open();

private:
	std::string			m_filename;
};
#endif // LasDataSet_h__