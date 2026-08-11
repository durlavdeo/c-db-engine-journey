//finally file handling..
//basic text files

#include<stdio.h>
#include<stdlib.h>


void addNote()
{
    FILE *fp;
    fp = fopen("notes.txt", "a");
    if(fp==NULL)
    {
        printf("Error opening file!");
        exit(1);
    }
    char note[100];
    printf("Enter note: ");
    scanf(" %99[^\n]", note);
    fprintf(fp,"%s\n", note);
    fclose(fp);
    return;
}


void listNotes()
{

    FILE *fp;
    fp = fopen("notes.txt", "r");
    if(fp==NULL)
    {
        printf("Error opening file!");
        exit(1);
    }
    char buffer[100];
    while( (fgets(buffer,sizeof(buffer), fp)) != NULL  )
    {
        printf("%s", buffer);
    }
    fclose(fp);
    return;
}

int main()
{
    int choice;

    while(1)
    {
        printf("Choose:\n");
        printf("1. Add notes\n");
        printf("2. List notes\n");
        printf("3. Exit\n");

        scanf("%d", &choice);

        switch (choice)
        {

        case 1:
            addNote();
            break;
        case 2:
            listNotes();
            break;
        case 3:
            exit(0);
        default :
            printf("Choose valid option!");
            break;
        }
    }

    return 0;
}
