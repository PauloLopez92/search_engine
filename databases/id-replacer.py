import random

filename = "database_repeat.csv"  # Replace with the actual filename

def generate_random_id():
    return str(random.randint(10000000000, 99999999999))

lines = []

with open(filename, "r") as file:
    for line in file:
        line = line.strip()
        linesplit = line.split(";")
        linesplit[0] = generate_random_id()
        updated_line = ";".join(linesplit)
        lines.append(updated_line)

output = "\n".join(lines)

with open(filename, "w") as file:
    file.write(output)

print("IDs replaced successfully!")
