filename = "database.csv"  # Replace with the actual filename

unique_ids = set()
unique_lines = []

with open(filename, "r") as file:
    for line in file:
        line = line.strip()
        linesplit = line.split(";")
        if linesplit[0] not in unique_ids:
            unique_ids.add(linesplit[0])
            unique_lines.append(line)

output = "\n".join(unique_lines)
print(output)
