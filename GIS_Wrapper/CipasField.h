#pragma once
#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QMap>
#include "GIS_WrapperConfig.h"

class GIS_WRAPPER_EXPORT_IMPORT CipasField
{
 public:
    CipasField( QString name = QString(),
              QVariant::Type type = QVariant::Invalid,
              QString typeName = QString(),
              int len = 0,
              int prec = 0,
              QString comment = QString() );
    ~CipasField();
    bool operator==( const CipasField& other ) const;
    const QString & name() const;
    QVariant::Type type() const;
    const QString & typeName() const;
    int length() const;
    int precision() const;
    const QString & comment() const;
    void setName( const QString & nam );
    void setType( QVariant::Type type );
    void setTypeName( const QString & typ );
    void setLength( int len );
    void setPrecision( int prec );
    void setComment( const QString & comment );
  private:
    QString mName;
    QVariant::Type mType;
    QString mTypeName;
    int mLength;
    int mPrecision;
    QString mComment;
};

typedef QMap<int, CipasField> FIELD_MAP, *P_FIELD_MAP;
