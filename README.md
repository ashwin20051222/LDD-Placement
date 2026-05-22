# LDD Placement

Common README for the complete LDD placement workspace.

This repository contains C programming practice, Makefile assignments, socket programming assignments, firmware HCL topic notes, and GDB/objdump learning documents. Folder-specific README files are kept inside their own assignment folders; this README describes the complete repository structure.

## Complete Folder Structure

```text
LDD-Placement/
|-- Firmware_HCL_topic/
|   `-- Firmware_HCL_REVIEW_TOPICS.xlsx
|-- HCL_week_1/
|   `-- C program/
|       |-- program_01.c
|       |-- program_02.c
|       |-- program_03.c
|       |-- program_04.c
|       |-- program_05.c
|       |-- program_06.c
|       |-- program_07.c
|       |-- program_08.c
|       |-- program_09.c
|       |-- program_10.c
|       |-- program_11.c
|       |-- program_12.c
|       |-- program_13.c
|       |-- program_14.c
|       |-- program_15.c
|       |-- program_16.c
|       |-- program_17.c
|       |-- program_18.c
|       |-- program_19.c
|       |-- program_20.c
|       |-- program_21.c
|       |-- program_22.c
|       |-- program_23.c
|       |-- program_24.c
|       |-- program_25.c
|       |-- program_26.c
|       |-- program_27.c
|       |-- program_28.c
|       |-- program_29.c
|       |-- program_30.c
|       |-- program_31.c
|       |-- program_32.c
|       |-- program_33.c
|       |-- program_34.c
|       |-- program_35.c
|       `-- program_36.c
|-- Make_File/
|   |-- Assignment/
|   |   `-- Make_FIle_Ass/
|   |       |-- linux_tools/
|   |       |   |-- Makefile
|   |       |   |-- bin/
|   |       |   |   |-- .mode
|   |       |   |   |-- installed/
|   |       |   |   |   |-- mycat
|   |       |   |   |   |-- mycp
|   |       |   |   |   `-- myls
|   |       |   |   |-- mycat
|   |       |   |   |-- mycp
|   |       |   |   `-- myls
|   |       |   |-- build/
|   |       |   |   |-- debug/
|   |       |   |   |   |-- mycat.d
|   |       |   |   |   |-- mycat.o
|   |       |   |   |   |-- mycp.d
|   |       |   |   |   |-- mycp.o
|   |       |   |   |   |-- myls.d
|   |       |   |   |   |-- myls.o
|   |       |   |   |   |-- utils.d
|   |       |   |   |   `-- utils.o
|   |       |   |   `-- release/
|   |       |   |       |-- mycat.d
|   |       |   |       |-- mycat.o
|   |       |   |       |-- mycp.d
|   |       |   |       |-- mycp.o
|   |       |   |       |-- myls.d
|   |       |   |       |-- myls.o
|   |       |   |       |-- utils.d
|   |       |   |       `-- utils.o
|   |       |   |-- implement_command.txt
|   |       |   |-- include/
|   |       |   |   `-- utils.h
|   |       |   |-- linux_tools/
|   |       |   |   |-- Makefile
|   |       |   |   |-- bin/
|   |       |   |   |   |-- .mode
|   |       |   |   |   |-- installed/
|   |       |   |   |   |   |-- mycat
|   |       |   |   |   |   |-- mycp
|   |       |   |   |   |   `-- myls
|   |       |   |   |   |-- mycat
|   |       |   |   |   |-- mycp
|   |       |   |   |   `-- myls
|   |       |   |   |-- include/
|   |       |   |   |   `-- utils.h
|   |       |   |   `-- src/
|   |       |   |       |-- mycat.c
|   |       |   |       |-- mycp.c
|   |       |   |       |-- myls.c
|   |       |   |       `-- utils.c
|   |       |   |-- linux_tools.tar.gz
|   |       |   `-- src/
|   |       |       |-- mycat.c
|   |       |       |-- mycp.c
|   |       |       |-- myls.c
|   |       |       `-- utils.c
|   |       `-- student_db/
|   |           |-- Makefile
|   |           |-- apps/
|   |           |   |-- add_main.c
|   |           |   |-- display_main.c
|   |           |   `-- search_main.c
|   |           |-- bin/
|   |           |   |-- .mode
|   |           |   |-- add_student
|   |           |   |-- display_student
|   |           |   `-- search_student
|   |           |-- build/
|   |           |   |-- debug/
|   |           |   |   |-- add_main.d
|   |           |   |   |-- add_main.o
|   |           |   |   |-- add_student.d
|   |           |   |   |-- add_student.o
|   |           |   |   |-- common.d
|   |           |   |   |-- common.o
|   |           |   |   |-- display_main.d
|   |           |   |   |-- display_main.o
|   |           |   |   |-- display_student.d
|   |           |   |   |-- display_student.o
|   |           |   |   |-- file_ops.d
|   |           |   |   |-- file_ops.o
|   |           |   |   |-- search_main.d
|   |           |   |   |-- search_main.o
|   |           |   |   |-- search_student.d
|   |           |   |   `-- search_student.o
|   |           |   `-- release/
|   |           |       |-- add_main.d
|   |           |       |-- add_main.o
|   |           |       |-- add_student.d
|   |           |       |-- add_student.o
|   |           |       |-- common.d
|   |           |       |-- common.o
|   |           |       |-- display_main.d
|   |           |       |-- display_main.o
|   |           |       |-- display_student.d
|   |           |       |-- display_student.o
|   |           |       |-- file_ops.d
|   |           |       |-- file_ops.o
|   |           |       |-- search_main.d
|   |           |       |-- search_main.o
|   |           |       |-- search_student.d
|   |           |       `-- search_student.o
|   |           |-- implement_comand.txt
|   |           |-- implement_command.txt
|   |           |-- include/
|   |           |   `-- student.h
|   |           |-- src/
|   |           |   |-- add_student.c
|   |           |   |-- common.c
|   |           |   |-- display_student.c
|   |           |   |-- file_ops.c
|   |           |   `-- search_student.c
|   |           `-- students.dat
|   |-- Class Documents/
|   |   |-- makefiletutorial/
|   |   |   |-- Makefile_Tutorial_01.docx
|   |   |   |-- Makefile_Tutorial_02.docx
|   |   |   |-- Makefile_Tutorial_03.docx
|   |   |   |-- makefile.1
|   |   |   |-- makefile.2
|   |   |   |-- makefile.3
|   |   |   |-- makefile.4
|   |   |   `-- makefile.5
|   |   `-- makefileupdatedversionwithmoreexplanation/
|   |       |-- Makefile Assignment.docx
|   |       `-- Makefile_Tutorial_03.docx
|   |-- calculator_project/
|   |   |-- Makefile
|   |   |-- build/
|   |   |   |-- add.d
|   |   |   |-- add.o
|   |   |   |-- div.d
|   |   |   |-- div.o
|   |   |   |-- main.d
|   |   |   |-- main.o
|   |   |   |-- mul.d
|   |   |   |-- mul.o
|   |   |   |-- sub.d
|   |   |   `-- sub.o
|   |   |-- calculator
|   |   |-- include/
|   |   |   `-- calc.h
|   |   |-- lib/
|   |   |   `-- libcalc.a
|   |   |-- src/
|   |   |   |-- add.c
|   |   |   |-- div.c
|   |   |   |-- main.c
|   |   |   |-- mul.c
|   |   |   `-- sub.c
|   |   `-- terminal.txt
|   `-- first_make/
|       `-- Makefile1
|-- Socket/
|   |-- Assignment/
|   |   |-- README.md
|   |   |-- assignment1_tcp_hello/
|   |   |   |-- Makefile
|   |   |   |-- README.md
|   |   |   |-- client
|   |   |   |-- client.c
|   |   |   |-- server
|   |   |   `-- server.c
|   |   |-- assignment2_tcp_chat/
|   |   |   |-- Makefile
|   |   |   |-- README.md
|   |   |   |-- client
|   |   |   |-- client.c
|   |   |   |-- server
|   |   |   `-- server.c
|   |   |-- assignment3_udp_message/
|   |   |   |-- Makefile
|   |   |   |-- README.md
|   |   |   |-- client
|   |   |   |-- client.c
|   |   |   |-- server
|   |   |   `-- server.c
|   |   |-- assignment4_tcp_file_transfer/
|   |   |   |-- Makefile
|   |   |   |-- README.md
|   |   |   |-- client
|   |   |   |-- client.c
|   |   |   |-- input.txt
|   |   |   |-- received.txt
|   |   |   |-- server
|   |   |   `-- server.c
|   |   `-- assignment5_multi_client_tcp/
|   |       |-- Makefile
|   |       |-- README.md
|   |       |-- client
|   |       |-- client.c
|   |       |-- server
|   |       `-- server.c
|   |-- Socket_prog/
|   |   |-- Makefile
|   |   `-- src/
|   |       |-- client.c
|   |       `-- server.c
|   `-- reg_socketprogramming/
|       |-- Socket_programming in C_BinaryTides.docx
|       `-- TCP_SCOKET_PROGRAMMING.docx
|-- objdumpgdb/
|   |-- GDB Basics Tutorial.docx
|   |-- GDB Tutorial 02.docx
|   |-- OBJDUMP Basics.docx
|   `-- mail_link_content.txt
`-- README.md
```
