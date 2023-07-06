#!/bin/python
import telebot
import re
import datetime
from threading import Lock
import ctypes

# Load the c library, database, log file and api key file
lib = ctypes.CDLL('../src/lib/bin/searchEngineLib.so')
filename = b"../databases/database.csv"  
api_key_file = "api_key.txt" # <<<--- create this file and paste your api key from bot father in
logfile = "searchs.log"  

# Declare C Functions
lib.getLine.argtypes = [ctypes.c_char_p, ctypes.c_int]
lib.getLine.restype = ctypes.c_char_p

lib.searchID.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.searchID.restype = int

lib.searchHalfNameAll.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
lib.searchHalfNameAll.restype = int

lib.searchDateAll.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
lib.searchDateAll.restype = int

lib.searchHalfNameDateAll.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
lib.searchHalfNameDateAll.restype = int

# To read api key from api_key.txt
with open(api_key_file, "r") as file:
	api_key = file.readline().strip()

# To set api key in telebot and use as bot
bot = telebot.TeleBot(api_key)

# vars to set only one resquest at time
lock = Lock()
ongoing_request = False
ongoing_search = False
ongoing_string = ""

@bot.message_handler(func=lambda message: True)
def search_message(message):

	# geting global vars
	global ongoing_request
	global ongoing_search
	global ongoing_string

	# To send help for /start and /help
	if message.text == "/start" or message.text == "/help":
		send_instructions(message.chat.id)
		return

	if message.text == "/stop":
		if ongoing_search:
			ongoing_search = False
			bot.send_message(message.chat.id, "Current search are being stopped.")
		else:
			bot.send_message(message.chat.id, "There is no ongoing search to stop.")
		return

	
	# Check if already there is a requesting going on
	if ongoing_request:
		bot.send_message(message.chat.id, "Search for {} in progess, wait.".format(ongoing_string))
	else:
		ongoing_request = True  

		# To store the searched line at log file
		with open(logfile, "a") as file:
			file.write(message.text + "\n")

		# To parse input search menssage
		name_info = extract_info(message.text)
		current_time = datetime.datetime.now().strftime("%Hh %Mm %Ss")
		
		exceptions = ["of"]
		name_info[1] = capitalize_first_characters_except_specific_words(name_info[1], exceptions)

		# To set main string in search input
		if name_info[0]:
			ongoing_string = name_info[0]
		elif name_info[1]:
			ongoing_string = name_info[1]
		elif name_info[2]:
			ongoing_string = name_info[2]

		bot.send_message(message.chat.id, "Search for {} started at {}.".format(ongoing_string, current_time))

		with lock:
			ongoing_search = True
			search(name_info[0], name_info[1], name_info[2], message.chat.id)

			current_time = datetime.datetime.now().strftime("%Hh %Mm %Ss")
			bot.send_message(message.chat.id, "Search for {} finished at {}.".format(ongoing_string, current_time))

			ongoing_request = False  
			ongoing_search = False  
			ongoing_string = ""

	# Free the memory allocated in the C function
	#lib.free()

# To send help message
def send_instructions(chat_id):
	bot.send_message(chat_id, "*How to Use SearchR0Bot*  \U0001F50D \n\nEnter the name, date of birth, or year of the person you are looking for in the search field.\nIt doesn't need to be in order, our bot searches for people who have all the information you entered in the chat.\nIf you want to know who a certain ID belongs to, just enter it without dots or dashes.\n\n*Search Examples:*\n\n*ID:* 12345678901\n*Name:* John Doe Kayne\n*Name and Birthdate:* John Doe Kayne 12/12/1999\n*Name and Anniversary:* John Doe Kayne 12/12\n*Name and Year:* John Doe Kayne 1999\n\n/help to display these instructions again", parse_mode="Markdown")

# To capitalize input search and set exeptions if needed
def capitalize_first_characters_except_specific_words(string, exceptions):
    words = string.split()
    capitalized_words = []

    for word in words:
        if word.lower() in exceptions:
            capitalized_words.append(word.lower())
        else:
            capitalized_words.append(word.capitalize())

    return ' '.join(capitalized_words)

# Parse function
def extract_info(text):
	id_regex = r"\d{11,}"
	id_match = re.search(id_regex, text)

	if id_match:
		id_number = id_match.group(0)
		name_and_birth = re.sub(id_regex, "", text).strip()
		name, birth_info = separate_name_and_birth(name_and_birth)
		return [id_number, name, birth_info]

	name, birth_info = separate_name_and_birth(text)
	return [None, name, birth_info]

# Parse date function
def separate_name_and_birth(text):
	full_birthday_regex = r"(\d{2})/(\d{2})/(\d{4})"
	full_birthday_match = re.search(full_birthday_regex, text)

	if full_birthday_match:
		day, month, year = full_birthday_match.groups()
		name = re.sub(full_birthday_regex, "", text).strip()
		return name, "{}/{}/{}".format(day, month, year)

	partial_birthday_regex = r"(\d{2})/(\d{2})"
	partial_birthday_match = re.search(partial_birthday_regex, text)

	if partial_birthday_match:
		day, month = partial_birthday_match.groups()
		name = re.sub(partial_birthday_regex, "", text).strip()
		return name, "{}/{}".format(day, month)

	name = re.sub(r"\d{4}", "", text).strip()
	birth_year_match = re.search(r"\d{4}", text)
	birth_year = birth_year_match.group(0) if birth_year_match else None

	return name, birth_year

# Function to send results and store at log file
def handle_output_line(strLine, chat_id):
	# Split the line into individual components
	components = strLine.split(';')

	# Ensure the line has the required number of components
	if len(components) != 4:
		bot.send_message(chat_id, "Invalid input format.")
		return

	# Extract the components
	ID = components[0].strip()
	name = components[1].strip()
	sex = components[2].strip()
	birth = components[3].strip()

	# Format the output message using .format()
	output = "ID: {}\nName: {}\nSex: {}\nBirth: {}\n".format(ID, name, sex, birth)

	# Send the formatted message
	bot.send_message(chat_id, output)
	
	# To store the found line at log file
	with open(logfile, "a") as file:
		file.write(strLine + "\n")

# Search Functions
def idSearch(ID, chat_id):
	global ongoing_search
	if ongoing_search:
		line = lib.searchID(filename, ID.encode('utf-8'))
		result = lib.getLine(filename, line)
		if result is not None:
			string = ctypes.string_at(result).decode('utf-8')
			handle_output_line(string, chat_id)

def nameSearch(name, chat_id):
	line = 0
	while line != -1:
		global ongoing_search
		if ongoing_search:
			line = lib.searchHalfNameAll(filename, name.encode('utf-8'), line)
			result = lib.getLine(filename, line)
			if result is not None:
				string = ctypes.string_at(result).decode('utf-8')
				handle_output_line(string, chat_id)
		else:
			line = -1

def dateSearch(date, chat_id):
	line = 0
	while line != -1:
		global ongoing_search
		if ongoing_search:
			line = lib.searchDateAll(filename, date.encode('utf-8'), line)
			result = lib.getLine(filename, line)
			if result is not None:
				string = ctypes.string_at(result).decode('utf-8')
				handle_output_line(string, chat_id)
		else:
			line = -1

def nameDateSearch(name, date, chat_id):
	line = 0
	while line != -1:
		global ongoing_search
		if ongoing_search:
			line = lib.searchHalfNameDateAll(filename, name.encode('utf-8'), date.encode('utf-8'), line)
			result = lib.getLine(filename, line)
			if result is not None:
				string = ctypes.string_at(result).decode('utf-8')
				handle_output_line(string, chat_id)
		else:
			line = -1

# Function to chose the function that should be used
def search(ID, name, date, chat_id):
	if ID:
		idSearch(ID, chat_id)
	elif date and name:
		nameDateSearch(name, date, chat_id)
	elif date:
		dateSearch(date, chat_id)
	elif name:
		nameSearch(name, chat_id)

bot.polling()
