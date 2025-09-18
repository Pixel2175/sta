# STA
sta is a minimal & simple status monitor app built in c.

## How it works
STA creates a UNIX socket on the server side and keeps running, waiting for updates from clients.

When you run it for the first time, it starts the server (daemon) and keeps running.  
After that, you can send status updates.

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

> **Note:** This project was created for learning purposes. 

