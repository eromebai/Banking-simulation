make:
	gcc main.c account.c transaction.c -lpthread -o asn3.out
clean:
	-rm asn3.out assignment_3_output_file.txt