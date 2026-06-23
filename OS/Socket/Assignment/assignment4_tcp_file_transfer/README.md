# Assignment 4: File Transfer Using TCP

## Build

```sh
make
```

## Run

Terminal 1:

```sh
./server
```

Terminal 2:

```sh
./client
```

The client sends `input.txt`. The server saves it as `received.txt`.

## Sample Output

Server:

```text
File server listening on port 5004...
Client Connected: 127.0.0.1:40334
File Transfer Successful
```

Client:

```text
File Transfer Successful
```
