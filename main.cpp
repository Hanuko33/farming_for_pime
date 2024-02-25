#include <cstring>
#include <stdio.h>
#include <termios.h>
#define SIZE 3
enum class tiles
{
    farmland,
    melon,
    wheat,
    corn,
    cane
};
enum class tile_plants
{
    none,
    wheat_crop,
    corn,
    melon_crop,
    cane,
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
            world[i][j].type = tiles::farmland;
            world[i][j].plant.type = tile_plants::none;
            world[i][j].plant.stage = 0;
            world[i][j].plant.double_stage = 0;
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
    
    for (;;)
    {
        printf("-----------------------------------------------\n");
        printf("\nPLANTS:\n");
        for (int i=0; i<SIZE; i++)
        {
            for (int j=0; j<SIZE; j++)
            {
                switch (world[i][j].plant.type)
                {
                    case tile_plants::PLANT_MAX: break;
                    case tile_plants::none:
                        printf("-");
                        break;
                    case tile_plants::cane:
                        printf("I");
                        break;
                    case tile_plants::corn:
                        printf("8");
                        break;
                    case tile_plants::melon_crop:
                        printf("r");
                        break;
                    case tile_plants::wheat_crop:
                        printf("w");
                        break;
                }
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
                switch (world[i][j].type)
                {
                    case tiles::farmland:
                        printf("=");
                        break;
                    case tiles::melon:
                        printf("#");
                        break;
                    case tiles::wheat:
                        printf("w");
                        break;
                    case tiles::cane:
                        printf("I");
                        break;
                    case tiles::corn:
                        printf("8");
                        break;
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
                y = (getchar()-'0');
                printf("\ny: ");
                x = (getchar()-'0');
                printf("\n");
                if (x >= SIZE || y >= SIZE || x < 0 || y < 0) 
                {
                    printf("\nNah, didn't break, too far away\n");
                    break;
                } 
                if (world[y][x].type == tiles::melon)
                {
                    world[y][x].type = tiles::farmland;
                    player.inventory[(int)items::melon] += 1;
                    player.inventory[(int)items::melon_seeds] += 1;
                }
                if (world[y][x].type == tiles::wheat)
                {
                    world[y][x].plant.type = tile_plants::none;
                    world[y][x].plant.stage = 0;
                    world[y][x].type = tiles::farmland;
                    player.inventory[(int)items::wheat] += 1;
                    player.inventory[(int)items::wheat_seeds] += 1;
                }
                if (world[y][x].type == tiles::cane)
                {
                    world[y][x].type = tiles::farmland;
                    player.inventory[(int)items::cane] += 5;
                }
                if (world[y][x].type == tiles::corn)
                {
                    world[y][x].plant.type = tile_plants::none;
                    world[y][x].type = tiles::farmland;
                    world[y][x].plant.stage = 0;
                    player.inventory[(int)items::corn] += 50;
                }
                break;
            case 'h':
                printf("h - show this message\n");
                printf("q - quit\n");
                printf("p - plant\n");
                printf("i - show inventory\n");
                printf("b - break (harvest) crop\n");
                printf("g - get watering can\n"); 
                printf("s - set watering can amount to 500\n"); 
                printf("r - refill watering can\n"); 
                printf("w - water crop with watering can\n"); 
                break;
            case 's':
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
            case 'w':
                printf("x: ");
                x = (getchar()-'0');
                printf("\ny: ");
                y = (getchar()-'0');
                if (x >= SIZE || y >= SIZE || y < 0 || x < 0) 
                {
                    printf("\nNah, didn't water, too far away\n");
                    break;
                } 
                if (!player.inventory[(int)items::watering_can_with_water])
                {
                    printf("\nNah, didn't water, you don't have watering can with water\n");
                    break;
                }
                world[y][x].plant.water+=10;
                player.inventory[(int)items::watering_can_with_water]--;
                player.inventory[(int)items::watering_can_without_water]++;
                printf("\n");
                break;
            case 'p':
                printf("x: ");
                x = (getchar()-'0');
                printf("\ny: ");
                y = (getchar()-'0');
                printf("\nm - melon\n");
                printf("w - wheat\n");
                printf("c - cane\n");
                printf("C - corn\n");
                printf("\ntype (m / w / c / C): ");
                char type = getchar();
                printf("\n%d %d\n", x, y);
                printf("\n");
                if (x >= SIZE || y >= SIZE || y < 0 || x < 0) 
                {
                    printf("\nNah, didn't plant, too far away\n");
                    break;
                } 
                switch (type)
                {
                    case 'm':
                        if (player.inventory[(int)items::melon_seeds])
                        {   
                            world[y][x].plant.type = tile_plants::melon_crop;
                            player.inventory[(int)items::melon_seeds]--;
                        }  
                        else
                            printf("Nah, didn't plant, no seeds\n");
                        break;
                    case 'w':
                        if (player.inventory[(int)items::wheat_seeds])
                        {
                            world[y][x].plant.type = tile_plants::wheat_crop;
                            player.inventory[(int)items::wheat_seeds]--;
                        }
                        else
                            printf("Nah, didn't plant, no seeds\n");
                        break;
                    case 'c':
                        if (player.inventory[(int)items::cane])
                        {
                            world[y][x].plant.type = tile_plants::cane;
                            player.inventory[(int)items::cane]--;
                        }
                        else
                            printf("Nah, didn't plant, no seeds\n");
                        break;
                    case 'C':
                        if (player.inventory[(int)items::corn])
                        {
                            world[y][x].plant.type = tile_plants::corn;
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
                if (world [i][j].plant.water >= water_required[(int)world[i][j].plant.type])
                {
                    // Melon Growing
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
                    // Wheat Growing
                    if (world[i][j].plant.type == tile_plants::wheat_crop)
                    {
                        world[i][j].plant.stage++;
                    }
                    if (world[i][j].plant.stage > 9)
                    {
                        world[i][j].plant.stage = 9;
                        world[i][j].type = tiles::wheat;
                    }
                    // Corn growing
                    if (world[i][j].plant.type == tile_plants::corn)
                    {
                        world[i][j].plant.stage++;
                    }
                    if (world[i][j].plant.stage > 9)
                    {
                        world[i][j].plant.stage=9;
                        world[i][j].type = tiles::corn;
                    }
                    // Cane growing
                    if (world[i][j].plant.type == tile_plants::cane)
                    {
                        world[i][j].plant.stage++;
                    }
                    if (world[i][j].plant.stage == 10 && world[i][j].plant.type == tile_plants::cane)
                    {
                        world[i][j].plant.stage=9;
                        if (world[i][j].plant.double_stage > 5 && world[i][j].plant.stage == 9)
                        {
                            world[i][j].plant.double_stage=0;
                            world[i][j].type=tiles::cane;
                        }
                        else if (world[i][j].plant.stage==9 && world[i][j].type!=tiles::cane)
                        {
                            world[i][j].plant.double_stage++;
                        }
                    }
                    if (world[i][j].plant.water) world[i][j].plant.water--;
                }
            }
        }
    }
}
