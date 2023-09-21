# GET FLIGHTS
# Author: Maximilian Langohr
# E-Mail: Maximilian.Langohr@fau.de

import requests
import csv
import os
import time
import logging
import argparse
from rich.console import Console

console = Console()
url = "https://opensky-network.org/api/routes?callsign="
header = ["callsign", "from", "to", "updateTime", "operatorIata", "flightNumber"]


# GET ROUTES
#
# This tool extracts the flights stored in OpenSky Network. Sometimes it takes multiple runs to find all flights.
# You can use the callsigns, that did not match to start a new run.

def main(callsigns: str, output_folder: str):
	row_count = count_lines_in_csv(callsigns)
	total_counter = 0
	error_counter = 0

	if not os.path.exists(output_folder):
		os.makedirs(output_folder)

	file = open(callsigns, "r")
	reader = csv.reader(file)

	# file containing all flights that are currently available on OpenSky
	flight_out_file = open(output_folder + "routes.csv", "a", newline='')
	flight_writer = csv.DictWriter(flight_out_file, header)
	flight_writer.AutFlush = True

	# contains all callsigns where no flight was store in OpenSky
	# You can rerun the program later with these callsigns and try to find more flights.
	no_flight_out_file = open(output_folder + "no_routes.csv", "a", newline='')
	no_flight_writer = csv.writer(no_flight_out_file)

	for row in reader:
		try:
			final_url = url + row[0].strip()
			result = requests.get(final_url)
			if result.status_code == 404 or result.status_code == 500 or result.status_code == 502:
				api_error_string = "Response code: " + str(result.status_code)
				logging.error(api_error_string)
				console.log(api_error_string)
				error_counter += 1
				status_string = "Successfully requested: " + str(total_counter) + " | Error count: " + str(
					error_counter) + " | of " + str(row_count)
				console.log(status_string)
				logging.info(status_string)
				no_flight_writer.writerow([row[0].strip()])
				no_flight_out_file.flush()
			else:
				json_data = result.json()
				data = dict()
				data["callsign"] = json_data["callsign"].strip()
				data["from"] = json_data["route"][0]
				data["to"] = json_data["route"][1]
				data["updateTime"] = json_data["updateTime"]
				data["operatorIata"] = json_data["operatorIata"]
				data["flightNumber"] = json_data["flightNumber"]
				flight_writer.writerow(data)
				total_counter += 1
				status_string = "Successfully requested: " + str(total_counter) + " | Error count: " + str(
					error_counter) + " | of " + str(row_count)
				console.log(status_string)
				logging.info(status_string)
		except Exception as e:
			error_counter += 1
			status_string = "Successfully requested: " + str(total_counter) + " | Error count: " + str(
				error_counter) + " | of " + str(row_count)
			error_string = "Error storing flights: " + str(e)
			logging.info(status_string)
			console.log(status_string)
			logging.error(error_string)
			console.log(error_string)
			time.sleep(1)
	flight_out_file.close()
	no_flight_out_file.close()


def count_lines_in_csv(file_path):
	line_count = 0
	with open(file_path, 'r') as csv_file:
		csv_reader = csv.reader(csv_file)
		for line in csv_reader:
			line_count += 1
	return line_count


# -i Path to callsigns.csv from "extract callsigns"
# -o Output folder to output the extracted flights
if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="SKYSHARK GET FLIGHTS")
	parser.add_argument("-i", action="store", dest="callsigns", default="./data/callsigns.csv",
						help="Path to callsigns.csv from extract callsigns")
	parser.add_argument("-o", action="store", dest="output_folder", default="./data", help="Output folder")
	args = parser.parse_args()

	logging.basicConfig(filename=args.output_folder + "flight_logging.log", encoding="utf-8", level=logging.INFO,
						format='%(asctime)s | %(levelname)s | %(message)s',
						datefmt='%m/%d/%Y %I:%M:%S %p')
	console.rule("[bold red] Starting to get routes from OpenSky")
	main(args.callsigns, args.output_folder)
	console.rule("[bold red] Done getting routes from OpenSky")
