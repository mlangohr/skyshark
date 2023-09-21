import json


# Represents a state downloaded from OpenSky
class State:
	icao24: str
	callsign: str
	origin_country: str = None
	time_position: int = None
	last_contact: int = None
	longitude: float = None
	latitude: float = None
	baro_altitude: float = None
	on_ground: str = None
	velocity: str = None
	true_track: float = None
	vertical_rate: float = None
	sensors: str = None
	geo_altitude: float = None
	squawk: int = None
	spi: str = None
	position_source: int = None

	def __init__(self, row: list) -> None:
		self.icao24 = row[0]
		self.callsign = row[1].strip()
		self.origin_country = row[2]
		if row[3].strip() != '':
			self.time_position = int(row[3].strip())
		else:
			self.time_position = None

		if row[4] != '':
			self.last_contact = int(row[4])
		if row[5] != '':
			self.longitude = float(row[5])
		if row[6] != '':
			self.latitude = float(row[6])
		if row[7] != '':
			self.baro_altitude = float(row[7])
		if row[8] != '':
			self.on_ground = row[8]
		if row[9] != '':
			self.velocity = float(row[9])
		if row[10] != '':
			self.true_track = float(row[10])
		if row[11] != '':
			self.vertical_rate = float(row[11])
		if row[12] != '':
			self.sensors = row[12]
		if row[13] != '':
			self.geo_altitude = float(row[13])
		if row[14] != '':
			self.squawk = int(row[14])
		if row[15] != '':
			self.spi = row[15]
		if row[16] != '':
			self.position_source = int(row[16])

	def to_list(self) -> list:
		result = list()
		result.append(self.icao24)
		result.append(self.callsign)
		result.append(self.origin_country)
		result.append(self.time_position)
		result.append(self.last_contact)
		result.append(self.longitude)
		result.append(self.latitude)
		result.append(self.baro_altitude)
		result.append(self.on_ground)
		result.append(self.velocity)
		result.append(self.true_track)
		result.append(self.vertical_rate)
		result.append(self.sensors)
		result.append(self.geo_altitude)
		result.append(self.squawk)
		result.append(self.spi)
		result.append(self.position_source)

		return result

	def __iter__(self):
		yield from {
			"icao24": self.icao24,
			"time_position": self.time_position,
			"callsign": self.callsign,
			"origin_country": self.origin_country,
			"last_contact": self.last_contact,
			"longitude": self.longitude,
			"latitude": self.latitude,
			"baro_altitude": self.baro_altitude,
			"on_ground": self.on_ground,
			"velocity": self.velocity,
			"true_track": self.true_track,
			"vertical_rate": self.vertical_rate,
			"sensors": self.sensors,
			"geo_altitude": self.geo_altitude,
			"squawk": self.squawk,
			"spi": self.spi,
			"position_source": self.position_source
		}.items()

	def __str__(self):
		return json.dumps(dict(self), ensure_ascii=False)

	def __repr_(self):
		return self.__str__()
