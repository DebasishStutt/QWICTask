#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init();
void reversePriority();
void takeInput();
char getNextCellValue(char direction);
void setCurrentDirection(char dir);
void findCurrentDirection();
void updatePositionCoordinates();
void moveInCurrentDirection(char direction);
void deleteMemory();
void enterBreakerMode();
void printPath();


static int breaker_mode;
static char **map;
static char current_dir;// direction_prio[4] = {'S','E','N','W'};
static int map_r, map_c,r, c, inverse_order;//global row column iterator and order priority indicator
static int infiniteLoop;// abort_program;

/*	unified exception handling structure
	All parts of the code that experiences
	an exception/error writes the message
	signals such a condition.
*/
struct abortHandler
{
    int abortsignal;
    char msg[50];//arbitrarily enough size
}abort_program;

/*linked list to store the path taken*/

typedef struct pnode //stores the path traversed and if not infinite loop then print to console.
{
    char dir[5];
    struct pnode *next;
}pnode_t;

pnode_t *head, *curr;


int main()
{
    char x;

    //printf("Hello world!\n");

    // take map input
    takeInput();

    //initialie some params to be used
    init();


    while(1)
    {
        infiniteLoop++;
        x = getNextCellValue(current_dir);

        if(x == 1)
        {
            //printf();
            abort_program.abortsignal = 1;
            strcpy(abort_program.msg,"\nUndefined Error: Could not fetch next cell value. Program will exit!!");

        }

        if(infiniteLoop == 100)//when the map is so set up that LG-Automaton never reaches $
        {
            abort_program.abortsignal = 1;
            strcpy(abort_program.msg, "LOOP");
        }

        if(abort_program.abortsignal)//if somebody has requested an abort
        {
            printf("\n%s\n",abort_program.msg);
            break;
        }
        switch(x)
        {
        case '$':
            moveInCurrentDirection(current_dir);
            printPath();
            abort_program.abortsignal = 1; //only case where this struct is used to convey a normal end to the program
            strcpy(abort_program.msg,"\n\nAutomaton-LG is no more!!\n\n");
            break;

        case ' ':
            moveInCurrentDirection(current_dir);
            break;

        case 'X':

            if(!breaker_mode)//obstacles matter
            {
                findCurrentDirection();
            }
            else//obstacles destroyed
            {
                moveInCurrentDirection(current_dir);
                map[r][c] = ' ';//destroying the obstacle X if in breaker mode
            }
            break;

        case'#':

            findCurrentDirection();
            //moveInCurrentDirection(current_dir);
            break;

        case 'N':
        case 'S':
        case 'E':
        case 'W':
        case 'I':

            moveInCurrentDirection(current_dir);//common activity in any case
            if((x == 'N')||(x == 'S')||(x == 'E')||(x == 'W'))
            {
                    setCurrentDirection(x);
            }
            if(x == 'I')
            {
                reversePriority();
                findCurrentDirection();
            }
            break;

        case 'B':

            moveInCurrentDirection(current_dir);
            enterBreakerMode();
            break;

        case 'T':
            moveInCurrentDirection(current_dir);
            updatePositionCoordinates();
            //printf(" :: Teleported!!");
            break;

        case '@': //not a part of the specification but just for more info
            moveInCurrentDirection(current_dir);
            //printf(" :: LG-Automaton returned to its starting position!!");
            break;

        default:
            //printf("\n*****Unknown map element! Aborting process...!*****\n");
            abort_program.abortsignal = 1;
            strcpy(abort_program.msg, "\nUnknown map element!Aborting process!\n");
        }

    }


    //delete the map and free the memory
    deleteMemory();
    return 0;
}



void deleteMemory()
{
    pnode_t *curr;
    int i;
    for(i =0; i < map_r; i++)
    {
        free(map[i]);
    }
    free(map);

    while(head != NULL)
    {
        curr = head;
        head = head->next;
        free(curr);
    }
}
void init()
{
    //find the start position i.e. @
    int i,j;
    for(i = 0; i < map_r; i++)
    {
        for(j = 0; j < map_c; j++)
        {
            if(map[i][j] == '@')
            {
                r = i;//map_r;
                c = j;//smap_c;

                break;//so now r and c has the coordinates of @
            }

        }

        if(map[r][c] == '@')//to terminate the outer loop
        {
            break;
        }
    }

    //default current direction is South
    current_dir = 'S';


    //infinite loop counter
    infiniteLoop = 0;

    //init the error signal
    abort_program.abortsignal = 0;

    //direction prio inversed
    inverse_order = 0;

    //breaker mode off
    breaker_mode = 0;

    //the path linked list
    head = NULL;
    //head = malloc(sizeof(node_t));
    //if (head == NULL)
    //{
     //   return 1;
    //}

    //head->val = 1;
    //head->next = NULL;
    //maybe more init tasks would be required
}

//input map
void takeInput()
{
    int i,j,k;
    char inputstring[256], r_str[4], c_str[4];
    //char *line;
    printf("Please enter the number of rows(>=4 and <=100) and columns(>=4 and <=100) of the map separated by space...\n");
    fgets(inputstring, sizeof(inputstring), stdin);
    fflush(stdin);

    for(i =0; ; i++)
    {
        if(inputstring[i] != ' ')
        {
            r_str[i] = inputstring[i];
        }
        else if(inputstring[i] == ' ')
        {
            r_str[i] = '\0';
            break;
        }
        else if(inputstring[i] == '\0')
        {
            printf("You didn't provide number of columns. Program will exit.\n");
            exit(1);
        }
    }
    if(i == 0)
    {
        printf("You didn't provide number of rows. Program will exit.\n");
        exit(1);
    }
    else
    {
        map_r = atoi(r_str);
    }


    for(k=0, j = i+1; ;j++)
    {
        if(inputstring[j] == '\0')
        {
            c_str[j] = '\0';
            break;
        }
        else
        {
            c_str[k] = inputstring[j];
            k++;
        }
    }
    if(j == (i+1))
    {
        printf("You didn't provide number of columns. Program will exit.\n");
        exit(1);
    }
    else
    {
        map_c = atoi(c_str);
    }

    printf("So, rows = %d   and   columns = %d\n", map_r, map_c);
    if(((map_r < 4) || (map_r > 100)) || (map_c < 4) || (map_c > 100))
    {
        printf("The valid constraints are:\n        4 <= row <= 100\n        4 <= col <= 100\nProgram will exit.");
        exit(1);
    }


    map = (char **)malloc(map_r * sizeof(char *));
    //sline = malloc((c+1) * sizeof(char));

    for (i=0; i<map_r; i++)
    {
        map[i] = (char *)malloc((map_c+1) * sizeof(char)); //c+1 to accommodate a null character
    }

    printf("Enter the map one line at a time!\n");

    for(i = 0; i < map_r; i++)
    {
        scanf("%[^\n]%*c", map[i]);
        //fgets (map[i], (map_c+1), stdin);
        //line[c] = '\0';
        fflush(stdin);
        //puts(map[i]);
    }
    printf("\nThe map looks like:\n");

	for(i = 0; i < map_r; i++)
    {
        puts(map[i]);
    }
    fflush(stdout);
}

char getNextCellValue(char direction)
{

    switch(direction)
    {
        case 'N':
            //r--;
            return map[r-1][c];
            break;

        case 'S':
            //r++;
            return map[r+1][c];
            break;

        case 'E':
            //c++;
            return map[r][c+1];
            break;

        case 'W':
            //c--;
            return map[r][c-1];
            break;
    }

    return 1;
}

void setCurrentDirection(char dir)//maybe make this a macro
{
    current_dir = dir;
}

void findCurrentDirection()
{
    switch(inverse_order)
    {

    case 0:
        //call setCurrentDirection  and pas what the current direction should be
        if(breaker_mode == 0)//two kinds of obstacles exist X and #
        {
            if((map[r+1][c] != 'X') && (map[r+1][c] != '#'))
            {
                setCurrentDirection('S');
            }
            else if((map[r][c+1] != 'X') && (map[r][c+1] != '#'))
            {
                setCurrentDirection('E');
            }
            else if((map[r-1][c] != 'X') && (map[r-1][c] != '#'))
            {
                setCurrentDirection('N');
            }
            else if((map[r][c-1] != 'X') && (map[r][c-1] != '#'))
            {
                setCurrentDirection('W');
            }
            else
            {
                //printf("\nLG-Automaton stuck with obstacles all around!!");
                abort_program.abortsignal = 1;
                strcpy(abort_program.msg,"\nLG-Automaton stuck with obstacles all around!!");
            }
        }
        else//only # is an obstacle now
        {
            if(map[r+1][c] != '#')
            {
                setCurrentDirection('S');
            }
            else if((map[r][c+1] != '#'))
            {
                setCurrentDirection('E');
            }
            else if((map[r-1][c] != '#'))
            {
                setCurrentDirection('N');
            }
            else if((map[r][c-1] != '#'))
            {
                setCurrentDirection('W');
            }
            else
            {
                //printf("\nLG-Automaton stuck in breaker mode with map boundary all around!!");
                abort_program.abortsignal = 1;
                strcpy(abort_program.msg,"\nLG-Automaton stuck in breaker mode with boundary all around!!");
            }
        }

        break;

    case 1:

        if(breaker_mode == 0)
        {

            if((map[r][c-1] != 'X') && (map[r][c-1] != '#'))
            {
                setCurrentDirection('W');
            }
            else if((map[r-1][c] != 'X') && (map[r-1][c] != '#'))
            {
                setCurrentDirection('N');
            }
            else if((map[r][c+1] != 'X') && (map[r][c+1] != '#'))
            {
                setCurrentDirection('E');
            }
            else if((map[r+1][c] != 'X') && (map[r+1][c] != '#'))
            {
                setCurrentDirection('S');
            }
            else
            {
                //printf("\nLG-Automaton stuck with obstacles all around!!");
                abort_program.abortsignal = 1;
                strcpy(abort_program.msg,"\nLG-Automaton stuck with obstacles all around!!");
            }
        }
        else
        {
            if((map[r][c-1] != '#'))
            {
                setCurrentDirection('W');
            }
            else if((map[r-1][c] != '#'))
            {
                setCurrentDirection('N');
            }
            else if((map[r][c+1] != '#'))
            {
                setCurrentDirection('E');
            }
            else if((map[r+1][c] != '#'))
            {
                setCurrentDirection('S');
            }
            else
            {
                //printf("\nLG-Automaton stuck in reverse priority and breaker mode with boundary all around!!");
                abort_program.abortsignal = 1;
                strcpy(abort_program.msg,"\nLG-Automaton stuck in reverse direction priority and breaker mode with boundary all around!!");
            }
        }
        break;
    default:
        abort_program.abortsignal = 1;
        strcpy(abort_program.msg,"\nUnknown issue uncovered in finding current direction.");
    }
}


void updatePositionCoordinates()
{

    int Tr, Tc;

    for(Tr = 0; Tr < map_r; Tr++)
    {
        for(Tc = 0; Tc < map_c; Tc++)
        {
            if(((Tr != r) || (Tc != c)) && (map[Tr][Tc] == 'T'))
            {
                r = Tr;
                c = Tc;
                break;
            }
        }
        if(map[Tr][Tc] == 'T')
        {
            break;
        }
    }

}

void moveInCurrentDirection(char direction)
{
    pnode_t *temp;
    if(head == NULL)
    {
        head = malloc(sizeof(pnode_t));
        head->next = NULL;
        curr = head;
    }
    else
    {
        while(curr ->next != NULL)
        {
            curr = curr->next;
        }
        temp = malloc(sizeof(pnode_t));
        temp->next = NULL;
        curr->next = temp;
        curr = curr ->next;
    }
    switch(direction)
    {
    case 'N':
        //printf("\nNorth");
        strcpy(curr->dir, "North");
        r--;
        break;

    case 'S':
        r++;
        strcpy(curr->dir, "South");
        //printf("\nSouth");
        break;

    case 'E':
        c++;
        strcpy(curr->dir, "East");
        //printf("\nEast");
        break;

    case 'W':
        c--;
        strcpy(curr->dir, "West");
        //printf("\nWest");
        break;

    default:
        abort_program.abortsignal = 1;
        strcpy(abort_program.msg,"\nUnknown issue uncovered while trying to move in current direction");
    }
}


void printPath()
{
    pnode_t *curr = head;

    while(curr != NULL)
    {
        printf("\n%s", curr->dir);
        curr = curr->next;
    }
}
void enterBreakerMode()
{
    breaker_mode ^= 1;
}

void reversePriority()
{
    inverse_order ^= 1;
}
