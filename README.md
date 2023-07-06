# search_engine
A Search Engine made with Python and C to work as at CLI and as a telegram bot.
# Project Name

A brief description of your project.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)

## Installation

1. Clone the repository:

   ```bash
   git clone git@github.com:PauloLopez92/search_engine.git

## Usage

  ```bash
  ./searchEngine 
Usage: program [-f filename] [ -c <ID number>]
		or
Usage: program [-f filename] [ -n '<name>', -d DD/MM/YYYY ]

Options:
  -f		 Specify a filename
  -i             Specify a search for ID	searchEngine -f <filename> -i <ID Number>
  -n             Specify a search for Name	searchEngine -f <filename> -n '<name>'
  -d             Specify a search for Birthday	searchEngine -f <filename> -d DD/MM/YYYY
  -d             Specify a search for Day/Month	searchEngine -f <filename> -d DD/MM
  -d             Specify a search for Year	searchEngine -f <filename> -d YYYY

./searchEngine -f ../databases/database.csv  -n John 
82957570305;John Doe;M;12/12/1999
76414639621;Michael Johnson;M;06/15/1992


./searchEngine -f ../databases/database.csv  -n John -d 1999
82957570305;John Doe;M;12/12/1999

./searchEngine -f ../databases/database.csv  -i 69485811004
69485811004;Ava Allen;F;12/22/1992
