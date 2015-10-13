#include "xmlparse.h"
#include "qfile.h"
#include "qdebug.h"




GpxParse::GpxParse(const QString &fileName)
{
    if(!init(fileName))
        qDebug() << "Error: " << QString(error());
}

QString GpxParse::error() const
{
    return m_errorText;
}

bool GpxParse::init(const QString &name)
{
    bool result = openXml(name);
    result = result ? parse() : false;
    result = result ? mapToStruct() : false;
    return result;

}

bool GpxParse::openXml(const QString &name)
{
    QFile* file = new QFile(name);
    bool isOk = file->open(QIODevice::ReadOnly | QIODevice::Text);
        if (!isOk)
        {
            setError("Невозможно открыть XML-конфиг");
            return false;
        }
        m_xml.setDevice(file);
        return true;
}

bool GpxParse::parse()
{
    //QList<StrMap > elements;
    while (!m_xml.atEnd() && !m_xml.hasError()){
        QXmlStreamReader::TokenType token = m_xml.readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement){
            if (m_xml.name() == "trkpt")
                m_elements.append(parseEtap(m_xml));
        }
    }
    if(m_xml.hasError()) {
        setError("Ошибка парсинга элементов");
        return false;
    }
    return true;
}

bool GpxParse::mapToStruct()
{
    bool isOk = true;
    Geo::PointList list;
    foreach (StrMap map, m_elements) {
        Geo::PointData data;
        data.latitude = map.value("lat").toDouble(&isOk);
        if(!isOk){
            setError(QString("Ошибка конвертации данных: lat %1").arg(map.value("lat")));
            break;
        }
        data.longitude = map.value("lon").toDouble(&isOk);
        if(!isOk){
            setError(QString("Ошибка конвертации данных: lon %1").arg(map.value("lon")));
            break;
        }
        data.dt = QDateTime::fromString(map.value("time"),"yyyy-MM-ddTHH:mm:ss.zzzZ"); //2015-09-22T06:04:18.000Z
        isOk = data.dt.isValid();
        if(!isOk){
            setError(QString("Ошибка конвертации данных: time %1").arg(map.value("time")));
            break;
        }
        list.append(data);
        qSort(list);
    }
    if(isOk)
        m_points = list;
    return isOk;
}

QString GpxParse::fillMsg(const Geo::PointList &elms) const
{
    QString str;
    if(!elms.isEmpty()) {
        Geo::PointData preData;
        foreach(Geo::PointData data, elms) {
                str += QString("%1* %2").arg(str.isEmpty() ? "" : "\n")
                        .arg(QString("lo:%1\t la:%2\t t:%3\t d:%4 м\t s:%5 км/ч")
                             .arg(data.longitude)
                             .arg(data.latitude)
                             .arg(data.dt.toString())
                             .arg(preData.isValid() ? Geo::distance(preData, data) : 0)
                             .arg(preData.isValid() ? Geo::avrSpeed_kmh(preData, data) : 0));
                preData = data;
        }
    }
    return str;
}

GpxParse::StrMap GpxParse::parseEtap(QXmlStreamReader &xml)
{

    StrMap etap;
    if (xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "trkpt")
        return etap;

    QXmlStreamAttributes attributes = xml.attributes();
        if (attributes.hasAttribute("lon"))
            etap["lon"] = attributes.value("lon").toString();
        if (attributes.hasAttribute("lat"))
            etap["lat"] = attributes.value("lat").toString();
        xml.readNext();
        while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "trkpt"))
        {
            if (xml.tokenType() == QXmlStreamReader::StartElement)
            {
                if (xml.name() == "time")
                    addElementDataToMap(xml, etap);
            }
            xml.readNext();
        }
        return etap;
}

void GpxParse::addElementDataToMap(QXmlStreamReader& xml, StrMap& map) const
{
    if (xml.tokenType() != QXmlStreamReader::StartElement)
        return;
    QString elementName = xml.name().toString();
    xml.readNext();
    map.insert(elementName, xml.text().toString());
}


QString GpxParse::outMsg() const
{
    if(!m_errorText.isEmpty()) {
        return m_errorText;
    }
    return fillMsg(m_points);
}

Geo::PointList GpxParse::treckData() const
{
    return m_points;
}

GpxParse::~GpxParse()
{
    m_xml.device()->close();
    delete (m_xml.device());
}

void GpxParse::setError(const QString &errmsg)
{
    m_errorText = errmsg;
}
