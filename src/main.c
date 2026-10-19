#include <genesis.h>
#include "resources.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224

#define SHIP_WIDTH 24
#define SHIP_HEIGHT 24

#define BULLET_WIDTH 8
#define BULLET_HEIGHT 8

#define SHIP_SPEED 2
#define BULLET_SPEED 5

#define MAX_BULLETS 8
#define FIRE_DELAY 8

typedef struct
{
    Sprite *sprite;
    s16 x;
    s16 y;
    bool active;
} Bullet;

int main(bool hardReset)
{
    s16 shipX = (SCREEN_WIDTH - SHIP_WIDTH) / 2;
    s16 shipY = (SCREEN_HEIGHT - SHIP_HEIGHT) / 2;

    u16 joy;
    u16 previousJoy = 0;
    u16 fireTimer = 0;

    Bullet bullets[MAX_BULLETS];

    SPR_init();

    // Carrega a paleta da nave.
    PAL_setPalette(PAL1, ship.palette->data, DMA);

    // Cria a nave.
    Sprite *playerShip = SPR_addSprite(
        &ship,
        shipX,
        shipY,
        TILE_ATTR(PAL1, TRUE, FALSE, FALSE)
    );

    if (playerShip == NULL)
    {
        VDP_drawText("ERRO AO CRIAR A NAVE", 8, 10);

        while (TRUE)
        {
            SYS_doVBlankProcess();
        }
    }

    // Inicializa o conjunto fixo de projéteis.
    for (u16 i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].sprite = NULL;
        bullets[i].x = 0;
        bullets[i].y = 0;
        bullets[i].active = FALSE;
    }

    while (TRUE)
    {
        joy = JOY_readJoypad(JOY_1);

        // Movimentação da nave.
        if (joy & BUTTON_LEFT)
            shipX -= SHIP_SPEED;

        if (joy & BUTTON_RIGHT)
            shipX += SHIP_SPEED;

        if (joy & BUTTON_UP)
            shipY -= SHIP_SPEED;

        if (joy & BUTTON_DOWN)
            shipY += SHIP_SPEED;

        // Limites da tela.
        if (shipX < 0)
            shipX = 0;

        if (shipY < 0)
            shipY = 0;

        if (shipX > SCREEN_WIDTH - SHIP_WIDTH)
            shipX = SCREEN_WIDTH - SHIP_WIDTH;

        if (shipY > SCREEN_HEIGHT - SHIP_HEIGHT)
            shipY = SCREEN_HEIGHT - SHIP_HEIGHT;

        SPR_setPosition(playerShip, shipX, shipY);

        // Intervalo entre disparos.
        if (fireTimer > 0)
            fireTimer--;

        // Dispara enquanto o botão B estiver pressionado.
        if ((joy & BUTTON_B) && (fireTimer == 0))
        {
            for (u16 i = 0; i < MAX_BULLETS; i++)
            {
                if (!bullets[i].active)
                {
                    // Cria o sprite do projétil na primeira utilização.
                    if (bullets[i].sprite == NULL)
                    {
                        bullets[i].sprite = SPR_addSprite(
                            &bullet,
                            0,
                            0,
                            TILE_ATTR(PAL1, TRUE, FALSE, FALSE)
                        );
                    }

                    if (bullets[i].sprite != NULL)
                    {
                        bullets[i].x = shipX + (SHIP_WIDTH - BULLET_WIDTH) / 2;
                        bullets[i].y = shipY - BULLET_HEIGHT;
                        bullets[i].active = TRUE;

                        SPR_setVisibility(
                            bullets[i].sprite,
                            VISIBLE
                        );

                        SPR_setPosition(
                            bullets[i].sprite,
                            bullets[i].x,
                            bullets[i].y
                        );

                        fireTimer = FIRE_DELAY;
                    }

                    break;
                }
            }
        }

        // Atualiza os projéteis ativos.
        for (u16 i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].active)
            {
                bullets[i].y -= BULLET_SPEED;

                if (bullets[i].y + BULLET_HEIGHT < 0)
                {
                    bullets[i].active = FALSE;

                    SPR_setVisibility(
                        bullets[i].sprite,
                        HIDDEN
                    );
                }
                else
                {
                    SPR_setPosition(
                        bullets[i].sprite,
                        bullets[i].x,
                        bullets[i].y
                    );
                }
            }
        }

        previousJoy = joy;

        SPR_update();
        SYS_doVBlankProcess();
    }

    return 0;
}