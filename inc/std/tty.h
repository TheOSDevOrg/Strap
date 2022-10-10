#pragma once

#include <stdint.h>
#include <stddef.h>
#include <std/decl.h>
#include <core/io/out.hpp>
#include <std/array.h>
#include <std/arraylist.h>
#include <std/string.h>
#include <hal/drivers/keyboard.h>

#ifdef __cplusplus

namespace std
{
    extern uint32_t ctty_num;

    enum class focus_t : uint8_t
    {
        PRESS,
        RELEASE
    };
    enum class keytype_t : uint8_t
    {
        NORMAL,
        SPECIAL
    };
    enum class special_t : uint8_t
    {
        ESC,
        CANC,
        
        CTRL_L,
        ALT_L,
        SFT_L,
        SPC_L,

        CPSLK,
        NUMLK,
        SCRLK,
        TAB,

        CTRL_R,
        ALT_R,
        SFT_R,

        UP_ARRW,
        DW_ARRW,
        RX_ARRW,
        LX_ARRW,

        BLCK_NUM,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        NEXT_TRACK,
        MUTE,
        CALC,
        PLAY,
        STOP,
        VOL_UP,
        VOL_DW,
        HOME,
        CUR_UP,
        CUR_LF,
        CUR_RG,
        CUR_DW,
        PG_UP,
        PG_DW,
        INSERT,
        END,
        GUI_LF,
        GUI_RG,
        APPS,
        POWER,
        SLEEP,
        WAKE
    };
    struct key_t final
    {
        focus_t focus : 1;
        keytype_t type : 1;

        union data_t
        {
            char character;
            special_t special;
        } data;

    public:
        bool operator==(key_t &right)
        {
            return focus == right.focus && type == right.type && data.character == right.data.character;
        }
        bool operator!=(key_t &right)
        {
            return !this->operator==(right);
        }
    };

    typedef void (*combination_handler)();

    class tty
    {
    friend class system::hal::drivers::kb::driver_t;
    public:
        bool _lock, _reg;
    public:
        tty(uint8_t initx = 0, uint8_t inity = 0);
        ~tty();
    protected:
        system::core::io::out _stdout;
        std::arraylist<key_t> _stdin;
    public:
        void write(array<char> str);
        void write(arraylist<char> str);
        void write(char *str);
        void write(char c);
        void write(string &str);
    public:
        void write_line(array<char> &str);
        void write_line(arraylist<char> &str);
        void write_line(char *str);
        void write_line(char c);
        void write_line(string str);
    public:
        void clear();
        void render();
    public:
        inline system::core::io::color get_fg() { return _stdout.get_fg(); }
        inline system::core::io::color get_bg() { return _stdout.get_bg(); }
    public:
        inline void set_fg(system::core::io::color c) { _stdout.set_fg(c); }
        inline void set_bg(system::core::io::color c) { _stdout.set_bg(c); }
    public:
        void dispose();
    private:
        void handle_scancode(system::hal::drivers::kb::scancode_t scan, bool right = false);
        virtual key_t parse_scancode(system::hal::drivers::kb::scancode_t scan, bool right = false) = 0;
        virtual bool handle_sequence(array<key_t> &sequence) = 0;
        virtual void handle_input(key_t &k) = 0;
    public:
        virtual bool register_sequence(array<key_t> &sequence, combination_handler handler) = 0;
    protected:
        void internal_init();
    public:
        static void init();
        void set(bool first = false);
    };
}

#endif