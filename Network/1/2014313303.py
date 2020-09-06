file_name = input("Input the file name: ")
new_name = input("Input the new name: ")

f = open(file_name, 'rb')
f1 = open(new_name, 'wb')
f2 = open("log.txt", 'a')

while True:
	line = f.readline()
	if not line: break
	f1.write(line)

f2.write("file copy is done\n")

f.close()
f1.close()
f2.close()
