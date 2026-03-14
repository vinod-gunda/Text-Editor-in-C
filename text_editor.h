#ifndef TEXT_EDITOR
#define TEXT_EDITOR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>

#define INITIAL_CAPACITY 10
#define STACK_EMPTY -1
#define STACK_FULL 1
#define STACK_NOT_FULL 0

// Define the structure for a node in the linked list (each line in the editor)
typedef struct Node {
    char *line;
    struct Node *prev;
    struct Node *next;
} Node;

// Define the structure for an action (used for undo/redo operations)
typedef struct {
    char *operation;  // Operation type ("insert" or "delete")
    char *text;       // Text involved in the operation
    int cursorLine;   // Line number where the operation occurred
    int cursorPos;    // Column position where the operation occurred
} Action;

// Define the structure for a dynamic array stack (used for undo/redo stacks)
typedef struct {
    Action *actions;  // Dynamic array of actions
    int size;         // Current size of the stack
    int capacity;     // Current capacity of the stack
} DynamicArrayStack;

// Define the structure for the text editor
typedef struct {
    Node *head;       // Pointer to the first line
    Node *tail;       // Pointer to the last line
    Node *cursor;     // Pointer to the current line where the cursor is
    int cursorLine;   // Line number of the cursor
    int cursorPos;    // Column position of the cursor
    DynamicArrayStack undoStack;
    DynamicArrayStack redoStack;
} TextEditor;

#define RED         "\033[1;3;31m"
#define GREEN       "\033[1;32m"
#define YELLOW      "\033[0;3;33m"
#define BLUE        "\033[1;3;34m"
#define MAGENTA     "\033[1;3;35m"
#define CYAN        "\033[1;3;36m"
#define RESET       "\033[0m"


// ------------- MAIN FUNCTION HEADERS ---------- //
void handleCommand(char *cmd,TextEditor *editor);
void printMenu();
void displayText(TextEditor *editor);
void init(TextEditor *); 


//------------------ READING  AND SAVING USE ---------------//
void read_file_to_list(TextEditor *editor,char *filename);
void saveFile(TextEditor *editor, const char *filename);


// -----------NAVIGATION PURPOSE -----------------//
void navigate_operations(TextEditor *editor);
void moveCursorUp(TextEditor *editor);
void moveCursorDown(TextEditor *editor);
void moveCursorLeft(TextEditor *editor);
void moveCursorRight(TextEditor *editor);
void jumpToStartOfFile(TextEditor *editor);
void jumpToEndOfFile(TextEditor *editor);
void jumpToStartOfLine(TextEditor *editor);
void jumpToEndOfLine(TextEditor *editor);


// ------------- TEXT EDIT FUNCTIONS -----------------//
void insertText(TextEditor *editor, const char *text);
void deleteCharacters(TextEditor *editor, int length);
void deleteLine(TextEditor *editor);
void restore_line(TextEditor *editor,Action action);


//  ------------------STACK UNDO / REDO OPREATIONS ------------//
void pushStack(DynamicArrayStack *stack,Action action);
Action popStack(DynamicArrayStack *stack);
int isStackEmpty(DynamicArrayStack *stack);
void undo(TextEditor *editor);
void redo(TextEditor *editor);
void init(TextEditor *); 


#endif
