# HTTPhony
A simple port scanner that emulates HTTP traffic.

Usage:

```
gcc -o httphony httphony.c
./httphony <host> <ports> <optional_delay_in_seconds>
./httphony example.com 80,443,8080,8443 10

```
