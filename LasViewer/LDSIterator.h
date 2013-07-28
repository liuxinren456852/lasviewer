/********************************************************************
	created:	2013/07/28
	created:	28:7:2013   11:42
	filename: 	D:\My Programs\LasViewer\LasViewer\LDSIterator.h
	file path:	D:\My Programs\LasViewer\LasViewer
	file base:	LDSIterator
	file ext:	h
	author:		Gao Guang
	
	purpose:	
*********************************************************************/

#ifndef LDSIterator_h__
#define LDSIterator_h__

#include <liblas/reader.hpp>

class LDSIterator
{
	typedef LDSIterator self;
public:
	enum State
	{
		Normal,
		End
	};

	LDSIterator(liblas::Reader reader);

	self&		operator ++ ();
	const self	operator ++ (int);
	
	const liblas::Point&	operator *();

	bool		operator != (State s);

private:
	typedef liblas::Reader Reader;
	Reader		m_reader;
	State		m_state;
};
#endif // LDSIterator_h__