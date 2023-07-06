import re
import sys

def extract_info(text):
    # Check if the string contains a ID number (11 or more digits)
    id_regex = r"\d{11,}"
    id_match = re.search(id_regex, text)

    if id_match:
        id_number = id_match.group(0)
        name_and_birth = re.sub(id_regex, "", text).strip()
        name, birth_info = separate_name_and_birth(name_and_birth)
        return [id_number, name, birth_info]

    name, birth_info = separate_name_and_birth(text)
    return [None, name, birth_info]

def separate_name_and_birth(text):
    # Check if the string contains a full birthday (dd/mm/yyyy)
    full_birthday_regex = r"(\d{2})/(\d{2})/(\d{4})"
    full_birthday_match = re.search(full_birthday_regex, text)

    if full_birthday_match:
        day, month, year = full_birthday_match.groups()
        name = re.sub(full_birthday_regex, "", text).strip()
        return name, "{}/{}/{}".format(day, month, year)

    # Check if the string contains a partial birthday (dd/mm)
    partial_birthday_regex = r"(\d{2})/(\d{2})"
    partial_birthday_match = re.search(partial_birthday_regex, text)

    if partial_birthday_match:
        day, month = partial_birthday_match.groups()
        name = re.sub(partial_birthday_regex, "", text).strip()
        return name, "{}/{}".format(day, month)

    # Extract the name and birth year if no birthday is found
    name = re.sub(r"\d{4}", "", text).strip()  # Updated regex to match 4-digit birth year
    birth_year_match = re.search(r"\d{4}", text)
    birth_year = birth_year_match.group(0) if birth_year_match else None

    return name, birth_year

input_string = " ".join(sys.argv[1:])
name_info = extract_info(input_string)

if not re.search(r"^\D+$", name_info[1]):
    name_info[1] = None
"""
print("ID:", name_info[0])
print("Name:", name_info[1])
print("Birth Info:", name_info[2])
"""
print(name_info)

