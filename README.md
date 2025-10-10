# STA
sta is a minimal & simple status monitor app built in c.

## How it works
STA creates a UNIX socket on the server side and keeps running, waiting for updates from clients.

When you run it for the first time, it starts the server (daemon) and keeps running.  
After that, you can send status updates.

> **Note:** This project was created for learning purposes. 
### Example
```bash
# Run the server
sta
```

```
# Send a status update
sta -id 1 -name "$(date +%H:%M)"
```

This sends the current time to the server with a unique ID.
To update the status, rerun the command.


i just get slstatus functions and put them here
## Patches
> how to apply patches
just run this after clone the repo
```
cd sta
python patcher.py

```
### slstatus
this patch lets u use it like slstatus, but you can control it in 2 different ways
- you can send a signle update from client-side using `sta -id <int>` 
- every command has its own delay ; 0 means run it ones and do thing else 


> **Note:** i just took the slstatus functions and put them here. 
