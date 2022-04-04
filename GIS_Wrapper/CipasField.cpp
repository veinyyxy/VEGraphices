#include "CipasField.h"


CipasField::CipasField( QString name, QVariant::Type type, QString typeName, int len, int prec, QString comment )
	: mName( name ), mType( type ), mTypeName( typeName ),
	mLength( len ), mPrecision( prec ), mComment( comment )
{
}


CipasField::~CipasField()
{
}

bool CipasField::operator==( const CipasField& other ) const
{
	return (( mName == other.mName ) && ( mType == other.mType ) && ( mTypeName == other.mTypeName )
		&& ( mLength == other.mLength ) && ( mPrecision == other.mPrecision ) );
}

const QString & CipasField::name() const
{
	return mName;
}

QVariant::Type CipasField::type() const
{
	return mType;
}

const QString & CipasField::typeName() const
{
	return mTypeName;
}

int CipasField::length() const
{
	return mLength;
}

int CipasField::precision() const
{
	return mPrecision;
}

const QString & CipasField::comment() const
{
	return mComment;
}

void CipasField::setName( const QString & nam )
{
	mName = nam;
}

void CipasField::setType( QVariant::Type type )
{
	mType = type;
}

void CipasField::setTypeName( const QString & typeName )
{
	mTypeName = typeName;
}

void CipasField::setLength( int len )
{
	mLength = len;
}

void CipasField::setPrecision( int prec )
{
	mPrecision = prec;
}

void CipasField::setComment( const QString & comment )
{
	mComment = comment;
}
