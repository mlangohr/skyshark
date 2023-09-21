# CREATE SCHEDULE
# Author: Maximilian Langohr
# E-MAIL: Maximilian.Langohr@fau.de


import argparse
import sqlite3
import pandas as pd
from rich.console import Console

console = Console()

# CREATE SCHEDULE
#
# This tool creates an SQLite database and imports the flights and routes created in previous steps.
# These to tables are than joined to create the flight schedule.
def main(routes_file: str, flights_file: str, output_file: str):
	db = sqlite3.connect("./data/schedule.db")
	with console.status("Loading routes to db ...", spinner="material"):
		try:
			routes = pd.read_csv(routes_file, low_memory=False)
			routes.to_sql("routes", db, if_exists="replace", index=False)
		except Exception as e:
			print(e)
	with console.status("Loading flights to db ...", spinner="material"):
		try:
			flights = pd.read_csv(flights_file, low_memory=False)
			flights.to_sql("flights", db, if_exists="replace", index=False)
		except Exception as e:
			print(e)
	with console.status("Writing schedule", spinner="material"):
		df = pd.read_sql_query("SELECT flights.icao24, flights.callsign, routes.'from' as dep_airport, "
							   "flights.first_seen as dep_time, routes.'to' as arr_airport, flights.last_seen as "
							   "arr_time FROM flights JOIN routes  ON routes.callsign = flights.callsign", db)

		out_file = open(output_file, "a")
		header = "icao24,callsign,dep_airport,dep_time,arr_airport,arr_time,cur_pos_lat,cur_pos_long,time_to_destination," \
				 "flight_phase\n"
		out_file.write(header)
		for index, row in df.iterrows():
			final_string = row["icao24"] + "," + row["callsign"] + "," + row["dep_airport"] + "," + row["dep_time"] + \
						   "," + row["arr_airport"] + "," + row["arr_time"] + ",,,,\n "
			out_file.write(final_string)


if __name__ == "__main__":
	parser = argparse.ArgumentParser("CREATE SCHEDULE")
	parser.add_argument("-r", action="store", dest="routes_file", default="./data/routes.csv",
						help="Path to the routes file")
	parser.add_argument("-f", action="store", dest="flights_file", default="./data/flights.csv",
						help="Path to the flights file")
	parser.add_argument("-o", action="store", dest="output_file", default="./data/schedule.csv",
						help="Path to the output file")
	args = parser.parse_args()
	console.rule("[bold red] Starting to create schedule")
	main(args.routes_file, args.flights_file, args.output_file)
	console.rule("[bold red] Done creating schedule")
