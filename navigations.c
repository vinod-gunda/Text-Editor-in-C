#include "text_editor.h"

void navigate_operations(TextEditor *editor)
{
    // Display navigation menu
    printf(BLUE "\n========== NAVIGATION MENU ==========\n" RESET);

    printf(GREEN "1. moveup" RESET " -> moveCursorUp\n");
    printf(GREEN "2. movedown" RESET " -> moveCursorDown\n");
    printf(GREEN "3. moveleft" RESET " -> moveCursorLeft\n");
    printf(GREEN "4. moveright" RESET " -> moveCursorRight\n");
    printf(GREEN "5. gostartoffile" RESET " -> jumpToStartOfFile\n");
    printf(GREEN "6. goendoffile" RESET " -> jumpToEndOfFile\n");
    printf(GREEN "7. gostartofline" RESET " -> jumpToStartOfLine\n");
    printf(GREEN "8. goendofline" RESET " -> jumpToEndOfLine\n");

    printf(CYAN "Enter the Command: " RESET);

    // Buffer to store navigation command
    char navigate[20] = {0};

    __fpurge(stdin);                   // Clear input buffer
    scanf("%[^\n]",navigate);          // Read navigation command

    // Move cursor up
    if(strcasecmp(navigate,"moveup") == 0)
    {
        moveCursorUp(editor);
    }
    // Move cursor down
    else if(strcasecmp(navigate,"movedown") == 0)
    {
        moveCursorDown(editor);
    }
    // Move cursor left
    else if(strcasecmp(navigate,"moveleft") == 0)
    {
        moveCursorLeft(editor);
    }
    // Move cursor right
    else if(strcasecmp(navigate,"moveright") == 0)
    {
        moveCursorRight(editor);
    }
    // Jump to start of file
    else if(strcasecmp(navigate,"gostartoffile") == 0)
    {
        jumpToStartOfFile(editor);
    }
    // Jump to end of file
    else if(strcasecmp(navigate,"goendoffile") == 0)
    {
        jumpToEndOfFile(editor);
    }
    // Jump to start of line
    else if(strcasecmp(navigate,"gostartofline") == 0)
    {
        jumpToStartOfLine(editor);
    }
    // Jump to end of line
    else if(strcasecmp(navigate,"goendofline") == 0)
    {
        jumpToEndOfLine(editor);
    }
    // Invalid command handling
    else
    {
        printf(RED "Invalid Command\n" RESET);
    }

}


// Move cursor one line up
void moveCursorUp(TextEditor *editor)
{
    // Check if previous line exists
    if((editor->cursor) -> prev != NULL)
    {
        editor->cursorLine--;                         // Decrement line number
        editor -> cursor = (editor->cursor) -> prev;  // Move cursor to previous node

        int len;
        len = strlen((editor->cursor)->line);         // Get length of new line

        // Adjust cursor position if it exceeds line length
        if(editor -> cursorPos > len)
        {
            editor -> cursorPos = len-1;
        }
    }
}


// Move cursor one line down
void moveCursorDown(TextEditor *editor)
{
    // Check if next line exists
    if((editor->cursor) -> next != NULL)
    {
        editor->cursorLine++;                         // Increment line number
        editor -> cursor = (editor->cursor) -> next;  // Move cursor to next node

        int len;
        len = strlen((editor->cursor)->line);         // Get length of new line

        // Adjust cursor position if it exceeds line length
        if(editor -> cursorPos > len)
        {
            editor -> cursorPos = len-1;
        }
    }
}


// Move cursor one character left
void moveCursorLeft(TextEditor *editor)
{
    // Ensure cursor is not at beginning
    if(editor -> cursorPos > 0)
    {
        editor -> cursorPos--;                        // Move cursor left
    }  
}


// Move cursor one character right
void moveCursorRight(TextEditor *editor)
{
    int len;
    len = strlen((editor->cursor)->line);             // Get current line length

    // Ensure cursor does not exceed line length
    if(editor -> cursorPos < len)
    {
        editor -> cursorPos++;                        // Move cursor right
    }
}


// Jump cursor to start of file
void jumpToStartOfFile(TextEditor *editor)
{
    editor -> cursor = editor -> head;                // Move cursor to first node
    editor -> cursorLine = 1;                         // Set line number to 1
    editor -> cursorPos = 0;                          // Set position to start
}


// Jump cursor to end of file
void jumpToEndOfFile(TextEditor *editor)
{
    editor -> cursorLine = 0;
    editor -> cursorPos = 0;

    Node *temp = editor->head;

    // Traverse to count total number of lines
    while(temp!=NULL)
    {
        editor->cursorLine++;
        temp = temp -> next;
    }

    editor -> cursor = editor -> tail;                // Move cursor to last node

    int len;
    len = strlen((editor->cursor)->line);             // Get last line length

    editor -> cursorPos = len-1;                        // Place cursor at end of line
}


// Jump cursor to start of current line
void jumpToStartOfLine(TextEditor *editor)
{
    editor -> cursorPos = 0;                          // Move cursor to beginning of line
}


// Jump cursor to end of current line
void jumpToEndOfLine(TextEditor *editor)
{
    int len;
    len = strlen((editor->cursor)->line);             // Get line length

    editor -> cursorPos = len-1;                        // Move cursor to end of line
}