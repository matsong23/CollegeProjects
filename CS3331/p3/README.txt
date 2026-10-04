Question 1:
No race conditions. Every thread is given two indexes and no other
threads in the pass(odd and even) are given access to those indexes

Question 2:
have an array of variables to dictate for each thread if they should be
waiting and pass the k value when created. Set busy wait after even pass
in the thread and unset in the main thread when the odd pass startes. 
This way one thread has access to the varibles at a time.

Because only one thread has access to the variables at a time no race
conditions therefore the solution works.

Question 3:
Have an array of varibles, one fore each thread to dictate when they
should busy wait and set the value in the main thread set them to busy
wait by defualt until the pass is started and chnage them back to busy 
wait in the thread. This way only one thread will even have access to 
the value at one time. The threads will be sent k when they are created
and will have a counter to dictate when it is an odd or even pass. All 
other logic is the same.

This solution prevents any race conditions by setting busy wait for 
threads when they are done comparing and swapping and unsetting it once
the pass is started. 

