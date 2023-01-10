Name: Phat Tran
Date: 11/30/22
Class: Operating Systems

RESOURCES:
	-https://www.geeksforgeeks.org/linear-search-using-multi-threading/
	-https://linuxhint.com/using_mmap_function_linux/
	-https://pubs.opengroup.org/onlinepubs/007908799/xsh/sysstat.h.html
	-https://www.tutorialspoint.com/linear-search-using-multi-threading-in-c
	
SETUP:
	On terminal, to create binary file:
	- rm -rf fec
	- gcc -g fec.c -o fec -lpthread -O

HOW TO USE:
	- There are test files filled with numbers separated by commas
	- To search for a number in the text files go into fec.c
		- char* key = ""
		- insert a number that you want to search for in the quotes
	- Save fec.c and recreate the binary file above
	- On terminal, type:
		-./fec test_1.txt test_2.txt test3.txt
		- it will return "found" in the terminal if the number exists
		- if it's not found then it will return a "Segmentation fault (core dumped) error"

PROBLEMS:
	- So basically, my code works fine if the key exist, but if it doesn't, it returns the segmentation error
	- Also my text files will only work if the beginning of every file other than the first starts with a comma
