#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define START_POS_X 1720 
#define START_POS_Y 45

#define NUM_COIN 50

#define NUM_ZOMBIES  20

#define NUM_SQUARE 196

#define NUM_LIFE 10

typedef struct{
    Rectangle zombiehitBox;
    int lifezombie;
}Zombie;

Zombie * loadZombies(int num_zombies, Zombie * zombies, Rectangle * paredes,  Texture zombie){
    zombies = (Zombie *)malloc(sizeof(Zombie) * num_zombies);
    srand(time(NULL));
    for (int i = 0; i < num_zombies; i++){
        Rectangle zumbiHitBox = {
            .x = 30 + (rand() % 3500),
            .y = 30 + (rand() % 3000),
            .width = zombie.width,
            .height = zombie.height
        };
        for (int j = 0; j < NUM_SQUARE; j++){
            for (int k = 0; k < num_zombies; k++){
                if(CheckCollisionRecs(zumbiHitBox, paredes[j]) || CheckCollisionRecs(zumbiHitBox, zombies[k].zombiehitBox)){
                    zumbiHitBox.x = 30 + (rand() % 3500);
                    zumbiHitBox.y = 30 + (rand() % 3000);
                    j = 0; 
                    k = 0;
                }
            }                
        }
        //int k = 0;
        zombies[i].zombiehitBox = zumbiHitBox;
        zombies[i].lifezombie = 1;
    }

    return zombies;
}

Rectangle * loadCoin(int num_coin, Rectangle * moedas, Rectangle * paredes, Texture coin)
{
    moedas = (Rectangle *)malloc( sizeof(Rectangle) * (num_coin + 1));
    srand(time(NULL));
    for (int j = 0; j < num_coin; j++){
            Rectangle moeda = {
                .x = 30 + (rand() % 3500),
                .y = 30 + (rand() % 3000),
                .width = coin.width/7,                
                .height = coin.height,
            };
        for (int i = 0;  i < NUM_SQUARE; i++){
                if(CheckCollisionRecs(moeda, paredes[i])){
                    moeda.x = 30 + (rand() % 3500);
                    moeda.y = 30 + (rand() % 3000);
                    i = 0;
                }
        }
        moedas[j] = moeda;
    }
    return moedas;
}

void checkCoinColector(Rectangle *moedas, int *size, Rectangle player, int *qtd_moedas){
    for(int i = 0; i < (*size); i++){
        if(CheckCollisionRecs(moedas[i], player)){
            Rectangle aux = moedas[(*size)];
            moedas[(*size)] = moedas[i];
            moedas[i] = aux;
            *size -= 1;
            *qtd_moedas += 1;
            //moedas = (Rectangle *)realloc(moedas, sizeof(Rectangle) * (*size));
        }
    }
}

void checkZombieAttack(Rectangle *zumbis, int *size, Rectangle player){
    for(int i = 0; i < (*size); i++){
        if(CheckCollisionRecs(zumbis[i], player)){
            Rectangle aux = zumbis[(*size)];
            zumbis[(*size)] = zumbis[i];
            zumbis[i] = aux;
            *size -= 1;
            //moedas = (Rectangle *)realloc(moedas, sizeof(Rectangle) * (*size));
        }
    }
}


#define refy  playerHitBox.y - zumbis[i].zombiehitBox.y /// Referencias para o zombie
#define refx  playerHitBox.x - zumbis[i].zombiehitBox.x

#define dist 600

#define sizeback 3 // Volta do zombie
#define playervelocity 3 // Velocidade de player 
#define zombievelocity 1  // Velocidade do zombie

void movimento();

int main (void){
    
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "The Last of Cin");
    InitAudioDevice();

    int texzomb2 = 0;

    //if(!IsWindowFullscreen()) ToggleFullscreen();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // carrega os audios do game
    Music menumusic = LoadMusicStream("assets/Music/Come-Play-with-Me.mp3");
    Music mainmusic = LoadMusicStream("assets/Music/caves-of-dawn-10376.mp3");

    //carrega as texturas
    Texture2D background = LoadTexture("assets/Map/WorldMap.png");
    Texture2D portais = LoadTexture("assets/Map/portal.png");
    Texture2D menu = LoadTexture("assets/Menu/MENU.png");
    Texture2D menuinfo = LoadTexture("assets/Menu/MENUinfo.png");
    Texture2D menuintro = LoadTexture("assets/Menu/MENUintro.png");
    
    // Tecturas do player
    Texture2D playerback = LoadTexture("assets/Player/back.png");
    Texture2D playerfront = LoadTexture("assets/Player/front.png");
    Texture2D playerleft = LoadTexture("assets/Player/left.png");
    Texture2D playerright = LoadTexture("assets/Player/right.png");
    Texture2D playerstandright = LoadTexture("assets/Player/standright.png");
    Texture2D playerstandleft = LoadTexture("assets/Player/standleft.png");
    Texture2D playerstandfront = LoadTexture("assets/Player/standfront.png");
    Texture2D playerstandback = LoadTexture("assets/Player/standback.png");
    

   
    // Texturas do Zombie1 
    Texture2D zombieplayerback = LoadTexture("assets/Zombie/back.png");
    Texture2D zombieplayerfront = LoadTexture("assets/Zombie/front.png");
    Texture2D zombieplayerleft = LoadTexture("assets/Zombie/left.png");
    Texture2D zombieplayerright = LoadTexture("assets/Zombie/right.png");
    Texture2D zombieplayerexplo = LoadTexture("assets/Zombie/explosion.png");

    //textura dos icones
    Texture2D coin = LoadTexture("assets/icons/coin.png");
    Texture2D kokoro = LoadTexture("assets/icons/kokoro.png");

    // Divisão de sprites do player
    float playerspritesback = (float)(playerback.width/2);
    float zombiespritesexplode = (float)(zombieplayerexplo.width/8);

    int maxFrames1 = (int)(playerback.width /(int)playerspritesback);
    int maxFramesexplo = (int)zombieplayerexplo.width/(int)zombiespritesexplode;

    int texzomb1 = 4;   
    //int lifezomb;
    float timer = 0.0f;
    int frame = 0;

    //variaveis menu
    bool firstbutton = 0;
    bool button1 = 0;
    bool button2 = 0;
    bool button3 = 0;
    bool button4 = 0;
    bool flagmenu = 0;

    float timerexplo = 0.0f;
    int frameexplo = 0;
    
    //posição do player, pode mudar a constante definida se quiser coloca-lo em outro lugar no mapa 
    Rectangle playerHitBox = {START_POS_X, START_POS_Y, (float) playerstandfront.width/20, playerstandfront.height - 10};

     //Vector2 positionCoin= {.x = playerHitBox.x + 20, .y = playerHitBox.y };

    //NAO mexer Sem me perguntar (Matheus)
    Camera2D camera = {
        .offset = (Vector2){ (screenWidth / 2.0f), (screenHeight/ 2.0f)}, 
        .target = {0},
        .rotation = 0,
        .zoom = 2.0f
    };
    
    Rectangle paredes1[NUM_SQUARE] = { // Colisões do mapa
        {1345, 0, 345, 135},
        {1344, 134, 127, 185},
        {1471, 134, 227, 60},
        {1741, 0, 371, 80},
        {1741, 63, 84, 66},
        {1949, 63, 163, 109},
        {1792, 129, 116, 69},
        {2000, 172, 112, 116},
        {1885, 198, 20, 75},
        {2005, 324, 107, 1042},
        {1876, 565, 129, 801},
        {1346, 372, 107, 194}, 
        {1515, 334, 99, 135},
        {1521, 265, 33, 8},
        {1552, 230, 110, 58},
        {1662, 251, 83, 68},
        {1793, 273, 110, 69},
        {1741, 296, 52, 8},
        {1662, 400, 260, 64},
        {1339, 565, 243, 628},
        {1189, 1188, 509, 136},
        {1651, 593, 154, 494},
        {1457, 513, 301, 15},
        {1779, 513, 183, 15},
        {1775, 330, 17, 20},
        {1834, 342, 28, 11},
        {1903, 328, 64, 8},
        {1962, 324, 5, 200},
        {1649, 563, 15, 8},
        {1651, 571, 2, 22},
        {1662, 584, 130, 8},
        {1792, 563, 15, 8},
        {1803, 571, 2, 22},
        {1824, 1090, 52, 61},
        {1780, 1188, 97, 183},
        {2038, 1323, 292, 247}, 
        {1457, 1345, 192, 67},
        {1649, 1398, 73, 15},
        {1856, 1151, 15, 17},
        {1649, 1444, 151, 217},
        {1473, 1470, 184, 55},
        {1473, 1454, 65, 55},
        {1602, 1454, 65, 55},
        {1800, 1462, 184, 64},
        {1923, 1454, 61, 13},
        {1489, 1574, 160, 110},
        {1800, 1574, 151, 109},
        {1473, 1725, 143, 73}, 
        {1616, 1740, 61, 58},
        {1677, 1725, 134, 73},
        {1842, 1725, 143, 73},
        {2046, 1642, 272, 403}, 
        {1892, 1983, 241, 688}, 
        {1800, 1867, 246, 134}, 
        {1489, 1839, 209, 28},
        {1416, 1867, 282, 116},
        {1416, 1983, 149, 693},
        {1745, 2405, 241, 266}, 
        {1856, 2182, 85, 223}, 
        {1565, 2165, 85, 129}, 
        {1679, 2163, 96, 25}, 
        {1679, 2182, 184, 49},
        {1575, 2276, 242, 78},
        {2005, 2584, 241, 266},
        {1321, 2660, 149, 223}, 
        {1327, 2883, 143, 150},
        {1328, 3033, 370, 223}, 
        {1745, 3065, 370, 223},
        {1951, 2963, 143, 234},
        {1607, 2473, 85, 151},
        {1969, 2900, 143, 299},
        {1256, 1642, 160, 403},
        {902, 1733, 354, 319},
        {1135, 1326, 284, 242},
        {901, 1305, 269, 170},
        {54, 1249, 902, 56},
        {0, 1886, 902, 319},
        {838, 1535, 183, 62},
        {901, 1593, 120, 147},
        {705, 1567, 150, 70},
        {542, 1408, 272, 71},
        {431, 1373, 112, 105},
        {405, 1345, 198, 31},
        {62, 1359, 337, 120},
        {0, 1247, 62, 387},
        {0, 1691, 62, 319},
        {62,1691, 64, 74},
        {126, 1705, 129, 71},
        {255, 1697, 64, 53},
        {255, 1712, 128, 53},
        {148, 1408, 139, 120},
        {513, 1697, 347, 72},
        {369, 1543, 123, 115},
        {197, 1590, 38, 42},
        {319, 1764, 64, 82},
        {390, 1817, 243, 42},
        {194, 1824, 124, 6},
        {32, 1824, 124, 6},
        {492, 1562, 167, 75},
        {673, 1864, 13, 13},
        {689, 1864, 13, 13},
        {658, 1818, 156, 42},
        {369, 1657, 287, 13},
        {705, 1658, 155, 12},
        {833, 1670, 27, 27},
        {643, 1342, 217, 34},
        {838, 1371, 22, 102},
        {860, 1443, 42, 31},
        {1057, 1623, 62, 62},
        {1027, 1647, 30, 6},
        {1782, 1398, 119, 15},
        {1817, 1371, 32, 10},
        {1876, 1366, 119, 8},
        {1912, 1371, 32, 10},
        {1957, 1400, 86, 13},
        {1856, 1392, 15, 6},
        {1985, 1392, 15, 8},
        {1969, 1526, 15, 16},
        {1800, 1839, 209, 28},
        {1808, 1753, 35, 11},
        {1088, 1583, 47, 6},
        {1025, 1583, 30, 6},
        {1132, 1543, 6, 47},
        {1150, 1647, 124, 6},
        {1041, 1671, 13, 13},
        {1824, 2082, 64, 67},
        {1698, 2105, 140, 11},
        {1565, 2105, 113, 11},
        {1565, 1994, 129, 7},
        {1565, 2045, 50, 7},
        {1634, 2045, 61, 7},
        {1690, 2001, 3, 33},
        {1793, 1986, 7, 45},
        {1793, 2045, 45, 7},
        {1853, 2045, 61, 7},
        {1565, 2409, 78, 67},
        {1565, 2621, 129, 55},
        {1513, 2668, 34, 15},
        {1470, 2727, 95, 77},
        {1565, 2760, 95, 44},
        {1521, 2789, 79, 69},
        {1505, 2836, 16, 15},
        {1631, 2799, 64, 59},
        {1521, 2900, 110, 83},
        {1472, 2950, 48, 13},
        {1631, 2950, 15, 12},
        {1665, 2935, 60, 55},
        {1665, 2980, 240, 32},
        {1665, 3004, 33, 32},
        {1775, 2910, 67, 62},
        {1843, 2903, 41, 80},
        {1882, 2965, 22, 19},
        {1777, 2791, 65, 46},
        {1777, 2835, 241, 16},
        {1888, 2848, 16, 21},
        {2318, 1443, 1066, 36},
        {2051, 1575, 14, 13},
        {2318, 1751, 1322, 36},
        {2639, 1574, 240, 196},
        {2417, 1527, 12, 140},
        {2330, 1479, 54, 47},
        {2364, 1526, 20, 19},
        {2423, 1655, 108, 12},
        {2519, 1479, 12, 140},
        {3009, 1479, 95, 191},
        {3104, 1479, 99, 98},
        {3203, 1479, 71, 109},
        {3274, 1479, 127, 47},
        {3470, 1479, 170, 116},
        {3322, 1522, 80, 21},
        {3388, 1582, 80, 60},
        {3308, 1664, 160, 53},
        {3201, 1640, 73, 117},
        {3546, 1641, 80, 60},
        {3615, 1551, 37, 200},
        {3595, 1556, 20, 21},
        {3498, 1619, 20, 21},
        {3448, 1505, 20, 21},
        {3368, 1619, 20, 21},
        {1650, 1669, 14, 16},
        {1704, 1655, 33, 16},
        {1826, 1779, 12, 16},
        {1475, 1655, 13, 13},
        {1394, 1575, 13, 13},
        {863, 1846, 156, 42},
        {1871, 2163, 24, 25},
        {417, 1465, 13, 13},
        {289, 1513, 13, 13},
        {354, 1577, 13, 13},
        {530, 1545, 13, 13},
        {1871, 2163, 24, 25},
        {0, 2000, 1425, 1248},
        {2015, 0, 1600, 1468},
        {2100, 1790, 1600, 1450},
        {1545, 2483, 76, 160},
        {0, 0, 1345, 1320},
    };

    
    Rectangle *moedas = NULL;
    int size = NUM_COIN;
    moedas = loadCoin(size, moedas,  paredes1, coin);
    //char buf[500] = {0};
    char coins[500] = {0};
    int coins_obt = 0;


    Zombie * zumbis = NULL;
    int num_zombis = NUM_ZOMBIES;
    zumbis = loadZombies(num_zombis, zumbis, paredes1, zombieplayerback);

    /*Rectangle * zombiehitBox = NULL;
    zombiehitBox = (Rectangle *) malloc(sizeof(Rectangle) * num_zombis);
    for(int i = 0; i < num_zombis; i++){
        Rectangle aux = {zumbis[i].x, zumbis[i].y, (float)zombieplayerfront.width/40, zombieplayerfront.height};
        zombiehitBox[i] = aux; // Zombie 1
        //aux = {0};
    }*/



    int PlayerLife = NUM_LIFE;
    //char status[500] = {0};

    SetTargetFPS(60);

    bool a =0 , w=0,  s=0 , d=0 ; // Vars para saber qual foi o último antes da colisão
    int flag = 0, flag2 = 0;

    Rectangle frameRec = { 0.0f, 0.0f, (float)coin.width/7, (float)coin.height };
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 9; 

    int framesCounterexplode = 0;
    Rectangle framerecexplode = {0.0f, 0.0f, (float)zombieplayerexplo.width/8, (float)zombieplayerexplo.height };

    PlayMusicStream(menumusic);
    PlayMusicStream(mainmusic);

    while (!WindowShouldClose()) {

        UpdateMusicStream(menumusic); 

        //Animação da moeda
         framesCounter++;
        if (framesCounter >= (60/framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;
            if (currentFrame > 7) currentFrame = 0;
            frameRec.x = (float)currentFrame*((float)coin.width/7);
        }

        //Animação da explosão
        
        

        checkCoinColector(moedas, &size, playerHitBox, &coins_obt);

        // Player movement
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){ 
            playerHitBox.x += playervelocity;
            //zombiehitBox.x += 2;
            d = 1;a = 0;w = 0;s = 0;
            flag = 1; flag2 = 1;
        }else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)){ 
            playerHitBox.x -= playervelocity;
            //zombiehitBox.x -= 2;
            d = 0;a = 1;w = 0;s = 0;
            flag = 2;flag2 = 2;
        }else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
            playerHitBox.y += playervelocity;
            //zombiehitBox.y += 2;
            d = 0;a = 0;w = 0;s = 1;
            flag = 3;flag2 = 0;
        }else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
            playerHitBox.y -= playervelocity;
            //zombiehitBox.y -= 2;
            d = 0;a = 0;w = 1;s = 0;
            flag = 4; flag2 = 4;}
        else{
            d = 0;a = 0;w = 0;s = 0;
            flag = 0;}

        for (int xx =0; xx < NUM_SQUARE; xx++){ // Código que verifca colisão
            if(CheckCollisionRecs(playerHitBox, paredes1[xx])){
                    if (d){playerHitBox.x -= playervelocity;d=0;};
                    if (a){playerHitBox.x += playervelocity;a=0;};
                    if (s){playerHitBox.y -= playervelocity;s=0;};
                    if (w){playerHitBox.y += playervelocity;w=0;};
            }
        }
        

        for (int i = 0; i < num_zombis; i++){
            // Zobie Section
            int zombieflagx = 0; // Flag para direcionar os zombie
            int zombieflagy = 0;
            bool az = 0, sz =0, dz =0, wz=0;   // Vars lógicas  
                    
            
            if ((refx <= dist) && (refx >= -dist) && (refy <= dist) && (refy >= -dist)){
                
                if((refx >= -dist) && (refx < 0)){
                    zombieflagx = -1;
                }
                else if ((refx <= dist) && (refx > 0)){
                    zombieflagx = 1;
                }else{
                    zombieflagx = 0;
                }
                if((refy >= -dist) && (refy < 0)){
                    zombieflagy = -1;
                }
                else if ((refy <= dist) && (refy > 0)){
                    zombieflagy = 1;
                }else{
                    zombieflagy = 0;
                }
            }
            
            if(refx > refy){
                if (zombieflagx == -1){ // Diminui pelo eixo de x 
                    zumbis[i].zombiehitBox.x -= zombievelocity;
                    az = 1; sz =0; dz =0; wz=0;
                    texzomb1 = 1;
                    
                }
                else if (zombieflagx == 1){ // Aumenta pelo eixo de x e pelo o de y
                    zumbis[i].zombiehitBox.x += zombievelocity;
                    az = 0; sz =0; dz =1; wz=0;
                    texzomb1 = 2;
                }
                else if (zombieflagy == -1){ // Diminui pelo eixo de y
                    zumbis[i].zombiehitBox.y -= zombievelocity;
                    az = 0; sz =0; dz =0; wz=1;
                    texzomb1 = 3;
                }
                else if (zombieflagy == 1){ // Aumenta pelo eixo de y
                    zumbis[i].zombiehitBox.y += zombievelocity;
                    az = 0; sz =1; dz =0; wz=0;
                    texzomb1 = 4;
                }

                for (int xx =0; xx < NUM_SQUARE; xx++){ // Código que verifca colisão
                    if(CheckCollisionRecs(zumbis[i].zombiehitBox, paredes1[xx])){
                        if (dz){zumbis[i].zombiehitBox.x -= zombievelocity*sizeback;dz=0;};
                        if (az){zumbis[i].zombiehitBox.x += zombievelocity*sizeback;az=0;};
                        if (sz){zumbis[i].zombiehitBox.y -= zombievelocity*sizeback;sz=0;};
                        if (wz){zumbis[i].zombiehitBox.y += zombievelocity*sizeback;wz=0;};       
                    }
                }
            }

            else if (refy > refx){
                if (zombieflagy == -1){ // Diminui pelo eixo de y
                    zumbis[i].zombiehitBox.y -= zombievelocity;
                    az = 0; sz =0; dz =0; wz=1;
                    texzomb1 = 3;
                }
                else if (zombieflagy == 1){ // Aumenta pelo eixo de y 
                    zumbis[i].zombiehitBox.y += zombievelocity;
                    az = 0; sz =1; dz =0; wz=0;
                    texzomb1 = 4;
                }
                else if (zombieflagx == -1){ // Diminui pelo eixo de x 
                    zumbis[i].zombiehitBox.x -= zombievelocity;
                    az = 1; sz =0; dz =0; wz=0;
                    texzomb1 =1;
                }
                else if (zombieflagx == 1){ // Aumenta pelo eixo de x 
                    zumbis[i].zombiehitBox.x += zombievelocity;
                    az = 0; sz =0; dz =1; wz=0;
                    texzomb1 = 2;
                }
                for (int xx =0; xx < NUM_SQUARE; xx++){ // Código que verifca colisão
                    if(CheckCollisionRecs(zumbis[i].zombiehitBox, paredes1[xx])){
                        if (dz){zumbis[i].zombiehitBox.x -= zombievelocity*sizeback;dz=0;}
                        if (az){zumbis[i].zombiehitBox.x += zombievelocity*sizeback;az=0;}
                        if (sz){zumbis[i].zombiehitBox.y -= zombievelocity*sizeback;sz=0;}
                        if (wz){zumbis[i].zombiehitBox.y += zombievelocity*sizeback;wz=0;}
                    }
                }
            }
            /*framesCounterexplode++;
                if (framesCounterexplode >= (60/framesSpeed)){
                    framesCounter = 0;
                    currentFrame++;
                    if (currentFrame > 8) currentFrame = 0;
                    framerecexplode.x = (float)currentFrame*((float)zombieplayerexplo.width/8);
                }*/
            if(CheckCollisionRecs(zumbis[i].zombiehitBox, playerHitBox)){ // Colisão do player com o zombie
                framerecexplode.x = zumbis[i].zombiehitBox.x;
                framerecexplode.y = zumbis[i].zombiehitBox.y;
                texzomb1 = 5;
                texzomb2 = 5;
                zumbis[i].lifezombie = 0;
                PlayerLife--;
                Zombie aux = zumbis[num_zombis];
                zumbis[num_zombis] = zumbis[i];
                zumbis[i] =  aux;
                num_zombis--;
            }
            
            
        }
            // Camera target segue o player
            camera.target = (Vector2){ playerHitBox.x + 20, playerHitBox.y + 20 };

            timer += GetFrameTime();
            timerexplo += GetFrameTime();


            // Timer para animações  Zombie /  Player
            if (timer >= 0.2f){
                timer = 0.0f;
                frame += 1;
            }
            frame = frame % maxFrames1;
            for (int i = 0; i < num_zombis; i++){
                
                if (timerexplo > 0.1f){
                    timerexplo = 0.0f;
                    frameexplo += 1;   
                }
            
            frameexplo = frameexplo % maxFramesexplo;
            }   
            if(PlayerLife > 0){
            BeginDrawing();

            SetMusicVolume(menumusic, 0.6);

                //toca musica
                if(IsMusicStreamPlaying(menumusic) == 0 && flagmenu == 0){
                PlayMusicStream(menumusic);
                }

                // MENU 
                if (flagmenu == 0 && firstbutton == 0){
                    DrawTexture(menu, 0, 0, RAYWHITE);
                    DrawRectangleLines(556, 248, 160, 103, ORANGE);
                    DrawRectangleLines(602, 379, 114, 103, WHITE);
                    button1 = 1;
                }

                if (IsKeyPressed(KEY_DOWN) == 1){
                    button1 = 0;
                    button2 = 1;
                    firstbutton = 1;
                }

                if (IsKeyPressed(KEY_UP) == 1){
                    button2 = 0;
                    button1 = 1;
                }

                if (flagmenu == 0 && button2 == 1){
                    DrawTexture(menu, 0, 0, RAYWHITE);
                    DrawRectangleLines(556, 248, 160, 103, WHITE);
                    DrawRectangleLines(602, 379, 114, 103, ORANGE);
                }

                else if(flagmenu == 0 && button1 == 1){
                    DrawTexture(menu, 0, 0, RAYWHITE);
                    DrawRectangleLines(556, 248, 160, 103, ORANGE);
                    DrawRectangleLines(602, 379, 114, 103, WHITE);
                }

                //MENU intro 
                if (IsKeyPressed(KEY_SPACE) == 1 && button1 == 1){
                    button4 = 1;
                }

                if (flagmenu == 0 && button4 == 1){
                    DrawTexture(menuintro, 0, 0, RAYWHITE);
                    DrawRectangleLines(463, 463, 302, 86, ORANGE);
                }

                //Botao info
                if (IsKeyPressed(KEY_SPACE) == 1 && button2 == 1){
                    button3 = 1;
                    button2 = 0;
                }

                if (flagmenu == 0 && button3 == 1){
                    DrawTexture(menuinfo, 0, 0, RAYWHITE);
                    DrawRectangleLines(554, 464, 213, 91, ORANGE);
                }

                if (IsKeyPressed(KEY_F) == 1 && button3 == 1){
                    button3 = 0;
                    button2 = 1;
                }
                
                ClearBackground(BLACK); // coloca o background na cor BLACK
                BeginMode2D(camera);// inicia o modo 2d usando a camera setada
                    
                    //realizar todos os desenhos aqui dentro 
                    if((IsKeyPressed(KEY_F) == 1 && button4 == 1)|| flagmenu == 1){

                    flagmenu = 1;
                    // Troca de musica
                    StopMusicStream(menumusic);
                    SetMusicVolume(mainmusic, 0.4);

                    UpdateMusicStream(menumusic); 
                    UpdateMusicStream(mainmusic); 

                    if(IsMusicStreamPlaying(mainmusic) == 0){
                    PlayMusicStream(mainmusic);
                    UpdateMusicStream(mainmusic); 
                    }

                    DrawTexture(background, 0, 0, RAYWHITE);  // desenha a Textura MAP em cima do Background
                    //DrawRectangle(zombiehitBox.x, zombiehitBox.y, 10, 10, GREEN);
                    //DrawRectangle(playerHitBox.x, playerHitBox.y, 10, 10, BLUE); // desenha
                    //for(int x = 0; x < 192; x++){DrawRectangleRec(paredes1[x], RED);} // Pintar colisões no mapa
                    
                    //Textures Player 
                    if (flag == 1){
                        DrawTextureRec(
                            playerright, 
                            (Rectangle){((playerright.width)/2 * frame),
                            0,
                            playerright.width/2, playerright.height}, 
                            (Vector2){playerHitBox.x,playerHitBox.y},
                            RAYWHITE);}
                    else if (flag == 2){
                        DrawTextureRec(
                            playerleft, 
                            (Rectangle){((playerleft.width)/2 * frame),
                            0,
                            playerleft.width/2, playerleft.height}, 
                            (Vector2){playerHitBox.x,playerHitBox.y},
                            RAYWHITE);}
                    else if (flag == 3){
                        DrawTextureRec(
                            playerfront, 
                            (Rectangle){((playerfront.width)/2 * frame),
                            0,playerfront.width/2, playerfront.height}, 
                            (Vector2){playerHitBox.x,playerHitBox.y},
                            RAYWHITE);}
                    else if (flag == 4){
                        DrawTextureRec(
                            playerback, 
                            (Rectangle){((playerback.width)/2 * frame),
                            0,
                            playerback.width/2, playerback.height}, 
                            (Vector2){playerHitBox.x,playerHitBox.y},
                            RAYWHITE);}
                    else{   
                        if(flag2 == 0){
                            DrawTextureRec(
                                playerstandfront, 
                                (Rectangle){((playerstandfront.width) * frame),
                                0,
                                playerstandfront.width, playerstandfront.height}, 
                                (Vector2){playerHitBox.x,playerHitBox.y},
                                RAYWHITE);}
                        else if (flag2 == 1) {
                            DrawTextureRec(
                                playerstandleft, 
                                (Rectangle){((playerstandleft.width) * frame),
                                0,
                                playerstandleft.width, 
                                playerstandleft.height}, 
                                (Vector2){playerHitBox.x,playerHitBox.y},
                                RAYWHITE);}
                        else if (flag2 == 2) {
                            DrawTextureRec(playerstandright, 
                            (Rectangle){((playerstandright.width) * frame),
                            0,
                            playerstandright.width, 
                            playerstandright.height}, 
                            (Vector2){playerHitBox.x,playerHitBox.y},
                            RAYWHITE);}
                        else if (flag2 == 4){
                            DrawTextureRec(
                                playerstandback,
                                (Rectangle){((playerstandback.width) * frame),
                                0,
                                playerstandback.width, 
                                playerstandback.height}, 
                                (Vector2){playerHitBox.x,playerHitBox.y},
                                RAYWHITE);}}

                    for (int i = 0; i < num_zombis+1; i++){
                        // Zombies textures
                        if (texzomb1 == 1){
                            DrawTextureRec(
                                zombieplayerleft, 
                                (Rectangle){((zombieplayerleft.width)/2 * frame),
                                0,
                                zombieplayerleft.width/2, zombieplayerleft.height}, 
                                (Vector2){zumbis[i].zombiehitBox.x,zumbis[i].zombiehitBox.y},
                                RAYWHITE);
                        }
                        else if(texzomb1 == 2){
                            DrawTextureRec(
                                zombieplayerright, 
                                (Rectangle){((zombieplayerright.width)/2 * frame),
                                0,
                                zombieplayerright.width/2, zombieplayerfront.height}, 
                                (Vector2){zumbis[i].zombiehitBox.x,zumbis[i].zombiehitBox.y},
                                RAYWHITE);
                        }
                        else if(texzomb1 == 3){
                            DrawTextureRec(
                                zombieplayerfront, 
                                (Rectangle){((zombieplayerfront.width)/2 * frame),
                                0,
                                zombieplayerfront.width/2, zombieplayerfront.height}, 
                                (Vector2){zumbis[i].zombiehitBox.x,zumbis[i].zombiehitBox.y},
                                RAYWHITE);
                        }
                        else if(texzomb1 == 4){
                            DrawTextureRec(
                                zombieplayerback, 
                                (Rectangle){((zombieplayerback.width)/2 * frame),
                                0,
                                zombieplayerback.width/2, zombieplayerback.height}, 
                                (Vector2){zumbis[i].zombiehitBox.x,zumbis[i].zombiehitBox.y},
                                RAYWHITE);
                        }
                        
                        if ((texzomb2 == 5)){
                            DrawTextureRec(
                                zombieplayerexplo, 
                                (Rectangle){((zombieplayerexplo.width)/8 * frameexplo),
                                0,
                                zombieplayerexplo.width/8, zombieplayerexplo.height}, 
                                (Vector2){framerecexplode.x,framerecexplode.y},
                                RAYWHITE);
                        }
                         /*else if(zumbis[i].lifezombie == 0){
                            DrawTextureRec(
                                zombieplayerexplo, 
                                (Rectangle){((zombieplayerexplo.width)/8 * frameexplo),
                                0,
                                zombieplayerexplo.width/8, zombieplayerexplo.height}, 
                                (Vector2){0,0},
                                RAYWHITE);
                        }*/
                    }
                    /*
                    for(int i = 0; i < NUM_SQUARE; i++){
                        DrawRectangleRec(paredes1[i], BLUE);
                    }*/

                    //desenha moedas 
                    for(int i = 0; i <= size; i++){
                        DrawTextureRec(coin, frameRec, (Vector2){.x = moedas[i].x, .y = moedas[i].y}, WHITE);
                    // DrawTexture(coin, moedas[i].x, moedas[i].y, RAYWHITE);
                    
                    DrawTexture(portais, 0, 0, RAYWHITE); // Desenha Portais
                    
                    }
                    
                    DrawTextureRec(coin, (Rectangle){ 60.0f, 0.0f, (float)coin.width/7, (float)coin.height },
                                (Vector2){.x =  playerHitBox.x - (pow(camera.zoom, 2) * 45),
                                .y = playerHitBox.y - (camera.zoom * 60) + 23}, RAYWHITE);

                    //desenha coins do personagem 
                    sprintf(coins, "x %d", coins_obt);
                    DrawText(coins, playerHitBox.x - 160, 
                                    playerHitBox.y - 100, 20, YELLOW);
                   /* 
                    //Apagar depois 
                    sprintf(buf, "pos x = %.2f pos y = %.2f", playerHitBox.x, playerHitBox.y);
                    DrawText(buf, playerHitBox.x - 100, playerHitBox.y - 100, 20, DARKGRAY);
                
                
                    */
                    //desenha status do personagem 
                    for(int i = 0; i < PlayerLife; i++){
                    DrawTexture(kokoro, playerHitBox.x - (pow(camera.zoom, 2) * 45)  + (20 * i),
                                      playerHitBox.y - (camera.zoom * 60), RAYWHITE);
                    }
                    

                    }
                
                EndMode2D();
                    
            EndDrawing();
        }else {
            BeginDrawing();
                ClearBackground(BLACK);
                DrawText("GAME OVER", (screenWidth / 2) - 250, screenHeight / 2, 80, RED);
            EndDrawing();
        }

    }


    // UnloadMusic
    UnloadMusicStream(menumusic); 
    UnloadMusicStream(mainmusic);
    // UnloadTexture (background)
    UnloadTexture(background);
    UnloadTexture(menu);
    UnloadTexture(menuintro);
    UnloadTexture(menuinfo);
    // UnloadTexture (player)
    UnloadTexture(playerback);
    UnloadTexture(playerfront);
    UnloadTexture(playerleft);
    UnloadTexture(playerright);
    UnloadTexture(playerstandright);
    UnloadTexture(playerstandleft);
    UnloadTexture(playerstandfront);
    UnloadTexture(playerstandback);
    //UnloadTexture(Zombie)
    UnloadTexture(zombieplayerexplo);
    UnloadTexture(zombieplayerback);
    UnloadTexture(zombieplayerfront);
    UnloadTexture(zombieplayerleft);
    UnloadTexture(zombieplayerright);

    UnloadTexture(coin);
    CloseWindow();
    CloseAudioDevice();

    return 0;
}
