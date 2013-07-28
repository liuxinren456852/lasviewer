#include "stdafx.h"
#include "LDSIterator.h"

LDSIterator::LDSIterator(liblas::Reader reader):
	m_reader(reader), m_state(Normal)
{
	this->operator++();
}

LDSIterator& LDSIterator::operator ++()
{
	m_state = m_reader.ReadNextPoint() ? Normal : End;

	return *this;
}

const LDSIterator LDSIterator::operator ++(int)
{
	LDSIterator tmp = *this;
	this->operator++();

	return tmp;
}

const liblas::Point& LDSIterator::operator* ()
{
	return m_reader.GetPoint();
}

bool LDSIterator::operator !=(State s)
{
	return m_state != s;
}