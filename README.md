# HTTPhony
A simple port scanner that emulates HTTP traffic, with an optional delay (in seconds) between each port scan.

Usage:

```
gcc -o httphony httphony.c
./httphony <host> <ports> <delay>
./httphony example.com 80,443,8080,8443 10

```
