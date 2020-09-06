import time
from threading import Thread

start = time.time()
chunk_size = 1024 * 4096

def copy_file():
	file_name = input("Input the file name: ")
	new_name = input("Input the new name: ")

	print()

	f2 = open("log.txt", 'a')
	tm = int(time.time() - start)
	f2.write('{:02d}:{:02d}'.format((tm%3600 // 60), tm % 60) 
			+ "\tStart copying " + file_name + " to " + new_name + "\n")
	f2.close()

	t = Thread(target = copy_file)
	t.start()

	with open(file_name, 'rb') as f:
		with open(new_name, 'wb') as f1:
			with open("log.txt", 'a') as f2:
				while True:
					buffer = f.read(chunk_size)
					if not buffer: break
					f1.write(buffer)
					
				tm = int(time.time() - start)
				f2.write('{:02d}:{:02d}'.format((tm%3600 // 60), tm % 60) 
						+ "\t" + new_name + " is copied completely\n")

				f.close()
				f1.close()
				f2.close()

if __name__ == "__main__":
	copy_file()
