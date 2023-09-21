# EXTRACT CALLSIGNS
# Author: Maximilian Langohr
# E-Mail: Maximilian.Langohr@fau.de

import csv
import argparse


# EXTRACT CALLSIGNS
#
# This tool extracts the unique callsigns within the states.csv file
# These are later used to identify the flights within the dataset

def main(input_file: str, output_file: str):
	in_file = open(input_file, "r")
	out_file = open(output_file, "a")

	state_reader = csv.reader(in_file)
	callsign_writer = csv.writer(out_file)

	next(state_reader, None)
	callsigns = list()
	for row in state_reader:
		callsign = row[1].strip()

		if not callsign in callsigns:
			callsigns.append(callsign)

	print("Done reading callsigns")
	for item in callsigns:
		callsign_writer.writerow(item)

	out_file.close()
	in_file.close()


# -i Path to the input file states.csv created by SKYSHARK DOWNLOADER
# -0 Path to the output file containing all callsigns (callsigns.csv)
if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="SKYSHARK EXTRACT CALLSIGNS")
	parser.add_argument("-i", action="store", dest="input_file", help="Path to the states.csv file",
						default="./data/states.csv")
	parser.add_argument("-o", action="store", dest="output_file", help="Path to the output file",
						default="./data/callsigns.csv")
	args = parser.parse_args()
	main(args.input_file, args.output_file)
