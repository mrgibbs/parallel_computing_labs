#!/usr/bin/python

MATRIX_SIZE = 8    #  matrix will be MATRIX_SIZE x MATRIX_SIZE
MAX_ITER = 100 # amount of additions of rows multipied by coefficient
OUTPUT_FILE_NAME = "data/random_linear_system.txt"
AMOUNT_OF_FILES_TO_SPLIT = None
SPLIT_FILE_NAME_TEMPLATE = "data/ps/p"

B_VECTOR_NUMBERS_AMPLITUDE = 10.0   #max (and min = -max) values of b vector random numbers
ITER_MAX_COEF_AMPLITUDE = 2.5      #max (and min = -max) values of coefficients for iterations (multiplication)

import random
import argparse
import sys
import os

def parse_input_args():
	parser = argparse.ArgumentParser()
	parser.add_argument("-s", "--size", type=int,
	                    help="matrix size (amount of rows and columns)")
	parser.add_argument("-i", "--iterations", type=int,
	                    help=("amount of iterations (during one iteration "
	                    	+ "row is multiplied by coefficients and is added to another"))
	parser.add_argument("-o", "--outfile", type=int,
	                    help="output file to write generated matrix")
	parser.add_argument("-n", "--nsplit", type=int,
	                    help="amount of files to split matrix")
	parser.add_argument("-t", "--split_template", type=int,
	                    help="template name for split file")
	args = parser.parse_args()

	global MATRIX_SIZE
	if args.size != None:
		if args.size > 1:
			MATRIX_SIZE = args.size
		else:
			print "Error. Matrix size must be greater than 1"
			sys.exit(1)

	global MAX_ITER
	if args.iterations != None:
		if args.iterations > 1:
			MAX_ITER = args.iterations
		else:
			print "Error. Number of iterations must be greater than 1"
			sys.exit(1)

	global OUTPUT_FILE_NAME
	if args.outfile != None:
		OUTPUT_FILE_NAME = args.outfile

	global AMOUNT_OF_FILES_TO_SPLIT
	if args.nsplit != None:
		if args.nsplit > 1 and args.nsplit <= MATRIX_SIZE:
			AMOUNT_OF_FILES_TO_SPLIT = args.nsplit
		else:
			print "Error. Number of files to split into must be greater than 1 and less or equal than matrix size"
			sys.exit(1)

	global SPLIT_FILE_NAME_TEMPLATE
	if args.split_template != None:
		SPLIT_FILE_NAME_TEMPLATE = args.split_template


def system_to_str(A, b):
	system_str = "A:\n"
	for row in A:
		system_str += str(row) + "\n"

	system_str += "\nb: " + str(b)
	return system_str

def write_row_in_file(outfile, row, b):
	for i in row:
		outfile.write(str(i) + " ")
	outfile.write(str(b) + "\n")


def main():
	parse_input_args()

	A = [];    #matrix of coefficients
	b = [];	   #right column
	answers_vector = []

	for i in range(MATRIX_SIZE):      #create diagonal matrix and fill b vector with random numbers
		row = [0] * MATRIX_SIZE
		row[i] = 1.0
		A.append(row)
		b.append(random.uniform(-B_VECTOR_NUMBERS_AMPLITUDE, B_VECTOR_NUMBERS_AMPLITUDE))

	answers_vector = b[:]
	# print "initial system:"
	# print_system(A, b)

	for i in range(MAX_ITER):        #start multipliyng and adding rows
		src_row_index = random.randint(0, MATRIX_SIZE - 1)
		dest_row_index = random.randint(0, MATRIX_SIZE - 1)

		coef = random.uniform(-ITER_MAX_COEF_AMPLITUDE, ITER_MAX_COEF_AMPLITUDE) #coefficient

		src_row = A[src_row_index]
		dest_row = A[dest_row_index]

		for j in range(MATRIX_SIZE):
			dest_row[j] += src_row[j] * coef

		b[dest_row_index] += b[src_row_index] * coef

		# print "\n\niter #" + str(i) + ":"
		# print "src row index: " + str(src_row_index)
		# print "dest row index: " + str(dest_row_index)
		# print "coefficient: " + str(coef)
		# print_system(A, b)


	
	#write generated matrix to file
	directory = os.path.dirname(OUTPUT_FILE_NAME)
	if not os.path.exists(directory):
	    os.makedirs(directory)
	out = open(OUTPUT_FILE_NAME, "w")

	out.write("System of linear equations:\n\n")
	out.write(system_to_str(A, b))
	out.write("\n\n\nCorrect answers:\n")
	out.write(str(answers_vector))
	out.close()


	#split generated matrix into several files
	if AMOUNT_OF_FILES_TO_SPLIT != None:
		# part_size = MATRIX_SIZE / AMOUNT_OF_FILES_TO_SPLIT
		split_files = []
		for i in range(AMOUNT_OF_FILES_TO_SPLIT):
			fname = SPLIT_FILE_NAME_TEMPLATE + str(i)
			directory = os.path.dirname(fname)
			if not os.path.exists(directory):
			    os.makedirs(directory)
			split_file = open(fname, "w")
			split_files.append(split_file)

		for i in range(MATRIX_SIZE):
			split_file_index = i % AMOUNT_OF_FILES_TO_SPLIT
			write_row_in_file(split_files[split_file_index], A[i], b[i])

		for i in range(AMOUNT_OF_FILES_TO_SPLIT):
			split_files[i].close()


if __name__ == '__main__':
	main()