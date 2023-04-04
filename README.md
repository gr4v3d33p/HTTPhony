# HTTPhony
A simple port scanner that emulates HTTP traffic.

Usage:
./httphony <host> <ports> <optional_delay>

```
gcc -o httphony httphony.c
./httphony example.com 80,443,8080,8443 10
```
