#include <genesis.h>

int main(bool hardReset)
{
    // Limpa a tela de texto.
    VDP_drawText("STAR RUNNER", 14, 10);

    VDP_drawText("Bora fazer um jogo Bruninhoooo", 5, 13);

    VDP_drawText("PRIMEIRO PROJETO SGDK", 9, 16);

    VDP_drawText("PRESSIONE START", 12, 22);

    while (TRUE)
    {
        // Mantem o jogo em execucao.
        SYS_doVBlankProcess();
    }

    return 0;
}
