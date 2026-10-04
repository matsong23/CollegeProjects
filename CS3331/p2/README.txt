using 1 late day 
remaining 3

Q1:
I have one sgement and each time I fork I have  a new range assigned to the fork so it doesn't go out of its range and 
nothing else goes into that range.

Q2:
Likely due to the program not being completed in the merge step

Q3:
This can cause race conditions leading to a failed sort. My program doesn't avoid this because it is not done\

Q4:
yes by doing

element process[]
for all elements in array {
	element process[i] = busywait fork
}

then based on the index of the process it coresponds to a element in the array of the samne index


NOTE:
My program works up until the merge step due to me not being able to figure out how to implement it in the way that was necessary for the 
assignment. I know everything else works because I spent 2 minutes making a different merge method that was at best case just as fast by 
forking for each element and going over the other side array to find the index it should be in and then testing it. I spent an hour and a
half on the everything but the merge section to get it working and 5 hours on the merge section before I ran out of time and it is just 
too confusing for no reason and I would most likely never have to do it this way in the career field. Could I get partial points for 
attempting it instead of 0 for not having it working? Thank you.
