#include <stdio.h>
#include <termios.h>
#define SIZE 2
enum class tiles
{
    farmland,
    melon
};

enum class tile_plants
{
    none,
    wheat,
    corn,
    melon_crop,
    cane
};

enum class items
{
    wheat,
    melon,
    corn,
    cane,
    // MUST be in the end
    ITEM_MAX
};

const char * items_names [(int)items::ITEM_MAX]=
{
    "wheat",
    "melon",
    "corn",
    "cane",
};

struct Player
{
    int inventory[(int)items::ITEM_MAX];
} player;

typedef struct
{

   int stage;
   int double_stage;
   tile_plants type;

} plants;

typedef struct
{
    tiles type;
    plants plant;
} world_obj;

int main()
{

    char input;
    world_obj world[SIZE][SIZE];
    for (int i=0; i<SIZE; i++)
    {
        for (int j=0; j<SIZE; j++)
        {
            world[i][j].type = tiles::farmland;
            world[i][j].plant.type = tile_plants::none;
            world[i][j].plant.stage = 0;
            world[i][j].plant.double_stage = 0;
        }
    }
    struct termios state, new_state;
    tcgetattr(0, &state);
    new_state=state;
    new_state.c_lflag &= ~(ECHO | ICANON |ECHOE| ISIG);
    new_state.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_state);
    char a;
    
    for (;;)
    {
        printf("\nPLANTS:\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                switch (world[i][j].plant.type)
                {
                    case tile_plants::none:
                        printf(" ");
                        break;
                    case tile_plants::cane:
                        printf("I");
                        break;
                    case tile_plants::corn:
                        printf("|");
                        break;
                    case tile_plants::melon_crop:
                        printf("r");
                        break;
                    case tile_plants::wheat:
                        printf("w");
                        break;
                }
            }
            printf("\n");
        }

        printf("\nTILES\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                switch (world[i][j].type)
                {
                    case tiles::farmland:
                        printf("=");
                        break;
                    case tiles::melon:
                        printf("#");
                }
            }
            printf("\n");
        }
        printf("\nSTAGES\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                printf("%d", world[i][j].plant.stage);
            }
            printf("\n");
        }

        printf("\nDOUBLE STAGES (for things like melons)\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                printf("%d", world[i][j].plant.double_stage);
            }
            printf("\n");
        }


        a=getchar();
        int x;
        int y;
        switch (a)
        {
            case 'i':
                printf("\n");
                for (int i = 0; i < (int)items::ITEM_MAX; i++)
                {
                    printf("%s: %d\n", items_names[i], player.inventory[i]);
                }
                break;
            case 'b':
                printf("x: ");
                x = (getchar()-'0');
                printf("\ny: ");
                y = (getchar()-'0');
                if (world[x][y].type == tiles::melon)
                {
                    world[x][y].type = tiles::farmland;
                    player.inventory[(int)items::melon] += 1;
                }
                break;
            case 'q':
                return 0;
            case 'p':
                printf("x: ");
                x = (getchar()-'0');
                printf("\ny: ");
                y = (getchar()-'0');
                printf("\ntype (m): ");
                char type = getchar();
                printf("%d %d\n", x, y);
                if (x >= SIZE || y >= SIZE) 
                {
                    printf("\nNah, didn't plant, too far away\n");
                    break;
                } 
                switch (type)
                {
                    case 'm':
                        world[x][y].plant.type = tile_plants::melon_crop;
                        break;
                }
                break;
        }
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (world[i][j].plant.type == tile_plants::melon_crop)
                {
                    world[i][j].plant.stage++;
                }
                if (world[i][j].plant.stage == 10 && world[i][j].plant.type == tile_plants::melon_crop)
                {
                    world[i][j].plant.stage=9;
                    if (world[i][j].plant.double_stage > 5 && world[i][j].plant.stage==9)
                    {
                        world[i][j].plant.double_stage=0;
                        world[i][j].type=tiles::melon;
                    }
                    else if (world[i][j].plant.stage==9 && world[i][j].type!=tiles::melon)
                    {
                        world[i][j].plant.double_stage++;
                    }
                }
            }
        }
    }
}
