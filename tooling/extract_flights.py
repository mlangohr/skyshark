# CREATE SCHEDULE
# Author: Maximilian Langohr
# E-Mail: Maximilian.Langohr@fau.de

import csv
import argparse
from datetime import datetime, timedelta
from state import State
from flight import Flight
from rich.console import Console

console = Console()
schedule_header = ["id", "icao24", "callsign", "first_seen", "last_seen"]


# EXTRACT FLIGHTS
#
# This tool iterates through all states collected in states.csv and identifies all flights available.
# It does this by identifying the start of a flight when the state attribute on_ground changes from true to false.
# The end of the flight is determined vice versa.
def main(input_file: str, output_file: str):
	state_in_file = open(input_file, "r")
	schedule_out_file = open(output_file, "a")

	state_reader = csv.reader(state_in_file)
	schedule_writer = csv.writer(schedule_out_file)
	schedule_writer.writerow(schedule_header)

	id = 1
	flights = {}
	next(state_reader, None)

	for row in state_reader:
		state: State = State(row)

		if state.callsign in flights:
			flight: Flight = flights[state.callsign]
			# airplane just lifted off
			if flight.last_tuple.on_ground == "True" and state.on_ground == "False":
				flight.first_seen = state.time_position
				flight.first_tuple = state
				flight.last_tuple = state
			# airplane just landed
			elif flight.last_tuple.on_ground == "False" and state.on_ground == "True":
				flight.last_seen = state.time_position
				flight.last_tuple = state
				write_flight(schedule_writer, flight)
				del flights[state.callsign]
			else:
				flight.last_tuple = state
		else:
			if state.callsign.strip() == '':
				continue
			new_flight: Flight = Flight()
			new_flight.id = id
			id += 1
			new_flight.first_seen = state.time_position
			new_flight.first_tuple = state
			new_flight.last_tuple = state
			flights[state.callsign] = new_flight
	state_in_file.close()

	schedule_out_file.close()


def write_flight(writer, flight: Flight) -> None:
	if flight.first_seen is None or flight.first_seen == '' or flight.last_seen is None or flight.last_seen == '':
		return
	values = list()
	values.append(flight.id)
	values.append(flight.last_tuple.icao24)
	values.append(flight.last_tuple.callsign)
	first_seen_obj = datetime.fromtimestamp(flight.first_seen)
	first_seen = first_seen_obj - timedelta(minutes=first_seen_obj.minute % 5, seconds=first_seen_obj.second,
											microseconds=first_seen_obj.microsecond)
	values.append(int(first_seen.timestamp()))
	last_seen_obj = datetime.fromtimestamp(flight.last_seen)
	last_seen = last_seen_obj - timedelta(minutes=last_seen_obj.minute % 5, seconds=last_seen_obj.second,
										  microseconds=last_seen_obj.microsecond)
	values.append(int(last_seen.timestamp()))
	writer.writerow(values)


# -i Path to states.csv
# -o Path to output file schedule.csv
if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="SKYSHARK CREATE SCHEDULE")
	parser.add_argument("-i", action="store", dest="input_file", default="./data/states.csv", help="Path to states.csv")
	parser.add_argument("-o", action="store", dest="output_file", default="./data/flights.csv",
						help="Path to the output file schedule.csv")
	args = parser.parse_args()
	console.rule("[bold red] Starting to extract flights")
	with console.status("Extracting flights", spinner="material"):
		main(args.input_file, args.output_file)
	console.rule("[bold red] Done extracting flights")
