CREATE TABLE airports  as FROM read_csv_auto('airports.csv');
CREATE TABLE schedules as FROM read_csv_auto('flight_schedule.csv');
CREATE TABLE airframes as FROM read_csv_auto('aircrafts.csv', header=TRUE);

-- manually creating states table because file has no header
CREATE TABLE states (
icao24          string,
callsign        string,
origin_country  string,
time_position   integer,
last_contact    integer,
longitude       double,
latitude        double,
baro_altitude   double,
on_ground       string,
velocity        double,
true_track      double,
vertical_rate   double,
sensors         string,
geo_altitude    double,
squawk          smallint,
spi             string,
position_source bigint);

INSERT INTO states FROM read_csv_auto('states.csv.gz', delim=',', quote='')