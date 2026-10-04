1.
           main
       /     |     \
catalan    buffon   pinball
2.
My parent process will start the processes for catalan, buffon, and pinball
and then wait for each process to exit before the poarent process exits

3.
a.
               main
               / 
           catalan
           /
		buffon
       /
  pinball
b.
No because all the processes will be started but they will then be waiting for
 their child process to finish so they won't be concurrent
