#ifndef XMLPARSE_H
#define XMLPARSE_H
#include "QXmlStreamReader"
#include "geo.h"

class GpxParse
{
public:

    GpxParse(const QString &fileName);
    QString error() const;
    QString outMsg() const;
    Geo::PointList treckData() const;
    ~GpxParse();

private:
    typedef QMap<QString, QString> StrMap;

    bool init(const QString &name);
    void setError(const QString &errmsg);
    bool openXml(const QString &name);

    bool parse();
    bool mapToStruct();
    StrMap parseEtap(QXmlStreamReader &xml);
    void addElementDataToMap(QXmlStreamReader &xml, StrMap &map) const;

    QString fillMsg(const Geo::PointList &elms) const;

    QString m_errorText;
    QXmlStreamReader m_xml;
    QList<StrMap > m_elements;
    Geo::PointList m_points;
};

#endif // XMLPARSE_H
