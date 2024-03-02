#include <cstring>
#include <stdio.h>
#include <termios.h>
#define SIZE 3
enum class tiles
{
    farmland = '=',
    wheat = 'W',
    corn = '8',
    melon = '#',
    cane = 'I',
};


enum class tile_plants
{
    none = '-',
    wheat = 'w',
    corn = '8',
    melon = 'r',
    cane = 'I',
    // MUST be in the end
    PLANT_MAX
};

int water_required[(int)tile_plants::PLANT_MAX]=
{
    0, // none
    1, // wheat_crop
    1, // corn
    1, // melon_crop
    5, // cane
};

enum class items
{
    wheat,
    wheat_seeds,
    melon,
    melon_seeds,
    corn,
    cane,
    watering_can_with_water,
    watering_can_without_water,
    // MUST be in the end
    ITEM_MAX
};

const char * items_names [(int)items::ITEM_MAX]=
{
    "wheat",
    "wheat_seeds",
    "melon",
    "melon_seeds",
    "corn",
    "cane",
    "watering can with water",
    "watering can without water",
};

struct Player
{
    int x;
    int y;
    int inventory[(int)items::ITEM_MAX];
} player;

typedef struct
{
    int water;
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
            world_obj * p = &world[i][j];
            plants * pp = &p->plant;
            p->type = tiles::farmland;
            pp->type = tile_plants::none;
            pp->stage = 0;
            pp->double_stage = 0;
        }
    }
    
    for (int i=0; i<SIZE; i++)
    {
        for (int j=0; j<SIZE; j++)
        {
            world[i][j].plant.water=0;
        }
    }

    player.inventory[(int)items::melon_seeds] = 2;
    player.inventory[(int)items::wheat_seeds] = 2;
    player.inventory[(int)items::cane] = 2;
    player.inventory[(int)items::corn] = 2;

    struct termios state, new_state;
    tcgetattr(0, &state);
    new_state=state;
    new_state.c_lflag &= ~(ECHO | ICANON |ECHOE| ISIG);
    new_state.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_state);
    char a;
    player.x=0;
    player.y=0;
    
    for (;;)
    {
        printf("-----------------------------------------------\n");
        printf("\nPLANTS:\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                printf("%c", (char)(world[i][j].plant.type));
            }
            printf("\n");
        }

        printf("\nWATER\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                printf("%5d ", world[i][j].plant.water);
            }
            printf("\n");
        }
        printf("\nTILES\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                printf("%c", (char)(world[i][j].type));
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

        printf("\nDOUBLE STAGES\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                printf("%d", world[i][j].plant.double_stage);
            }
            printf("\n");
        }

        printf("x: %d, y: %d\n", player.x, player.y);
        a=getchar();
        world_obj * p;
        switch (a)
        {
            case 'w':
                if (player.y > 0)
                {
                    player.y--;
                }
                break;

            case 's':
                if (player.y < SIZE-1)
                {
                    player.y++;
                }
                break;
            case 'd':
                if (player.x < SIZE-1)
                {
                    player.x++;
                }
                break;
            case 'a':
                if (player.x > 0)
                {
                    player.x--;
                }
                break;

            case 'i':
                printf("\n");
                for (int i = 0; i < (int)items::ITEM_MAX; i++)
                {
                    printf("%s: %d\n", items_names[i], player.inventory[i]);
                }
                break;
                
            case 'b':
                p = &world[player.y][player.x];

                if (p->type == tiles::melon)
                {
                    p->type = tiles::farmland;
                    player.inventory[(int)items::melon] += 1;
                    player.inventory[(int)items::melon_seeds] += 1;
                }
                if (p->type == tiles::wheat)
                {
                    p->type = tiles::farmland;
                    player.inventory[(int)items::wheat] += 1;
                    player.inventory[(int)items::wheat_seeds] += 1;
                }
                if (p->type == tiles::cane)
                {
                    p->type = tiles::farmland;
                    player.inventory[(int)items::cane] += 5;
                }
                if (p->type == tiles::corn)
                {
                    p->type = tiles::farmland;
                    player.inventory[(int)items::corn] += 50;
                }
                break;
            case 'h':
                printf("wasd - select place\n");
                printf("h - show this message\n");
                printf("q - quit\n");
                printf("p - plant\n");
                printf("i - show inventory\n");
                printf("b - break (harvest) crop\n");
                printf("g - get watering can\n"); 
                printf("S - set watering can amount to 500\n"); 
                printf("r - refill watering can\n"); 
                printf("W - water crop with watering can\n"); 
                break;
            case 'S':
                player.inventory[(int)items::watering_can_with_water]=500;
                break;
            case 'g':
                player.inventory[(int)items::watering_can_with_water]++;
                break;
            case 'r':
                if (player.inventory[(int)items::watering_can_without_water])
                {
                    player.inventory[(int)items::watering_can_with_water]++;
                    player.inventory[(int)items::watering_can_without_water]--;
                }
                else
                {
                    printf("You need a watering can!\n");
                }
                break;
            case 'q':
                return 0;
            case 'W':
                if (!player.inventory[(int)items::watering_can_with_water])
                {
                    printf("\nNah, didn't water, you don't have watering can with water\n");
                    break;
                }
                world[player.y][player.x].plant.water+=10;
                player.inventory[(int)items::watering_can_with_water]--;
                player.inventory[(int)items::watering_can_without_water]++;
                printf("\n");
                break;
            case 'p':
                printf("\nm - melon\n");
                printf("w - wheat\n");
                printf("c - cane\n");
                printf("C - corn\n");
                printf("\ntype (m / w / c / C): ");
                char type = getchar();
                p = &world[player.y][player.x];
                plants * pp = &p->plant;
                switch (type)
                {
                    case 'm':
                        if (player.inventory[(int)items::melon_seeds])
                        {   
                            pp->type = tile_plants::melon;
                            player.inventory[(int)items::melon_seeds]--;
                        }  
                        else
                            printf("Nah, didn't plant, no seeds\n");
                        break;
                    case 'w':
                        if (player.inventory[(int)items::wheat_seeds])
                        {
                            pp->type = tile_plants::wheat;
                            player.inventory[(int)items::wheat_seeds]--;
                        }
                        else
                            printf("Nah, didn't plant, no seeds\n");
                        break;
                    case 'c':
                        if (player.inventory[(int)items::cane])
                        {
                            pp->type = tile_plants::cane;
                            player.inventory[(int)items::cane]--;
                        }
                        else
                            printf("Nah, didn't plant, no seeds\n");
                        break;
                    case 'C':
                        if (player.inventory[(int)items::corn])
                        {
                            pp->type = tile_plants::corn;
                            player.inventory[(int)items::corn]--;
                        }
                        else
                            printf("Nah, didn't plant, no seeds\n");
                        break;
                }
                break;
        }
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (world [i][j].plant.water >= water_required[(int)world[i][j].plant.type] && world[i][j].plant.type != tile_plants::none)
                {
                    world_obj * p = &world[i][j];
                    plants * pp = &p->plant;

                    pp->stage++;
                    if (pp->stage == 10)
                    {
                        pp->stage=9;
                        if (p->type == tiles::farmland)
                        {
                            if (pp->double_stage > 5)
                            {
                                pp->double_stage=0;
                                p->type=(tiles)pp->type;
                            }
                            else pp->double_stage++;
                        }
                    }
                    pp->water--;
                }
            }
        }
    }
}
