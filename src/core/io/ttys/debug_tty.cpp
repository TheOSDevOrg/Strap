#include <core/io/ttys/debug_tty.hpp>
#include <core/kernel.hpp>

using namespace std;
using namespace system::core::io::ttys;
using namespace system::hal::drivers::kb;

debug_tty::debug_tty(layout *layout) :
tty(),
_combinations(),
_layout(layout)
{
    internal_init();
    write_line("DEBUG_TTY");
}
debug_tty::~debug_tty()
{
}

key_t debug_tty::parse_scancode(scancode_t scan, bool right)
{
    array<special_t> _comb = array<special_t>(_stdin.size());

    for (int i = 0, sz = _stdin.size(); i < sz; i++) _comb[i] = _stdin[i].data.special;

    auto r = _layout->scan(scan, _comb, right);
    _comb.dispose();
    return r;
}
bool debug_tty::handle_sequence(array<key_t> &seq)
{
    combination_handler h = nullptr;
    for (auto s : _combinations.keys())
    {
        if (s->operator==(seq) && (h = _combinations[s]) != nullptr) break;
    }
    if (h != nullptr)
    {
        h();
        return true;
    }
    return false;
}
void debug_tty::handle_input(key_t &k)
{
    //system::kernel::ttys[2]->write_line("debug_tty: input");
}
bool debug_tty::register_sequence(array<key_t> &seq, combination_handler handler)
{
    _combinations.add(&seq, handler);
    return false;
}