/*
 * TempFile.cpp
 *
 *  Created on: 06.06.2015
 *      Author: marius
 */

#include "../../include/gerbil/TempFile.h"

#include <errno.h>

using namespace std;

gerbil::uint_tfn gerbil::TempFile::__nextId = 0;

gerbil::TempFile::TempFile()
		: _size(0),
		  _file(NULL),
		  _filled(0),
		  _kmers(0),
		  _btUkmers(0),
		  _smers(0),
		  _ukmers(0),
		  _filename(""),
		  _numberOfRuns(0) {
	_id = __nextId++;
}

gerbil::TempFile::~TempFile() {

}

// open file (write mode)
bool gerbil::TempFile::openW(const std::string filename) {
	_filename = filename;
	// remove old Files
	std::remove(_filename.c_str());
	_file = fopen(_filename.c_str(), "wb");
	if (!_file) {
		perror(_filename.c_str());
		printf("Error %d \n", errno);
		return false;
	}
	setbuf(_file, NULL);
	return true;
}

// open file (read mode)
bool gerbil::TempFile::openR() {
	_file = fopen(_filename.c_str(), "rb");
	if (!_file)
		return false;
	setbuf(_file, NULL);
	return true;
}

// remove old file
bool gerbil::TempFile::remove() {
	return std::remove(_filename.c_str()) != 0;
}

void gerbil::TempFile::reset() {
	rewind(_file);
}

// LINGXI: record time stamp to calculate IO bubbles
bool gerbil::TempFile::write(SuperBundle *superBundle) {
	
	// //// ORIGINAL //
	// _size += SUPER_BUNDLE_DATA_SIZE_B;
	// _smers += superBundle->sMerNumber;
	// _kmers += superBundle->kMerNumber;
	// _filled += superBundle->getSize();
	

	// return fwrite((char *) superBundle->data, 1, SUPER_BUNDLE_DATA_SIZE_B, _file) == SUPER_BUNDLE_DATA_SIZE_B;
	// //// END ORIGINAL // 

	_size += SUPER_BUNDLE_DATA_SIZE_B;
	_smers += superBundle->sMerNumber;
	_kmers += superBundle->kMerNumber;
	_filled += superBundle->getSize();
	uint64_t result; 

	// timer start
	// SimpleTimer timer;
	// timer.reset();
	// timer.start();
	
	result = fwrite((char *) superBundle->data, 1, SUPER_BUNDLE_DATA_SIZE_B, _file);

	// timer end
	// timer.stop();

	// ofstream bubble_fwrite_run1;
	// bubble_fwrite_run1.open("bubble_fwrite_run1.csv", ios::app);
	// bubble_fwrite_run1 << timer.tv1.tv_sec << "," << timer.tv1.tv_usec << endl; // start time
	// bubble_fwrite_run1 << timer.tv2.tv_sec << "," << timer.tv2.tv_usec << endl; // end time
	// bubble_fwrite_run1.close();

	if(result == SUPER_BUNDLE_DATA_SIZE_B){
		return true;
	} else {
		return false;
	}
}

bool gerbil::TempFile::write(char *data, const uint64 &size, const uint64 &smers, const uint64 &kmers,
                             const uint64 &filled) {
	_size += size;
	_smers += smers;
	_kmers += kmers;
	_filled += filled;
	return fwrite((char *) data, 1, size, _file) == size;
}

bool gerbil::TempFile::read(SuperBundle *superBundle) {
	
	// //// ORIGINAL //
	// return fread(superBundle->data, 1, SUPER_BUNDLE_DATA_SIZE_B, _file) == SUPER_BUNDLE_DATA_SIZE_B;
	// //// END ORIGINAL //

	uint64_t result;
	// timer start
	// SimpleTimer timer;
	// timer.reset();
	// timer.start();

	result = fread(superBundle->data, 1, SUPER_BUNDLE_DATA_SIZE_B, _file); 

	// timer end
	// timer.stop();
	// printf("run2 fread, end_sec: %llu, end_usec: %llu\n", timer.tv2.tv_sec, timer.tv2.tv_usec);
	

	// ofstream bubble_fread_run2;
	// bubble_fread_run2.open("bubble_fread_run2.csv", ios::app);
	// bubble_fread_run2 << timer.tv1.tv_sec << "," << timer.tv1.tv_usec << endl; // start time
	// bubble_fread_run2 << timer.tv2.tv_sec << "," << timer.tv2.tv_usec << endl; // end time
	// bubble_fread_run2.close();

	if(result == SUPER_BUNDLE_DATA_SIZE_B){
		return true;
	} else {
		return false;
	} 

}

bool gerbil::TempFile::isEmpty() {
	return !_size;
}

void gerbil::TempFile::close() {
	if (_file)
		fclose(_file);
}

void gerbil::TempFile::fprintStat(FILE *file) const {
	// fprintf(file, "%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\n", _id, _smers, _kmers, _ukmers, _size, _filled, _numberOfRuns);
	fprintf(file, "%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\n", _id, _smers, _kmers, _ukmers, _btUkmers, _size, _filled, _numberOfRuns);
	// printf("%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\n", _id, _smers, _kmers, _ukmers, _size, _filled, _numberOfRuns);
}

void gerbil::TempFile::loadStats(string path, FILE *file) {
	if (!fscanf(file, "%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\n", &_id, &_smers, &_kmers, &_ukmers, &_size, &_filled, &_numberOfRuns)) {
		cerr << "binStatFile in " << path << " is corrupted" << endl;
		exit(1);
	}
	// _ukmers = 0;
	_filename = path + "temp" + to_string(_id) + ".bin";
}


