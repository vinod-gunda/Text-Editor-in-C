#include "text_editor.h"

int main(int argc,char *argv[])
{
    
    // Initialize the text editor and stack structures
    TextEditor editor;
    editor.head = editor.tail = editor.cursor = NULL;   // Initializing linked list pointers
    editor.cursorLine = 1;                               // Cursor starts at first line
    editor.cursorPos = 0;                                // Cursor starts at first position
    char filename[20];
    init(&editor);                                       // Initialize undo and redo stacks
    if(argc < 2)
    {
        Node *new = malloc(sizeof(Node));
        new->line = malloc(1);
        new->line[0] = '\0';
        new -> prev = new -> next = NULL;
        editor.head = editor.tail = editor.cursor = new;
    }
    else
    {
        strcpy(filename,argv[1]);
        read_file_to_list(&editor,argv[1]);              // Read file content into linked list
    }
    
    

    char command[256];
    printf(CYAN "=====================================\n" RESET);
    printf(GREEN "        SIMPLE TEXT EDITOR\n" RESET);
    printf(CYAN "=====================================\n" RESET);

    // User interaction loop
    while (1) 
    {
        
        // Display menu options
        printMenu();

        memset(command,0,sizeof(command));               // Clear previous command buffer

        printf(CYAN "Enter command: " RESET);
        __fpurge(stdin);                                 // Clear input buffer
        scanf("%[^\n]",command);                         // Read command from user

        // Exit condition
        if (strcmp(command, "exit") == 0) 
        {
            char var = 'y';

            printf(YELLOW "Do you want to save file(y/n): " RESET);
            __fpurge(stdin);
            scanf("%c",&var);

            // Save file if user selects yes
            if(var == 'y')
            {
                if(argc < 2)
                {
                    printf(YELLOW"Enter the file name:"RESET);
                    __fpurge(stdin);
                    scanf("%s",filename);
                }
                saveFile(&editor,filename);
            }
            break;                                       // Exit program
        } 
        else 
        {
            handleCommand(command, &editor);             // Process entered command
        }
    }

    return 0;
}


void printMenu()
{
    // Display available editor commands
    printf(MAGENTA "\nText Editor Commands:\n" RESET);

    printf(GREEN "1. insert" RESET " -> Insert text at the current cursor position\n");
    printf(GREEN "2. delete <num_chars>" RESET " -> Delete a number of characters from the current cursor position\n");
    printf(GREEN "3. undo" RESET " -> Undo the last operation\n");
    printf(GREEN "4. redo" RESET " -> Redo the last undone operation\n");
    printf(GREEN "5. print" RESET " -> Print the current state of the text editor\n");
    printf(GREEN "6. navigate" RESET " -> Navigate the cursor position\n");
    printf(GREEN "7. exit" RESET " -> Exit the text editor\n");
}


void handleCommand(char *cmd,TextEditor *editor)
{
    // Navigation command
    if(strcasecmp(cmd,"navigate") == 0)
    {
        navigate_operations(editor);
    }

    // Print current text
    else if(strcasecmp(cmd,"print") == 0)
    {
        displayText(editor);
    }

    // Insert text command
    else if(strcasecmp(cmd,"insert") == 0)
    {
        char text[100];

        printf(CYAN "Enter the text to be insert: " RESET);
        __fpurge(stdin);
        scanf("%[^\n]",text);

        insertText(editor,text);
    }

    // Undo operation
    else if(strcasecmp(cmd,"undo") == 0)
    {
        undo(editor);
    }

    // Redo operation
    else if(strcasecmp(cmd,"redo") == 0)
    {
        redo(editor);
    }

    // Delete command (delete characters or delete entire line)
    else if(strstr(cmd,"delete") != NULL)
    {
        char opt = 0;

        // Extract number of characters from command
        sscanf(cmd,"delete %hhd",&opt);

        // Delete N characters
        if(opt > 0)
        {
            deleteCharacters(editor, opt);
        }
        // Delete entire line
        else if(opt == 0)
        {
            deleteLine(editor);
        }
    }

    // Invalid command handling
    else
    {
        printf(RED "Invalid Command\n" RESET);
    }
}


// Stack initialization function
void init(TextEditor *editor)
{
    // Initializing Undo stack
    editor->undoStack.capacity = 10;                     // Maximum stack capacity
    editor->undoStack.size = -1;                         // Stack initially empty
    editor->undoStack.actions = malloc(10 * sizeof(Action)); // Allocate memory for actions

    // Initializing Redo stack
    editor->redoStack.capacity = 10;                     // Maximum stack capacity
    editor->redoStack.size = -1;                         // Stack initially empty
    editor->redoStack.actions = malloc(10 * sizeof(Action)); // Allocate memory for actions
}