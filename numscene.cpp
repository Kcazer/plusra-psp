#include "numscene.h"

NumScene::NumScene(void) {
    int i;
    // Chargement des donnees
    _loaded = loadData();
    loadScores();
    // Repetitions des touches
    oslSetKeyAutorepeat(OSL_KEYMASK_UP | OSL_KEYMASK_RIGHT | OSL_KEYMASK_DOWN | OSL_KEYMASK_LEFT, 15, 5);
    // Initialisation
    for (i = 0; i < 20; i++) _arrows[i].x = 500;
    for (i = 0; i < 8; i++) _playerName[i] = 42;
    oslPlaySound(_bgm.menu, 0);
    _status = GAME_MENU;
    _menuItem = 0;
}

NumScene::~NumScene(void) {
    // Free Image
    oslDeleteImage(_img.arrow);
    oslDeleteImage(_img.background);
    oslDeleteImage(_img.block);
    oslDeleteImage(_img.boomRed);
    oslDeleteImage(_img.boomWhite);
    oslDeleteImage(_img.combo);
    oslDeleteImage(_img.cursor);
    oslDeleteImage(_img.font);
    oslDeleteImage(_img.fontMini);
    oslDeleteImage(_img.goal32);
    oslDeleteImage(_img.goal40);
    oslDeleteImage(_img.menu);
    oslDeleteImage(_img.name);
    oslDeleteImage(_img.playArea);
    oslDeleteImage(_img.pressKey);
    oslDeleteImage(_img.score);
    oslDeleteImage(_img.slide);
    oslDeleteImage(_img.title);
    // Free Audio
    oslDeleteSound(_bgm.menu);
    oslDeleteSound(_bgm.game);
    oslDeleteSound(_se.move);
    oslDeleteSound(_se.success);
    oslDeleteSound(_se.fail);
}

bool NumScene::loaded(void) {
    return _loaded;
}

bool NumScene::loadData(void) {
    bool success = true;
    success &= loadImage("data/imgArrow.png", &(_img.arrow), 0, 0, 16, 16, 32, 32);
    success &= loadImage("data/imgBackground.png", &(_img.background), -32, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgBlock.png", &(_img.block), 0, 0, 0, 0, 32, 32);
    success &= loadImage("data/imgBoomRed.png", &(_img.boomRed), 0, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgBoomWhite.png", &(_img.boomWhite), 0, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgCombo.png", &(_img.combo), 0, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgCursor.png", &(_img.cursor), 0, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgFont.png", &(_img.font), 0, 0, 0, 0, 12, 12);
    success &= loadImage("data/imgFontMini.png", &(_img.fontMini), 0, 0, 0, 0, 6, 6);
    success &= loadImage("data/imgGoal32.png", &(_img.goal32), 38, 0, 0, 0, 32, 32);
    success &= loadImage("data/imgGoal40.png", &(_img.goal40), 20, 155, 0, 0, 40, 40);
    success &= loadImage("data/imgMenu.png", &(_img.menu), 0, 0, 0, 0, 204, 22);
    success &= loadImage("data/imgName.png", &(_img.name), 0, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgPlayArea.png", &(_img.playArea), 0, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgPressKey.png", &(_img.pressKey), 146, 242, 0, 0, 0, 0);
    success &= loadImage("data/imgScore.png", &(_img.score), 0, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgSlide.png", &(_img.slide), -64, 0, 0, 0, 0, 0);
    success &= loadImage("data/imgTitle.png", &(_img.title), 112, 16, 0, 0, 0, 0);
    success &= loadSound("data/bgmGame.bgm", &(_bgm.game), true, true);
    success &= loadSound("data/bgmMenu.bgm", &(_bgm.menu), true, true);
    success &= loadSound("data/seFail.wav", &(_se.fail), false, false);
    success &= loadSound("data/seMove.wav", &(_se.move), false, false);
    success &= loadSound("data/seSuccess.wav", &(_se.success), false, false);
    return success;
}

bool NumScene::loadImage(char *file, OSL_IMAGE **img, int x, int y, int centerX, int centerY, int frameX, int frameY) {
    (*img) = oslLoadImageFile(file, OSL_IN_RAM, OSL_PF_8888);
    if (!(*img)) return false;
    (*img)->x = x;
    (*img)->y = y;
    (*img)->centerX = centerX;
    (*img)->centerY = centerY;
    if (frameX && frameY) oslSetImageFrameSize((*img), frameX, frameY);
    return true;
}

bool NumScene::loadSound(char *file, OSL_SOUND **snd, bool streamed, bool repeat) {
    (*snd) = oslLoadSoundFile(file, (streamed) ? OSL_FMT_STREAM : OSL_FMT_NONE);
    if (!(*snd)) return false;
    oslSetSoundLoop((*snd), repeat);
    return true;
}


void NumScene::update(void) {
    switch (_status) {
        case GAME_MENU:
            updateMenu();
            drawMenu();
            break;
        case GAME_PLAYING:
            updateGame();
            drawGame();
            break;
        case GAME_PAUSED:
            updatePause();
            drawPause();
            break;
        case GAME_OVER:
            updateGameOver();
            drawGameOver();
            break;
        case GAME_HSCORES:
            updateHighScores();
            drawHighScores();
            break;
        case GAME_CREDITS:
            updateCredits();
            drawCredits();
            break;
        default:
            // Will never happen
            break;
    }
}


void NumScene::updateMenu(void) {
    // Deplacement dans le menu
    if (osl_pad.pressed.up) {
        _menuItem = (_menuItem + 2) % 3;
        oslPlaySound(_se.move, 1);
    } else if (osl_pad.pressed.down) {
        _menuItem = (_menuItem + 1) % 3;
        oslPlaySound(_se.move, 1);
    }
    if (osl_pad.pressed.circle) {
        switch (_menuItem) {
            case 0:
                startGame();
                break;
            case 1:
                _oldStatus = GAME_MENU;
                _status = GAME_HSCORES;
                break;
            case 2:
                _status = GAME_CREDITS;
                break;
            default:
                break;
        }
    }
}

void NumScene::drawMenu(void) {
    int i;
    // Deplacement des fleches
    for (i = 0; i < 20; i++)
        if (_arrows[i].x > 500) {
            _arrows[i].color = rand() % 4;
            _arrows[i].y = rand() % 240 + 16;
            _arrows[i].x = rand() % 500 - 500;
            _arrows[i].angle = rand() % 360;
        } else {
            _arrows[i].x += 5;
            _arrows[i].angle += 4;
        }
    // Affichage
    oslDrawFillRect(0, 0, 480, 272, 0xFFFFFFFF);
    for (i = 0; i < 20; i++) {
        _img.arrow->x = _arrows[i].x;
        _img.arrow->y = _arrows[i].y;
        _img.arrow->angle = _arrows[i].angle;
        oslSetImageFrame(_img.arrow, _arrows[i].color);
        oslDrawImage(_img.arrow);
    }
    oslDrawImage(_img.title);

    oslSetImageFrame(_img.menu, _menuItem);
    _img.menu->x = 111;
    _img.menu->y = 135 + 48 * _menuItem;
    oslDrawImage(_img.menu);

    for (i = 0; i < 3; i++) {
        oslSetImageFrame(_img.menu, i);
        _img.menu->x = 48 + 64 * ((i == _menuItem) ? 1 : 0);
        _img.menu->y = 136 + 48 * i;
        oslDrawImage(_img.menu);
    }

}


void NumScene::updateGame(void) {
    int x, y, oldX, oldY;
    bool loseHP, fusion = false;
    /*// Pause ?
    if (osl_pad.pressed.start) {
        _status = GAME_PAUSED;
        return;
    }
    */
    // Deplacement des blocs libres
    for (y = 0; y < 8; y++)
        for (x = 0; x < 12; x++) {
            // Pas de bloc
            if (_map[y][x].value == -1) continue;
            // Bloc en position finale
            if (_map[y][x].position == 0) {
                // Ne peut pas bouger
                if (x == 0 || _map[y][x - 1].value != -1) continue;
                // Se transfere au bloc inferieur
                _map[y][x - 1].value = _map[y][x].value;
                _map[y][x].value = -1;
                _map[y][x - 1].position = 32 - _speed;;
            } else _map[y][x].position -= _speed;;
        }
    // Gestion du respawn
    _timer--;
    if (osl_pad.pressed.square || _timer == 0) {
        if (osl_pad.pressed.square) _hp -= 2;
        _timer = 120;
        addBlock();
    }
    // Déplacement du curseur
    oldX = _cursor.x;
    oldY = _cursor.y;
    if (osl_pad.pressed.up) _cursor.y = (_cursor.y + 7) % 8;
    else if (osl_pad.pressed.down) _cursor.y = (_cursor.y + 1) % 8;
    if (osl_pad.pressed.left) _cursor.x = (_cursor.x + 11) % 12;
    else if (osl_pad.pressed.right) _cursor.x = (_cursor.x + 1) % 12;
    // Son de deplacement
    if (oldX != _cursor.x || oldY != _cursor.y) oslPlaySound(_se.move, 1);
    // Test fusion de blocs
    if (osl_pad.held.circle && _map[oldY][oldX].value != -1                    // Cercle enfonc� et ancien bloc non vide
        && (oslAbs(_cursor.x - oldX) + oslAbs(_cursor.y - oldY)) == 1) {    // Et pas en diagonale
        // Deplacement sur un bloc vide ou occup� ?
        if (_map[_cursor.y][_cursor.x].value == -1) {
            _map[_cursor.y][_cursor.x].value = _map[oldY][oldX].value;
            _combo = 1;
        } else {
            _map[_cursor.y][_cursor.x].value += _map[oldY][oldX].value;
            fusion = true;
        }
        _map[oldY][oldX].value = -1;
    }
    // Si il y a eu fusion
    if (fusion) {
        if (_map[_cursor.y][_cursor.x].value == _goal[0]) {
            // Fusion reussie
            oslPlaySound(_se.success, 2);
            _score += _goal[0] * _combo;
            _combo += 1;
            if (_combo > _maxCombo) _maxCombo = _combo;
            _map[_cursor.y][_cursor.x].value = -1;
            _map[_cursor.y][_cursor.x].effectDisplay = EFFECT_WHITE;
            _map[_cursor.y][_cursor.x].effectTimer = 30;
            addGoal();
        } else if (_map[_cursor.y][_cursor.x].value > 9) {
            oslPlaySound(_se.fail, 2);
            _hp -= _map[_cursor.y][_cursor.x].value;
            _combo = 1;
            _map[_cursor.y][_cursor.x].value -= 10;
            _map[_cursor.y][_cursor.x].effectDisplay = EFFECT_RED;
            _map[_cursor.y][_cursor.x].effectTimer = 30;
        }
    }
    // Cas d'une ligne remplie
    for (y = 0; y < 8; y++) {
        loseHP = true;
        for (x = 0; x < 12; x++) loseHP &= (_map[y][x].value != -1);
        if (loseHP) {
            _hp--;
            break;
        }
    }
    // Hp restant
    if (_hp <= 0) {
        _hp = 0;
        _maxCombo--;
        _currentChar = 0;
        _status = GAME_OVER;
    }
}

void NumScene::drawGame(void) {
    int x, y;
    int explosionSize;
    OSL_IMAGE *boom;
    // Déplacement du background
    _img.background->x++;
    if (_img.background->x == 0) _img.background->x = -32;
    _img.slide->x += 2;
    if (_img.slide->x == 480) _img.slide->x = -128;
    // Gestion des explosions
    for (y = 0; y < 8; y++)
        for (x = 0; x < 12; x++) {
            if (_map[y][x].effectDisplay == EFFECT_NONE) continue;
            _map[y][x].effectTimer -= 5;
            if (_map[y][x].effectTimer < 0) _map[y][x].effectDisplay = EFFECT_NONE;
        }
    // Background
    oslDrawImage(_img.background);
    oslDrawImage(_img.slide);
    oslDrawImage(_img.playArea);
    // Dessine les blocs
    for (y = 0; y < 8; y++)
        for (x = 0; x < 12; x++) {
            if (_map[y][x].value == -1) continue;
            _img.block->x = (x << 5) + 88 + _map[y][x].position;
            _img.block->y = (y << 5) + 8;
            oslSetImageFrame(_img.block, _map[y][x].value);
            oslDrawImage(_img.block);
        }
    // Dessine les explosions
    for (y = 0; y < 8; y++)
        for (x = 0; x < 12; x++) {
            if (_map[y][x].effectDisplay == EFFECT_NONE) continue;
            if (_map[y][x].effectDisplay == EFFECT_RED) boom = _img.boomRed;
            else boom = _img.boomWhite;
            explosionSize = 60 + _map[y][x].effectTimer;
            boom->stretchX = explosionSize;
            boom->stretchY = explosionSize;
            boom->x = (x << 5) + 104 - (explosionSize >> 1);
            boom->y = (y << 5) + 24 - (explosionSize >> 1);
            oslDrawImage(boom);
        }
    // Le curseur
    _img.cursor->x = (_cursor.x << 5) + 88;
    _img.cursor->y = (_cursor.y << 5) + 8;
    oslDrawImage(_img.cursor);
    // Score et HP
    printNum(8, 26, _score, 6, false);
    printNum(8, 90, _hp, 3, true);
    // Blocs suivants
    oslSetImageFrame(_img.goal40, _goal[0]);
    oslDrawImage(_img.goal40);
    _img.goal32->y = 196;
    oslSetImageFrame(_img.goal32, _goal[1]);
    oslDrawImage(_img.goal32);
    _img.goal32->y = 229;
    oslSetImageFrame(_img.goal32, _goal[2]);
    oslDrawImage(_img.goal32);
}


void NumScene::updatePause(void) {
    // Gestion des touches
    if (osl_pad.pressed.start) _status = GAME_PLAYING;
}

void NumScene::drawPause(void) {
    drawGame();
    oslDrawFillRect(0, 0, 480, 272, 0xCCFFFFFF);
    printText(202, 110, "Paused");
    printText(110, 144, "Press Start to resume");
}


void NumScene::updateHighScores(void) {
    // Gestion des touches
    if (osl_pad.pressed.circle) {
        _status = GAME_MENU;
        if (_oldStatus == GAME_OVER) {
            oslStopSound(_bgm.game);
            oslPlaySound(_bgm.menu, 0);
        }
    }
}

void NumScene::drawHighScores(void) {
    int i;
    if (_oldStatus == GAME_OVER) drawGame();
    else if (_oldStatus == GAME_MENU) drawMenu();
    else oslDrawFillRect(0, 0, 480, 272, 0xFFFFFFFF);    // Will never happen
    oslDrawFillRect(0, 0, 480, 272, 0xCCFFFFFF);
    oslDrawImageXY(_img.name, 92, 20);
    oslDrawImageXY(_img.combo, 228, 20);
    oslDrawImageXY(_img.score, 342, 20);
    oslDrawImage(_img.pressKey);
    for (i = 0; i < 10; i++) {
        printRawText(64, 46 + 18 * i, _highScores[i].name, 8);
        printNum(230, 46 + 18 * i, _highScores[i].combo, 3, true);
        printNum(330, 46 + 18 * i, _highScores[i].score, 6, false);
    }
}


void NumScene::updateGameOver(void) {
    int i, j, k;
    char c;
    if (osl_pad.pressed.circle) {
        // En cas de nouveau highscore
        if (_score > _highScores[9].score) {
            // On copie les infos en derniere position
            for (i = 0; i < 8; i++) _highScores[9].name[i] = _playerName[i];
            _highScores[9].combo = _maxCombo;
            _highScores[9].score = _score;
            // Et on remonte le score tant qu'il est superieur
            i = 9;
            while (1) {
                if (i == 0) break;
                else if (_highScores[i].score < _highScores[i - 1].score) break;
                else if (_highScores[i].score == _highScores[i - 1].score &&
                         _highScores[i].combo < _highScores[i - 1].combo)
                    break;
                // Inversion des noms
                for (j = 0; j < 8; j++) {
                    c = _highScores[i].name[j];
                    _highScores[i].name[j] = _highScores[i - 1].name[j];
                    _highScores[i - 1].name[j] = c;
                }
                // Inversion des scores / combo
                k = _highScores[i].combo;
                _highScores[i].combo = _highScores[i - 1].combo;
                _highScores[i - 1].combo = k;
                k = _highScores[i].score;
                _highScores[i].score = _highScores[i - 1].score;
                _highScores[i - 1].score = k;
                i--;
            }
            saveScores();
        }
        _oldStatus = GAME_OVER;
        _status = GAME_HSCORES;
    }
    if (_score > _highScores[9].score) {
        if (osl_pad.pressed.left) _currentChar = (_currentChar + 7) % 8;
        else if (osl_pad.pressed.right) _currentChar = (_currentChar + 1) % 8;
        if (osl_pad.pressed.up) _playerName[_currentChar] = (_playerName[_currentChar] + 1) % 43;
        else if (osl_pad.pressed.down) _playerName[_currentChar] = (_playerName[_currentChar] + 42) % 43;
    }
}

void NumScene::drawGameOver(void) {
    drawGame();
    oslDrawFillRect(0, 0, 480, 272, 0xCCFFFFFF);
    printText(186, 35, "Game Over");
    oslDrawImageXY(_img.combo, 215, 70);
    printNum(222, 90, _maxCombo, 3, true);
    oslDrawImageXY(_img.score, 215, 120);
    printNum(205, 140, _score, 6, false);
    if (_score > _highScores[9].score) {
        printText(150, 180, "Enter your Name");
        oslDrawLine(191, 213, 289, 213, 0xFF000000);
        oslDrawFillRect(191 + 12 * _currentChar, 199, 204 + 12 * _currentChar, 213, 0x80FF0000);
        printRawText(192, 200, _playerName, 8);
    }
    oslDrawImage(_img.pressKey);
}


void NumScene::updateCredits(void) {
    if (osl_pad.pressed.circle) _status = GAME_MENU;
}

void NumScene::drawCredits(void) {
    int y = 14;
    drawMenu();
    oslDrawFillRect(0, 0, 480, 272, 0xCCFFFFFF);
    printText(20, y, "Credits");
    y += 10;
    printTextMini(30, y += 18, "Coding : Kcazer");
    printTextMini(44, y += 12, "http://tsundere.fr");

    printTextMini(30, y += 18, "Original idea : Gil Bear");
    printTextMini(44, y += 12, "http://gil.nobody.jp");

    printTextMini(30, y += 18, "PSP Library : OSLib");
    printTextMini(44, y += 12, "http://oslib.playeradvance.org");

    printTextMini(30, y += 18, "BGM : SILDRA COMPANY");
    printTextMini(44, y += 12, "http://sildra.ddo.jp");

    printTextMini(30, y += 18, "Sound effects : OSA");
    printTextMini(44, y += 12, "http://osabisi.sakura.ne.jp");

    printTextMini(30, y += 18, "Fonts :");
    printTextMini(44, y += 12, "- MW QUOIN : http://www.milkwort.org");
    printTextMini(44, y += 12, "- Visitor");

    oslDrawImage(_img.pressKey);
}


void NumScene::startGame(void) {
    int x, y;
    // Initialisation map
    for (y = 0; y < 8; y++)
        for (x = 0; x < 12; x++) {
            _map[y][x].value = ((x < 3) ? rand() % 10 : -1);
            _map[y][x].effectDisplay = EFFECT_NONE;
            _map[y][x].position = 0;
        }
    // Divers
    _hp = 100;
    _speed = 8;
    _score = 0;
    _combo = 1;
    _maxCombo = 1;

    _status = GAME_PLAYING;
    oslStopSound(_bgm.menu);
    oslPlaySound(_bgm.game, 0);

    _timer = 5;
    addGoal();
    addGoal();
    addGoal();
}

void NumScene::addBlock(void) {
    int y = rand() % 8;
    _map[y][11].value = rand() % 10;
    _map[y][11].position = 0;
}

void NumScene::addGoal(void) {
    _goal[0] = _goal[1];
    _goal[1] = _goal[2];
    _goal[2] = 5 + rand() % 12;
}


void NumScene::printRawText(int x, int y, char *text, int size) {
    char c;
    int i = 0;
    _img.font->x = x;
    _img.font->y = y;
    while (size-- != 0) {
        c = text[i++];
        if (c < 42) {
            oslSetImageFrame(_img.font, c);
            oslDrawImage(_img.font);
        }
        _img.font->x += 12;
    }
}

void NumScene::printText(int x, int y, char *text) {
    int i = 0;
    char c;
    _img.font->x = x;
    _img.font->y = y;
    while ((c = text[i++]) != 0) {
        if (c >= '0' && c <= '9') c = c - '0';
        else if (c >= 'a' && c <= 'z') c = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') c = c - 'A' + 10;
        else if (c == ' ') c = -1;
        else if (c == '@') c = 36;
        else if (c == '-') c = 37;
        else if (c == '_') c = 38;
        else if (c == ':') c = 39;
        else if (c == '.') c = 40;
        else if (c == '/') c = 41;
        else c = -1;
        if (c >= 0) {
            oslSetImageFrame(_img.font, c);
            oslDrawImage(_img.font);
        }
        _img.font->x += 12;
    }
}

void NumScene::printTextMini(int x, int y, char *text) {
    int i = 0;
    char c;
    _img.fontMini->x = x;
    _img.fontMini->y = y;
    while ((c = text[i++]) != 0) {
        if (c >= '0' && c <= '9') c = c - '0';
        else if (c >= 'a' && c <= 'z') c = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z') c = c - 'A' + 10;
        else if (c == ' ') c = -1;    // Optimisation
        else if (c == '@') c = 36;
        else if (c == '-') c = 37;
        else if (c == '_') c = 38;
        else if (c == ':') c = 39;
        else if (c == '.') c = 40;
        else if (c == '/') c = 41;
        else c = -1;
        if (c >= 0) {
            oslSetImageFrame(_img.fontMini, c);
            oslDrawImage(_img.fontMini);
        }
        _img.fontMini->x += 6;
    }
}

void NumScene::printNum(int x, int y, int num, int pad, bool hideZero) {
    int num2 = num;
    bool first = true;
    if (pad == 0) while (num2 /= 10) pad++;
    _img.font->x = x + 12 * (pad - 1);
    _img.font->y = y;
    while (pad > 0 || num != 0) {
        oslSetImageFrame(_img.font, num % 10);
        if (num != 0 || !hideZero || first) oslDrawImage(_img.font);
        num = (num - (num % 10)) / 10;
        _img.font->x -= 12;
        first = false;
        pad--;
    }
}


void NumScene::loadScores(void) {
    int i, j, b0, b1, b2, b3;
    FILE *f = fopen("scores.dat", "rb");
    if (f == NULL) {
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 8; j++) _highScores[i].name[j] = 37;
            _highScores[i].combo = 0;
            _highScores[i].score = 0;
        }
        saveScores();
    } else {
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 8; j++) _highScores[i].name[j] = fgetc(f);
            b1 = fgetc(f);
            b0 = fgetc(f);
            _highScores[i].combo = (b1 << 8) | b0;
            b3 = fgetc(f);
            b2 = fgetc(f);
            b1 = fgetc(f);
            b0 = fgetc(f);
            _highScores[i].score = (b3 << 0x18) | (b2 << 0x10) | (b1 << 0x08) | b0;
        }
    }
    fclose(f);
}

void NumScene::saveScores(void) {
    int i, j;
    FILE *f = fopen("scores.dat", "wb");
    if (f == NULL) oslDebug("Error saving HighScores");
    else {
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 8; j++) fputc(_highScores[i].name[j], f);
            fputc((_highScores[i].combo >> 0x08) & 0xFF, f);
            fputc((_highScores[i].combo >> 0x00) & 0xFF, f);
            fputc((_highScores[i].score >> 0x18) & 0xFF, f);
            fputc((_highScores[i].score >> 0x10) & 0xFF, f);
            fputc((_highScores[i].score >> 0x08) & 0xFF, f);
            fputc((_highScores[i].score >> 0x00) & 0xFF, f);
        }
        fclose(f);
    }
}
