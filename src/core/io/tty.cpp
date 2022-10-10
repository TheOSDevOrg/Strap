#include <std/tty.h>
#include <core/io/out.hpp>
#include <std/array.h>
#include <std/arraylist.h>
#include <core/memory.h>
#include <core/kernel.hpp>

using namespace std;
using namespace system::core::io;
using namespace system::hal::drivers::kb;
using namespace system::kernel;

uint32_t std::ctty_num;

namespace std::data::tty
{
    array<key_t> _seq1;
    array<key_t> _seq2;
    array<key_t> _seq3;
    array<key_t> _seq4;
    array<key_t> _seq5;
    array<key_t> _seq_fw;
    array<key_t> _seq_bk;
}

tty::tty(uint8_t initx, uint8_t inity)
: _stdout(),
  _stdin(1000, true),
  _lock(false),
  _reg(false)
{
    _stdout.init(0xb8000, 80, 25, 2, initx, inity);
    _stdout._lock = true;
}
tty::~tty()
{
}

void tty::dispose()
{
    _lock = true;
    _stdout.dispose();
    _stdout.~out();
}

void tty::write(array<char> str)
{
    _lock = true;
    _stdout.puts(str.c_list());
    _lock = false;
}
void tty::write(arraylist<char> str)
{
    _lock = true;
    _stdout.puts(str.c_list());
    _lock = false;
}
void tty::write(char *str)
{
    _lock = true;
    _stdout.puts(str);
    _lock = false;
}
void tty::write(char c)
{
    _stdout.putc(c);
}
void tty::write(string &str)
{
    _lock = true;
    _stdout.puts(str);
    _lock = false;
}

void tty::write_line(array<char> &str)
{
    _lock = true;
    _stdout.puts(str.c_list());
    _stdout.putc('\n');
    _lock = false;
}
void tty::write_line(arraylist<char> &str)
{
    _lock = true;
    _stdout.puts(str.c_list());
    _stdout.putc('\n');
    _lock = false;
}
void tty::write_line(char *str)
{
    _lock = true;
    _stdout.puts(str);
    _stdout.putc('\n');
    _lock = false;
}
void tty::write_line(char c)
{
    _stdout.putc(c);
    _stdout.putc('\n');
}
void tty::write_line(string str)
{
    _lock = true;
    _stdout.puts(str);
    _stdout.putc('\n');
    _lock = false;
}

void tty::clear()
{
    _lock = true;
    _stdout.clean();
    _lock = false;
}
void tty::render()
{
    //if (_lock) return;
    return _stdout.render();
}
void tty::handle_scancode(scancode_t s, bool right)
{
    key_t k = this->parse_scancode(s, right);
    if (_stdin.size())
        for (int r = _stdin.size()-1; r >= 0; r--)
        {
            auto &key = _stdin.at(r);
            if (key.type == k.type && key.data.character == k.data.character && key.focus != k.focus)
            {
                if (key.type != keytype_t::SPECIAL) break;
                if (_stdin.size() > 1) _stdin.remove_at(r);
                else _stdin.clear();
                return;
            }
            else if (key.type == k.type && key.data.character == k.data.character && key.focus == k.focus)
            {
                if (key.type == keytype_t::SPECIAL) return;
                break;
            }
        }
    if (k.focus == focus_t::RELEASE) return;
    _stdin.add(k);

    int sz;
    int x = _stdin.size();
    
    if (x < 2 && k.type == keytype_t::NORMAL)
    {
        key_t k = _stdin.at(_stdin.size()-1);
        _stdin.remove_at(_stdin.size()-1);
        handle_input(k);
        return;
    }

    array<key_t> seq = array<key_t>(x);
    for (int y = 0; y < x; y++) seq.at(y) = _stdin.at(y);

    if (!handle_sequence(seq) && k.type == keytype_t::NORMAL)
    {
        key_t kl = _stdin.at(_stdin.size()-1);
        _stdin.remove_at(_stdin.size()-1);
        handle_input(kl);
    }
}
void tty::init()
{
    data::tty::_seq1 = array<key_t>(2, true);
    data::tty::_seq1[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CTRL_L } };
    data::tty::_seq1[1] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::F1 } };

    data::tty::_seq2 = array<key_t>(2, true);
    data::tty::_seq2[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CTRL_L } };
    data::tty::_seq2[1] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::F2 } };

    data::tty::_seq3 = array<key_t>(2, true);
    data::tty::_seq3[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CTRL_L } };
    data::tty::_seq3[1] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::F3 } };

    data::tty::_seq4 = array<key_t>(2, true);
    data::tty::_seq4[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CTRL_L } };
    data::tty::_seq4[1] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::F4 } };

    data::tty::_seq5 = array<key_t>(2, true);
    data::tty::_seq5[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CTRL_L } };
    data::tty::_seq5[1] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::F5 } };

    data::tty::_seq_fw = array<key_t>(2, true);
    data::tty::_seq_fw[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CTRL_L } };
    data::tty::_seq_fw[1] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CUR_RG } };

    data::tty::_seq_bk = array<key_t>(2, true);
    data::tty::_seq_bk[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CTRL_L } };
    data::tty::_seq_bk[1] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CUR_LF } };
}
void tty::internal_init()
{
    this->register_sequence(data::tty::_seq1, []() 
    {
        if (system::kernel::ttys.size() < 1) return;
        system::kernel::ttys[0]->set();
        ctty_num = 0;
    });
    this->register_sequence(data::tty::_seq2, []() 
    {
        if (system::kernel::ttys.size() < 2) return;
        system::kernel::ttys[1]->set();
        ctty_num = 1;
    });
    this->register_sequence(data::tty::_seq3, []() 
    {
        if (system::kernel::ttys.size() < 3) return;
        system::kernel::ttys[2]->set();
        ctty_num = 2;
    });
    this->register_sequence(data::tty::_seq4, []() 
    {
        if (system::kernel::ttys.size() < 4) return;
        system::kernel::ttys[3]->set();
        ctty_num = 3;
    });
    this->register_sequence(data::tty::_seq5, []() 
    {
        if (system::kernel::ttys.size() < 5) return;
        system::kernel::ttys[4]->set();
        ctty_num = 4;
    });
    this->register_sequence(data::tty::_seq_fw, []() 
    {
        if (system::kernel::ttys.size() <= ctty_num + 1) ctty_num = -1;
        system::kernel::ttys[++ctty_num]->set();
    });
    this->register_sequence(data::tty::_seq_bk, []() 
    {if (ctty_num == 0) ctty_num = system::kernel::ttys.size();
        system::kernel::ttys[--ctty_num]->set();
    });
}
void tty::set(bool first)
{
    if (current_tty == this) return;
    else if (_lock && !first) return;

    if (!first)
    {
        if (current_tty->_stdin.size() != 0) this->_stdin.clone(current_tty->_stdin);
        else this->_stdin.clear();
        current_tty->_stdout._lock = true;
    }
    current_tty = this;
    this->_stdout._lock = false;
}