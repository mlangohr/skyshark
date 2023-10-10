SELECT * FROM states;

SELECT icao24, time_position, callsign FROM states;

SELECT icao24,time_position, last_contact, longitude, latitude, (velocity * 3.6 / 1.852) AS velocity, true_track, vertical_rate, callsign, on_ground,  spi, squawk, baro_altitude, geo_altitude, last_contact FROM states;

SELECT icao24, callsign, time_position, longitude, latitude, velocity, true_track, baro_altitude FROM states WHERE icao24 like '3cc492';

SELECT icao24, callsign, time_position longitude, latitude, baro_altitude FROM states WHERE squawk = 7500 OR squawk = 7900 OR squawk = 7700;

SELECT icao24, callsign, time_position, longitude, latitude, baro_altitude FROM states WHERE baro_altitude > 2500 AND (vertical_rate < -25.4 OR ((squawk = 7600 OR squawk = 7700) AND vertical_rate < -10.15));

SELECT icao24, callsign, time_position, longitude, latitude, velocity, true_track, baro_altitude FROM states WHERE callsign LIKE 'DLH%';

SELECT states.icao24, states.callsign, states.time_position, (latitude <= 50.7 AND latitude >= 49.2 AND longitude >= 6.4 AND longitude <= 10.8 AND baro_altitude > 3048 AND baro_altitude < 9124.4) AS inside_airspace FROM states;

SELECT position.icao24, position.callsign, position.time_position, position.latitude, position.longitude, position.baro_altitude,
        ((position.airport_latitude_km - (position.state_latitude_km)) * (position.airport_latitude_km - (position.state_latitude_km)))
            + ((airport_longitude_km - (state_longitude_km)) * (airport_longitude_km - (state_longitude_km))) as distance_squared
FROM (
    SELECT  icao24, callsign, time_position, latitude, longitude, baro_altitude,
        (latitude * 111.139) AS state_latitude_km,
        ((1001.879 / 9) * (0.0129281 * longitude + 1.2) * longitude) AS state_longitude_km,
        (50.036521 * 111.139) AS airport_latitude_km,
        ((1001.879 / 9) * (0.0129281 * 8.561268 + 1.2) * 8.561268) AS airport_longitude_km
FROM states
     ) as position
WHERE ((position.airport_latitude_km - (position.state_latitude_km)) * (position.airport_latitude_km - (position.state_latitude_km)))
          + ((airport_longitude_km - (state_longitude_km)) * (airport_longitude_km - (state_longitude_km))) < 625;

SELECT states.*, airframes.registration, airframes.type, airframes.owner FROM states JOIN airframes ON states.icao24 = airframes.icao24;

SELECT extract('hour' from make_timestamp(time_position::BIGINT*1000000)) as Time, icao24, COUNT(*) as count FROM states GROUP BY Time, icao24;

SELECT AVG(states.time_position - states.last_contact) OVER (ROWS BETWEEN 500 PRECEDING AND 500 FOLLOWING ) as lc_latency_avg FROM states;

SELECT icao24, callsign, time_position, ident as airport, ((airports_latitude_km - (state_latitude_km)) * (airports_latitude_km - (state_latitude_km)) )
       + ((airports_longitude_km - (states_longitude_km)) * (airports_longitude_km - (states_longitude_km))) as distance_to_airport_squared
FROM (
    SELECT  states.icao24, states.callsign, states.time_position,
            (states.latitude * 111.139) AS state_latitude_km,
            ((1001.879 / 9)*(0.0129281 * states.longitude + 1.2) * states.longitude) AS states_longitude_km
    FROM states
    WHERE NOT states.on_ground
     ) as state_position
JOIN (
    SELECT  airports.ident,airports.type,
            (airports.latitude * 111.139) as airports_latitude_km,
            ((1001.879 / 9)*(0.0129281 * airports.longitude + 1.2) * airports.longitude) AS airports_longitude_km
    FROM airports
    WHERE airports.type like 'large_airport'
) as airport_position
ON ((airports_latitude_km - (state_latitude_km)) * (airports_latitude_km - (state_latitude_km)) )
       + ((airports_longitude_km - (states_longitude_km)) * (airports_longitude_km - (states_longitude_km))) < 62500;


SELECT states.icao24, states.latitude, states.longitude, states.velocity, states.vertical_rate, states.callsign, states.on_ground, states.baro_altitude, airports.latitude, airports.longitude
FROM states
JOIN schedules ON states.callsign = schedules.callsign
JOIN airports ON schedules.arr_airport = airports.ident;

