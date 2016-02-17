#ifndef __GByteStream_h__
#define __GByteStream_h__

#include "GStream.h"
#include <QBuffer>

class GByteStream : public GStream
{
public:

	GByteStream();
	virtual ~GByteStream();

	inline virtual gint64 size();
	inline virtual void close();
	inline virtual bool seek( gint64 position );
	inline virtual gint64 pos() const;
	inline virtual void reset();

	// Unhide inherited read method
	using GStream::write;

	virtual gint64 write( const char* pBuffer, gint64 bufLen );

	// Unhide inherited read method
	using GStream::read;

	virtual gint64 read(char* pBuffer, gint64 bufLen);

	inline virtual bool atEnd() const;

	void resize( gint64 newSize );

private:

	GByteStream( const GByteStream& rhs );				// cannot be copied
	GByteStream& operator=( const GByteStream& rhs );	// nor assigned

protected:

	QBuffer bbuf_;
};

#endif // __GByteStream_h__
