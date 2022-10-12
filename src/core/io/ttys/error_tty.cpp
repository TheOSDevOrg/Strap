#include <core/io/ttys/error_tty.hpp>
#include <core/kernel.hpp>


using namespace std;
using namespace system::core::io::ttys;
using namespace system::hal::drivers::kb;

error_tty::error_tty(char *e) :
tty(0, 1)
{
    //internal_init();

    _stdout.clean();

    _stdout.clean(system::core::io::color::white, system::core::io::color::red, 0, 0);
    _stdout.clean(system::core::io::color::white, system::core::io::color::yellow, (_stdout.get_max_x()/2)-((5/2)+1), 0, (_stdout.get_max_x()/2)+((5/2)+2), 0);
    _stdout.puts_color_at("FATAL", system::core::io::color::black, system::core::io::color::yellow, (_stdout.get_max_x()/2)-(5/2), 0);

    _stdout.puts("An error has occurred and Strap has been shut down.\nPlease consider reporting this on github, specifying the steps to reproduce the error.\n\nGithub page: https://www.github.com/TheOSDevOrg/Strap\n");
    _stdout.puts("Error: ");
    _stdout.puts(e);
    _stdout.puts("\n");
}
error_tty::error_tty(string &e) :
tty()
{
    //internal_init();


}
error_tty::~error_tty()
{
}

key_t error_tty::parse_scancode(scancode_t scan, bool right)
{
    return {};
}
bool error_tty::handle_sequence(array<key_t> &seq)
{
    return false;
}
void error_tty::handle_input(key_t &k)
{
    //system::kernel::ttys[2]->write_line("error_tty: input");
}
bool error_tty::register_sequence(array<key_t> &seq, combination_handler handler)
{
    return true;
}

void error_tty::enter() {}
void error_tty::exit() {}