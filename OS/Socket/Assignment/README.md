# Socket Programming Assignments

This folder contains one separate directory for each assignment.

## Folder List

| Assignment | Folder | Port |
| --- | --- | --- |
| 1. Simple TCP Hello Communication | `assignment1_tcp_hello` | 5001 |
| 2. TCP Chat Application | `assignment2_tcp_chat` | 5002 |
| 3. UDP Message Transfer | `assignment3_udp_message` | 5003 |
| 4. File Transfer Using TCP | `assignment4_tcp_file_transfer` | 5004 |
| 5. Multi-Client TCP Server | `assignment5_multi_client_tcp` | 5005 |

## Build Any Assignment

```sh
cd assignment1_tcp_hello
make
```

Use the same `make` command inside each assignment folder.

## Clean Any Assignment

```sh
make clean
```

## Notes

- Each assignment contains `server.c`, `client.c`, `Makefile`, and `README.md`.
- Assignment 4 also includes `input.txt`.
- Run the server first, then run the client from another terminal.
- All examples use `127.0.0.1` for local testing.




 Created separate folders for all 5 socket assignments under:

  /home/silicondoc/Documents/Class file/LDD/Socket/Assignment

  Each assignment folder includes:

  - server.c
  - client.c
  - Makefile
  - README.md with build/run commands and sample output

  Also added a top-level README.md index.

  Verified successfully:

  - Assignment 1 TCP hello communication
  - Assignment 2 TCP chat with exit
  - Assignment 3 UDP uppercase reply
  - Assignment 4 TCP file transfer, including cmp input.txt received.txt
  - Assignment 5 multi-client TCP server with two clients

  Compiled binaries are already present in each folder. You can rebuild any assignment with:

  make

  and clean with:

  make clean


