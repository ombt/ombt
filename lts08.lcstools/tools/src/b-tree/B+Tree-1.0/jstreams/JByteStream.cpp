#include "GByteStream.h"

GByteStream::GByteStream()
{}

GByteStream::~GByteStream()
{
	close();
}

inline gint64 GByteStream::size()
{
	return bbuf_.size();
}

void GByteStream::close()
{
	bbuf_.close();
}

bool GByteStream::seek( gint64 position )
{
	return bbuf_.seek( position );
}

gint64 GByteStream::pos() const
{
	return bbuf_.pos();
}

void GByteStream::reset()
{
	bbuf_.reset();
}

gint64 GByteStream::write( const char* pBuffer, gint64 bufLen )
{
	return bbuf_.write( pBuffer, bufLen );
}

gint64 GByteStream::read(char* pBuffer, gint64 bufLen)
{
	return bbuf_.read( pBuffer, bufLen );
}

inline bool GByteStream::atEnd() const
{
	return bbuf_.atEnd();
}

void GByteStream::resize( gint64 newSize )
{
	bbuf_.buffer().resize( newSize );
}

