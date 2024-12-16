#include "Club.h"
#include <fstream>
#include <iostream>

int
main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input file>" << std::endl;
    return 1;
  }

  std::ifstream infile(argv[1]);
  if (!infile.is_open()) {
    std::cerr << "Error opening file: " << argv[1] << std::endl;
    return 1;
  }

  int numTables;
  std::string openTime, closeTime;
  int costPerHour;

  infile >> numTables >> openTime >> closeTime >> costPerHour;

  Club club(numTables, openTime, closeTime, costPerHour);
  club.readEvents(infile);
  infile.close();

  club.handleEvents();
  club.handleEndOfDay();
  club.printResults();

  return 0;
}
