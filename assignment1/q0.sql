CREATE TABLE airports (
	Airport_ID INT,
	Name VARCHAR,
	City VARCHAR,
	Country VARCHAR,
	IATA_FAA CHAR(3)	NULL,
	ICAO CHAR(4)	NULL,
	Latitude FLOAT,
	Longitude FLOAT,
	Altitude INT,
	Timezone FLOAT,
	DST CHAR(1),
	Tz_database_time_zone VARCHAR,
	PRIMARY KEY(Airport_ID)
);

CREATE TABLE airlines (
	Airline_ID INT,
	Name VARCHAR,
	Alias VARCHAR,
	IATA CHAR(2)	NULL,
	ICAO CHAR(3)	NULL,
	Callsign VARCHAR,
	Country VARCHAR,
	Active CHAR,
	PRIMARY KEY(Airline_ID)
);

CREATE TABLE routes (
	Airline CHAR,
	Airline_ID INT,
	Source_airport CHAR,
	Source_airport_ID INT	NULL,
	Destination_airport CHAR,
	Destination_airport_ID INT	NULL,
	Codeshare CHAR(1)	NULL, 
	Stops INT	NULL,
	Equipments INT(3)
);

.mode csv
.import airports.csv airports
.import airlines.csv airlines
.import routes.csv routes
 