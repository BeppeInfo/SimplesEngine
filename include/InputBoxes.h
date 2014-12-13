#ifndef INPUTBOXES_H
#define INPUTBOXES_H

#include "include/utils.h"

class InputBox
{
    public:
        InputBox( int, int, int, int, int, string = "" );
        virtual ~InputBox();

        virtual bool Update() = 0;
        virtual void Draw();

        void SnapCursor();
        bool CursorInside();
        virtual string BoxText() = 0;

        bool selected;

    protected:
        int x, y, w, h, bw;
        SDL_Rect box;
        SDL_Surface *icon;
};

class Button : public InputBox
{
    public:
        Button( int, int, int, int, int, string, string );
        virtual ~Button();

        virtual bool Update();
        virtual void Draw();

        virtual string BoxText() { return link; }

    private:
        string link;
};

class TextTypeBox : public InputBox
{
    public:
        TextTypeBox( int, int, int, int, int, string& );
        virtual ~TextTypeBox();

        virtual bool Update();

        virtual string BoxText() { return ref_text; }

    private:
        string& ref_text;
};

class FileSelectionBox : public InputBox
{
    public:
        FileSelectionBox( int, int, int, int, int, string, string& );
        virtual ~FileSelectionBox();

        virtual bool Update();

        virtual string BoxText() { return ref_file; }

    private:
        string& ref_file;
        vector<string> file_list;
        vector<string>::iterator file_it;
};

class NumberSelectionBox : public InputBox
{
    public:
        NumberSelectionBox( int, int, int, int, int, int&, int = 0, int = 0, string = "" );
        virtual ~NumberSelectionBox();

        virtual bool Update();

        virtual string BoxText()
        {
            stringstream string_output;
            string_output << ref_value << unit;
            return string_output.str();
        }

    private:
        int& ref_value;
        int min_value, max_value;
        string unit;
};

enum { POS, VEL, CURR };

class EposValueBox : public InputBox
{
    public:
        EposValueBox( int, int, int, int, int, int&, int = 0, int = 0 );
        virtual ~EposValueBox();

        virtual bool Update();

        virtual string BoxText()
        {
            stringstream string_output;

            if( mode == POS ) string_output << "Pos: ";
            else if( mode == VEL ) string_output << "Vel: ";
            else if( mode == CURR ) string_output << "Cur: ";

            string_output << ref_value;
            return string_output.str();
        }

    private:
        int& ref_value;
        int eposId;
        int mode;
};

#endif // INPUTBOXES_H
