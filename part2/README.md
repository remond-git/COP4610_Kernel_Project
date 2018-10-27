### Part 2 - `xtime` Module

On the first `proc` read, only the value of `xtime` is stored. On all subsequent `proc` reads, the difference of the current and last `xtime` values is also stored.