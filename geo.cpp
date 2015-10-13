#include "geo.h"
#include "math.h"
const double pi = 3.14159265358979323846;
/*
   function distance($lat1,$lng1,$lat2,$lng2)
     {
         // Convert degrees to radians.
        $lat1=deg2rad($lat1);
        $lng1=deg2rad($lng1);
        $lat2=deg2rad($lat2);
        $lng2=deg2rad($lng2);

        // Calculate delta longitude and latitude.
        $delta_lat=($lat2 - $lat1);
        $delta_lng=($lng2 - $lng1);

        return round( 6378137 * acos( cos( $lat1 ) * cos( $lat2 ) * cos( $lng1 - $lng2 ) + sin( $lat1 ) * sin( $lat2 ) ) );
     }
  */

Geo::Geo()
{

}

double Geo::degToRad(const double &deg)
{
    return deg * pi / 180;
}

double Geo::distance(const PointData &pos1, const PointData &pos2)
{
    double rLat1 = degToRad(pos1.latitude);
    double rLon1 = degToRad(pos1.longitude);
    double rLat2 = degToRad(pos2.latitude);
    double rLon2 = degToRad(pos2.longitude);
    double dist = 6378137 * acos(cos(rLat1) * cos(rLat2) * cos(rLon1 - rLon2) + sin(rLat1) * sin(rLat2));
    return dist;
}

double Geo::distance(const PointList &treck)
{
    double dist(0);
    PointData previosPos;
    foreach (PointData pos, treck) {
        if(previosPos.dt.isValid()){
            dist += distance(previosPos, pos);
        }
        previosPos = pos;
    }
    return dist;
}

double Geo::avrSpeed_kmh(const Geo::PointList &treck)
{
    return avrSpeed_ms(treck) * 3.6;
}

double Geo::avrSpeed_kmh(const Geo::PointData &pos1, const Geo::PointData &pos2)
{
    PointList treck;
    treck << pos1 << pos2;
    return avrSpeed_kmh(treck);
}

double Geo::avrPace(const Geo::PointList &treck)
{
    double pace(0);
    time_t time = timeSec(treck);
    double dist = distance(treck);
    if(time > 0 && dist > 0) {
        pace = (time / 60) / (dist / 1000);
    }
    return pace;
}

double Geo::avrPace(const Geo::PointData &pos1, const Geo::PointData &pos2)
{
    PointList treck;
    treck << pos1 << pos2;
    return avrPace(treck);
}

time_t Geo::timeSec(const Geo::PointList &treck)
{
    time_t time(0);
    if(!treck.isEmpty()) {
        time_t firstSek = treck.first().dt.toTime_t();
        time_t lastSek = treck.last().dt.toTime_t();
        time = lastSek - firstSek;
    }
    return time;
}

double Geo::avrSpeed_ms(const Geo::PointList &treck)
{
    double speed(0);
    if(!treck.isEmpty()) {
        double dist = distance(treck);
        time_t dSec = timeSec(treck);
        speed = dSec > 0 ? (dist / dSec) : 0;
    }
    return speed;
}

double Geo::avrSpeed_ms(const Geo::PointData &pos1, const Geo::PointData &pos2)
{
    PointList treck;
    treck << pos1 << pos2;
    return avrSpeed_ms(treck);
}

