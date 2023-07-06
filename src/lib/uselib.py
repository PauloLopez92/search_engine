import ctypes

# Load the library and file name
lib = ctypes.CDLL('bin/searchEngineLib.so')
filename = b"../../database2.csv"  

# Declare C Functions
lib.getLine.argtypes = [ctypes.c_char_p, ctypes.c_int]
lib.getLine.restype = ctypes.c_char_p

lib.searchID.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.searchID.restype = int

lib.searchHalfNameAll.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
lib.searchHalfNameAll.restype = int

lib.searchHalfNameDateAll.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
lib.searchHalfNameDateAll.restype = int

def idSearch(ID):
	line = lib.searchID(filename, ID)
	result = lib.getLine(filename, line)
	if result is not None:
		string = ctypes.string_at(result).decode('utf-8')
		print(string)

def nameSearch(name):
	line = 0
	while line != -1:
		line = lib.searchHalfNameAll(filename, name, line)
		result = lib.getLine(filename, line)
		if result is not None:
			string = ctypes.string_at(result).decode('utf-8')
			print(string)


def nameDateSearch(name, date):
	line = 0
	while line != -1:
		line = lib.searchHalfNameDateAll(filename, name, date, line)
		result = lib.getLine(filename, line)
		if result is not None:
			string = ctypes.string_at(result).decode('utf-8')
			print(string)


def main():

	# VARS
	ID = b"90123456781"
	name = b"Benjamin"
	#date = b"24/12"
	date = b"1988"

	idSearch(ID)

	nameSearch(name)

	nameDateSearch(name, date)

	# Free the memory allocated in the C function
	lib.free()
if __name__ == '__main__':
	main()
