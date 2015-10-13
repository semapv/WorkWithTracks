#ifndef GEO_H
#define GEO_H
#include "QDateTime"

class Geo
{
public:
    struct PointData {
        double longitude;
        double latitude;
        QDateTime dt;

        PointData (): longitude(0), latitude(0) {}
        bool operator<(const PointData &other) const
        {
            return this->dt < other.dt;
        }

        bool isValid()
        {
            return dt.isValid();
        }
    };
    typedef QList<PointData> PointList;

    Geo();
    static double distance(const PointData &pos1, const PointData &pos2);
    static double distance(const PointList &treck);
    static double avrSpeed_ms(const PointList &treck);
    static double avrSpeed_ms(const PointData &pos1, const PointData &pos2);
    static double avrSpeed_kmh(const PointList &treck);
    static double avrSpeed_kmh(const PointData &pos1, const PointData &pos2);
    static double avrPace(const PointList &treck);
    static double avrPace(const PointData &pos1, const PointData &pos2);

private:
    static time_t timeSec(const PointList &treck);
    static double degToRad(const double &deg);
};

#endif // GEO_H
