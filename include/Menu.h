#ifndef MENU_H
#define MENU_H

#include "include/GameScreen.h"

class Menu : public GameScreen
{
    public:
        Menu(string = "");
        virtual ~Menu();

        virtual bool Update();

    private:
        static uint8_t currentBox;
        static InputBox* handle;
};

#endif // MENU_H
