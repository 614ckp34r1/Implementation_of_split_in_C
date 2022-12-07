# Split Implementation in C

This C program splits the input file into multiple files of specified sizes or lines.

Input is the filename and spliting option (either size or line) with corresponding values

To run the program:

        gcc main.c -o split
        
        ./split -s 1KB <input_file_name>
        
Here the input file is splitted into sub files each of 1KB in size.

        ./split -n 100 <input_file_name>
        
Here the input file is splitted into sub files each consisting of 100 lines.

Output:

        Details of Splitted files with corresponding memory size or lines.
