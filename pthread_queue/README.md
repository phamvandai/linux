# Implement simple message queue for pthreads 

This implementation uses pthread conditional variable.

Threads can block on waiting for item (a pointer to somewhere) available on message queue.

Other threads can send message to queue.

Currently implemented blocking read

