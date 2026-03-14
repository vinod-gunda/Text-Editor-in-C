#include "text_editor.h"

int delete_flag = 0;   // Flag used to avoid duplicate undo push during delete operations

void read_file_to_list(TextEditor *editor, char *filename)
{
    char line[100];   // Buffer to read each line from file

    FILE *fptr = fopen(filename,"r");   // Open file in read mode
    if(fptr == NULL)
    {
        printf(RED "File not present\n" RESET);
        exit(0);
    }

    memset(line,0,sizeof(line));   // Clear line buffer

    // Reading file line by line
    while(fscanf(fptr," %[^\n]",line) != EOF)
    {
        char *str = malloc(strlen(line)+1);   // Allocate memory for line
        strcpy(str,line);                     // Copy line content

        Node *new = malloc(sizeof(Node));     // Create new node
        if(new == NULL)
        {
            printf(RED "Memory not allocated\n" RESET);
            return;
        }

        new -> line = str;   // Store line in node
        new -> prev = NULL;
        new -> next = NULL;

        // Insert node into linked list
        if(editor->head == NULL)
        {
            editor -> head = new;
            editor -> tail = new;
        }
        else 
        {
            new -> prev = editor -> tail;
            (editor ->tail)-> next = new;
            editor -> tail = new;
        }

        memset(line,0,sizeof(line));   // Clear buffer for next read
    }

    editor -> cursor = editor -> head;   // Set cursor to first line
    fclose(fptr);                        // Close file
}


void saveFile(TextEditor *editor, const char *filename)
{
    FILE *fptr = fopen(filename,"w+");   // Open file in write mode

    Node *temp = editor->head;           // Start from first node
    unsigned char i = 0;

    while(temp != NULL)
    {
        i=0;

        // Write each character of the line
        while (temp->line[i])
        {
            putc(temp->line[i],fptr);
            i++;
        }

        putc('\n',fptr);   // Move to next line in file
        temp = temp -> next;
    }

    fclose(fptr);   // Close file
}


void displayText(TextEditor *editor)
{
    if(editor->head == NULL)
    {
        printf(YELLOW "Nothing to display\n" RESET);
        return;
    }

    Node *temp = editor->head;
    int line,pos;
    line = pos = 0;

    // Traverse entire linked list
    printf(YELLOW"");
    while(temp != NULL)
    {
        line++;

        // Check if current line is cursor line
        if(editor->cursorLine == line)
        {
            int i=0;

            while((editor -> cursor) -> line[i])
            {
                if(i == editor->cursorPos)
                {
                    printf(""RESET);
                    printf("🔺");   // Display cursor indicator
                    printf(YELLOW"");
                }
                
                printf("%c",(editor -> cursor) -> line[i]);
                i++;
            }
            // if((editor -> cursor) -> line[i] == '\0' && i == 0)
            if(editor->cursorPos == i)
            {
                printf("🔺");
            }

            printf("\n");
        }
        else
        {
            printf("%s\n",temp->line);   // Print normal line
        }

        temp = temp -> next;
    }
    printf(""RESET);

    // Display cursor position
    printf(BLUE "CURSOR POSTION:\n" RESET);
    printf("linenumber -> %d\n",editor->cursorLine);
    printf("position   -> %d\n",editor->cursorPos);
}


void insertText(TextEditor *editor, const char *text)
{
    // Save current state for undo
    Action action;
    action.operation = "insert";
    action.text = strdup((editor->cursor)->line);
    action.cursorLine = editor -> cursorLine;
    action.cursorPos = editor -> cursorPos;
    pushStack(&editor->undoStack,action);

    Node *temp = editor -> head;
    unsigned char j = 0,k=0;

    char updatedline[150];   // Temporary buffer for modified line

    while(temp != NULL)
    {
        if(temp == editor -> cursor)
        {
            int i=0;

            while(temp -> line[i])
            {
                if(editor ->cursorPos == i)
                {
                    // Insert new text
                    while(text[k])
                    {
                        updatedline[j++] = text[k++];
                    }
                }

                updatedline[j++] = temp -> line[i];
                i++;
            }

            // Handle insertion at end of line
            if(editor ->cursorPos == i)
            {
                while(text[k])
                {
                    updatedline[j++] = text[k++];
                }
            }

            updatedline[j] = '\0';

            char *new=malloc(strlen(updatedline)+1);
            strcpy(new, updatedline);

            free((editor -> cursor)->line);   // Free old line
            (editor -> cursor)->line = new;   // Update line

            break;  
        }

        temp = temp -> next;
    }
}


void deleteCharacters(TextEditor *editor, int length)
{
    delete_flag = 1;   // Set flag for delete operation

    // Store undo action
    Action action;
    action.operation = "delete_chars";
    action.text = strdup((editor->cursor)->line);
    action.cursorLine = editor -> cursorLine;
    action.cursorPos = editor -> cursorPos;
    pushStack(&editor->undoStack,action);

    Node *temp = editor->head;
    char updatedline[150];

    int j=0;

    while(temp != NULL)
    {  
        if(temp == editor -> cursor)
        {
            int size = strlen(temp->line);
            int i = 0; 

            while(temp->line[i])
            {
                if(i == editor -> cursorPos)
                {
                    i=i+length;

                    if(i >= size)
                    break;
                }

                updatedline[j++] = temp->line[i];
                i++;
            }

            updatedline[j] = '\0';

            // If line becomes empty call deleteLine
            if(strlen(updatedline) == 0)
            {
                deleteLine(editor);
            }
            else
            {
                delete_flag = 0;

                char *new=malloc(strlen(updatedline)+1);
                strcpy(new, updatedline);

                free((editor -> cursor)->line);
                (editor -> cursor)->line = new;
            }

            break;
        }

        temp = temp->next;
    }
}


void deleteLine(TextEditor *editor)
{
    // Prevent double undo push
    if(delete_flag == 1)
    {
        delete_flag = 0;
        popStack(&editor->undoStack);
    }

    // Store undo action
    Action action;
    action.operation = "delete_line";
    action.text = strdup((editor->cursor)->line);
    action.cursorLine = editor -> cursorLine;
    action.cursorPos = editor -> cursorPos;

    pushStack(&editor->undoStack,action);

    Node *temp = editor->head;

    while(temp != NULL)
    {
        if(editor -> cursor == temp)
        {
            // Replace line with empty string instead of deleting node
            char updatedline[1] = {'\0'};

            char *new=malloc(strlen(updatedline)+1);
            strcpy(new, updatedline);

            free((editor -> cursor)->line);
            (editor -> cursor)->line = new;

            break;
        }

        temp = temp -> next;
    }
}


void undo(TextEditor *editor)
{
    // Check if undo stack empty
    if(isStackEmpty(&editor->undoStack) == STACK_EMPTY)
    {
        printf(YELLOW "Nothing is there to undo\n" RESET);
        return;
    }

    // Pop last action
    Action action = popStack(&editor->undoStack);

    editor->cursorLine = action.cursorLine;
    editor -> cursorPos = action.cursorPos;

    Node *temp = editor->head;
    int line = 1;
    while(temp)
    {
        if(line == action.cursorLine)
        {
            editor->cursor = temp;
            break;
        }
        temp = temp->next;
        line++;
    }

    // Store redo action
    Action redoAction;
    redoAction.cursorLine = editor -> cursorLine;
    redoAction.cursorPos = editor -> cursorPos;
    redoAction.text = strdup((editor->cursor)->line);

    pushStack(&editor->redoStack, redoAction);

    // Restore previous line
    restore_line(editor,action);

    free(action.text);   // Free duplicated memory
}


void redo(TextEditor *editor)
{
    // Check if redo stack empty
    if(isStackEmpty(&editor->redoStack) == STACK_EMPTY)
    {
        printf(YELLOW "Nothing is there to redo\n" RESET);
        return;
    }

    // Pop redo action
    Action action = popStack(&editor->redoStack);

    editor->cursorLine = action.cursorLine;
    editor -> cursorPos = action.cursorPos;
    Node *temp = editor->head;
    int line = 1;
    while(temp)
    {
        if(line == action.cursorLine)
        {
            editor->cursor = temp;
            break;
        }
        temp = temp->next;
        line++;
    }
    
    // Store undo action
    Action undoAction;
    undoAction.cursorLine = editor -> cursorLine;
    undoAction.cursorPos = editor -> cursorPos;
    undoAction.text = strdup((editor->cursor)->line);

    pushStack(&editor->undoStack, undoAction);

    // Restore line
    restore_line(editor,action);

    free(action.text);
}


void restore_line(TextEditor *editor,Action action)
{
    Node *temp = editor->head;
    int line = 1;

    // Traverse to required line
    while(temp != NULL)
    {
        if(line == action.cursorLine)
        {
            char *new = malloc(strlen(action.text) + 1);
            strcpy(new, action.text);

            free(temp->line);   // Free existing line
            temp->line = new;   // Restore line

            editor->cursor = temp;
            editor->cursorLine = action.cursorLine;
            editor->cursorPos = action.cursorPos;

            break;
        }

        temp = temp->next;
        line++;
    }
}


void pushStack(DynamicArrayStack *stack,Action action)
{
    // Check if stack is full
    if(stack->size == (stack->capacity-1))
    {
        return;   // Do nothing if stack capacity reached
    }

    stack->size++;                     // Move stack top
    stack->actions[stack->size] = action;   // Push action into stack

    return;
}

Action popStack(DynamicArrayStack *stack)
{
    Action empty = {0};   // Empty action structure

    // Check if stack is empty
    if(stack->size == -1)
    {
        // printf(YELLOW "Nothing is there to undo\n" RESET);
        return empty;   // Return empty action
    }

    return stack->actions[stack->size--];   // Pop action and decrease stack size
}

int isStackEmpty(DynamicArrayStack *stack)
{
    // Check if stack contains no elements
    if(stack->size == -1)
    {
        return STACK_EMPTY;   // Stack is empty
    }
    else 
    {
        return STACK_NOT_FULL;   // Stack contains elements
    }
}