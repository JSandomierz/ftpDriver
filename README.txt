1. Connections are detected with help of system calls, if there is incomming ftp connection the following procedure is applied.
2. PSU is enabled with Raspbery-Pi GPIO pin through NPN transistor key (it connects POWER_ON and GND wires).
3. After spinup time hdd is detected in system, then it is mounted as ftp share.
4. If there are no connections hdd is put into standby mode, and after spindown time PSU is shut down.