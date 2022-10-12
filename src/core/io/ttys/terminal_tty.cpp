#include <core/io/ttys/terminal_tty.hpp>
#include <core/kernel.hpp>

using namespace system::core::io::ttys;
using namespace std;

terminal_tty::terminal_tty(layout *l, system::core::io::color fg, system::core::io::color bg)
: tty(0, 1),
  _layout(l),
  _combinations(0, true),
  _input((size_t)0, true),
  _callbacks(0, true)
{
    internal_init();

    _stdout.set_fg(fg);
    _stdout.set_bg(bg);
    _stdout.clean();

    _stdout.clean(system::core::io::color::white, system::core::io::color::blue, 0, 0);
    _stdout.clean(system::core::io::color::white, system::core::io::color::cyan, (_stdout.get_max_x()/2)-((8/2)+1), 0, (_stdout.get_max_x()/2)+((8/2)+1), 0);
    _stdout.puts_color_at("TERMINAL", system::core::io::color::white, system::core::io::color::cyan, (_stdout.get_max_x()/2)-(8/2), 0);

    print_term();
}

key_t terminal_tty::parse_scancode(system::hal::drivers::kb::scancode_t scan, bool right)
{
    array<special_t> _comb = array<special_t>(_stdin.size());

    for (int i = 0, sz = _stdin.size(); i < sz; i++) _comb[i] = _stdin[i].data.special;

    auto r = _layout->scan(scan, _comb, right);
    _comb.dispose();
    return r;
}
bool terminal_tty::handle_sequence(array<key_t> &seq)
{
    //combination_handler h = nullptr;
    int i;
    if ((i = _combinations.keys().find(seq)) != -1)
    {
        _combinations.values().at(i)();
        return true;
    }
    return false;
}
bool terminal_tty::register_sequence(array<key_t> &seq, combination_handler handler)
{
    _combinations.add(seq.make_resident(), handler);
    return true;
}

void terminal_tty::handle_input(key_t &k)
{
    if (k.type != keytype_t::NORMAL) return;

    char c = k.data.character;

    if (c == '\b')
    {
        if (_input.size() < 1) return;
        else
        {
            _input.remove_at(_input.size()-1);
            _stdout.putc('\b');
            return;
        }
    }
    else if (c == '\n')
    {
        _stdout.putc('\n');
        handle_command();
        _input.clear();
        print_term();
        return;
    }

    _stdout.putc(c);
    _input.add(c);
    _stdout.render();
}

void terminal_tty::handle_command()
{
    arraylist<string> args = arraylist<string>(0, true);
    _input.split(' ', args, true);
    string cmd_name = args[0].c_str();
    cmd_name.to_upper();
    args[0].dispose();
    args.remove_at(0);

    kernel::ttys[2]->write("command: ");
    kernel::ttys[2]->write_line(cmd_name);
    kernel::ttys[2]->write("args: ");
    
    for (auto arg : args)
    {
        kernel::ttys[2]->write(arg);
        kernel::ttys[2]->write(", ");
    }
    kernel::ttys[2]->write("\n");

    int i;
    if((i = _callbacks.keys().find(cmd_name)) != -1) 
    {
        cmd_callback h = _callbacks.values().at(i);
        if (h != nullptr)
        {
            h(args);
        }
    }
    else if (cmd_name != "")
    {
        kernel::current_tty->write("StrapTerm: command ");
        kernel::current_tty->write(cmd_name);
        kernel::current_tty->write_line(" not found");
    }
    cmd_name.dispose();
    for (auto s : args) s.dispose();
    args.dispose();
}

void terminal_tty::print_term()
{
    _stdout.puts_color("[strap term]", system::core::io::color::green, _stdout.get_bg());
    _stdout.putc(' ');
}

void terminal_tty::enter() { _input.clear(); }
void terminal_tty::exit() {}