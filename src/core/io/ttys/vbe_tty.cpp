#include <core/io/ttys/vbe_tty.hpp>
#include <core/kernel.hpp>

using namespace system::kernel;
using namespace std;
using namespace system::core::io::ttys;
using namespace system::hal::drivers::kb;

vbe_tty::vbe_tty(layout *layout, uint16_t w, uint16_t h) : tty(),
_combinations(),
_layout(layout),
_in(false),
_buffer(),
vesa()
{
    internal_init();
    _buffer = system::kernel::gfx::vbe_buffer_init(w, h);
}
vbe_tty::~vbe_tty()
{
}

key_t vbe_tty::parse_scancode(scancode_t scan, bool right)
{
    array<special_t> _comb = array<special_t>(_stdin.size());

    for (int i = 0, sz = _stdin.size(); i < sz; i++) _comb[i] = _stdin[i].data.special;

    auto r = _layout->scan(scan, _comb, right);
    _comb.dispose();
    return r;
}
bool vbe_tty::handle_sequence(array<key_t> &seq)
{
    int i;
    if ((i = _combinations.keys().find(seq)) != -1)
    {
        _combinations.values().at(i)();
        return true;
    }
    return false;
}
void vbe_tty::handle_input(key_t &k)
{
}
bool vbe_tty::register_sequence(array<key_t> &seq, combination_handler handler)
{
    _combinations.add(seq.make_resident(), handler);
    return true;
}

void vbe_tty::enter()
{
    _in = true;
    vesa.Init(_buffer);
    vesa.Clear(__RGB_COLOR_(255, 255, 255));
    return vesa.Render();
}
void vbe_tty::exit()
{
    _in = false;
    return vesa.Disable();
}
void vbe_tty::render()
{
    if (_in) return vesa.Render();
}