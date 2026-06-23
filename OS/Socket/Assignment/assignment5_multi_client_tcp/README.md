# Assignment 5: Multi-Client TCP Server

## Build

```sh
make
```

## Run

Terminal 1:

```sh
./server
```

Open multiple terminals and run:

```sh
./client
```

Enter values such as:

```text
Arun - 23CS101
Priya - 23CS102
```

## Sample Output

Server:

```text
Multi-client TCP server listening on port 5005...
Client 1 Connected: 127.0.0.1:40340
Server Timestamp: 2026-05-21 10:30:12
Client 1: Arun - 23CS101
Client 2 Connected: 127.0.0.1:40342
Server Timestamp: 2026-05-21 10:30:18
Client 2: Priya - 23CS102
```

Client:

```text
Enter Student Name and Roll Number: Arun - 23CS101
Message sent to server.
```
