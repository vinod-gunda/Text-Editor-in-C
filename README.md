Text Editor in C
Overview
-
This project implements a simple terminal-based Text Editor written in C.
The editor allows users to create, edit, and manage text files using various editing operations.
The program works with a text file along with the compiled binary executable, enabling the user to perform editing operations directly on the file.


Features
-
The editor supports the following operations:
Insert Text – Insert text at the current cursor position
Delete Characters – Delete a specified number of characters
Delete Line – Remove the contents of the current line
Undo – Revert the last operation
Redo – Reapply the last undone operation
Display – Show the current contents of the file

Cursor Navigation
-
Move Up
Move Down
Move Left
Move Right
Go to Start of File
Go to End of File
Go to Start of Line
Go to End of Line


File Handling
-
When the program starts with a filename argument, the editor loads the contents of that file.
All editing operations are performed on the loaded file.
When exiting the editor:
The program prompts the user to save the changes.

Empty File Handling
-
If no filename is provided while running the program:
The editor creates an empty node internally.
The user can perform all editing operations on this empty buffer.
When exiting, the editor asks the user to provide a filename to save the content.


Technologies Used
-
C Programming
Doubly Linked List
Stack (Undo / Redo)
Dynamic Memory Allocation


File Handling
How to Run 
-
Compile the program:
gcc text_editor.c -o text_editor

Run with a file:
./text_editor filename.txt

Run without a file:
./text_editor


Future Improvements
-
-> Dynamic buffer resizing
-> Search and replace functionality
-> Syntax highlighting
-> GUI-based editor
