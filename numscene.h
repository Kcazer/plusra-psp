#pragma once

#include <oslib/oslib.h>

#define EFFECT_NONE   0
#define EFFECT_RED    1
#define EFFECT_WHITE  2
#define GAME_MENU     0
#define GAME_PAUSED   1
#define GAME_PLAYING  2
#define GAME_OVER     3
#define GAME_HSCORES  4
#define GAME_CREDITS  5

class NumScene {
public:
    NumScene(void);

    ~NumScene(void);

    void update(void);

    bool loaded(void);

private:
    // Chargement des données
    bool loadData(void);

    bool loadImage(char *file, OSL_IMAGE **img, int x, int y, int centerX, int centerY, int frameX, int frameY);

    bool loadSound(char *file, OSL_SOUND **snd, bool streamed, bool repeat);

    // Fonctions d'update
    void updateMenu(void);

    void updateGame(void);

    void updatePause(void);

    void updateGameOver(void);

    void updateHighScores(void);

    void updateCredits(void);

    // Fonctions de dessin
    void drawMenu(void);

    void drawGame(void);

    void drawPause(void);

    void drawGameOver(void);

    void drawHighScores(void);

    void drawCredits(void);

    // Affichage de textes
    void printText(int x, int y, char *text);

    void printTextMini(int x, int y, char *text);

    void printRawText(int x, int y, char *text, int size);

    void printNum(int x, int y, int num, int pad, bool hideZero);

    // Autres fonctions utiles
    void startGame(void);

    void addGoal(void);

    void addBlock(void);

    void loadScores(void);

    void saveScores(void);

    // Attributs
    bool _loaded;
    int _timer;
    int _goal[3];
    int _speed;
    int _score;
    int _hp;
    int _combo;
    int _maxCombo;
    int _status;
    int _oldStatus;
    int _menuItem;
    int _currentChar;
    char _playerName[8];

    struct {
        char name[8];
        int combo;
        int score;
    } _highScores[10];
    struct {
        int mode;
        int value;
        int position;
        int effectDisplay;
        int effectTimer;
    } _map[8][12];
    struct {
        int color;
        int y;
        int x;
        int angle;
    } _arrows[20];
    struct {
        int x;
        int y;
    } _cursor;
    struct {
        OSL_IMAGE *arrow;
        OSL_IMAGE *background;
        OSL_IMAGE *block;
        OSL_IMAGE *boomRed;
        OSL_IMAGE *boomWhite;
        OSL_IMAGE *combo;
        OSL_IMAGE *cursor;
        OSL_IMAGE *font;
        OSL_IMAGE *fontMini;
        OSL_IMAGE *goal32;
        OSL_IMAGE *goal40;
        OSL_IMAGE *menu;
        OSL_IMAGE *name;
        OSL_IMAGE *playArea;
        OSL_IMAGE *pressKey;
        OSL_IMAGE *result;
        OSL_IMAGE *score;
        OSL_IMAGE *slide;
        OSL_IMAGE *title;
    } _img;
    struct {
        OSL_SOUND *menu;
        OSL_SOUND *game;
    } _bgm;
    struct {
        OSL_SOUND *move;
        OSL_SOUND *success;
        OSL_SOUND *fail;
    } _se;
};
