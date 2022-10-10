#include <core/io/debug.hpp>
#include <core/kernel.hpp>

using namespace std;

dbgio::dbgtype dbgio::debug_lvl = dbgio::dbgtype::ERROR;
bool dbgio::serial = true;
bool dbgio::tty = false;

bool dbgio::print_debug(const char *fmt)
{
    if (debug_lvl < dbgtype::DEBUG) return false;

    int s = 0, j = 0;
    while (fmt[j++]) s++;    
    
    if (tty)
    {
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("[");
        system::kernel::ttys[0]->set_fg(system::core::io::color::cyan);
        system::kernel::ttys[0]->write("DEBUG");
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("] ");
        system::kernel::ttys[0]->set_fg(system::core::io::color::white);
        system::kernel::ttys[0]->write_line((char *)fmt);
    }

    if (serial)
    {
        system::kernel::env->stddbg.writes_color("[", 1, 0x0F);
        system::kernel::env->stddbg.writes_color("DEBUG", 5, 0x0B);
        system::kernel::env->stddbg.writes_color("]", 1, 0x0F);
        system::kernel::env->stddbg.writes(" ", 1);
        return system::kernel::env->stddbg.writes_color_ln(fmt, s, 0xFF);
    }
    return true;
}
bool dbgio::print_error(const char *fmt)
{
    if (debug_lvl < dbgtype::ERROR) return false;

    int s = 0, j = 0;
    while (fmt[j++]) s++;

    if (tty)
    {
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("[");
        system::kernel::ttys[0]->set_fg(system::core::io::color::red);
        system::kernel::ttys[0]->write("ERROR");
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("] ");
        system::kernel::ttys[0]->set_fg(system::core::io::color::white);
        system::kernel::ttys[0]->write_line((char *)fmt);
    }

    if (serial)
    {
        system::kernel::env->stddbg.writes_color("[", 1, 0x0F);
        system::kernel::env->stddbg.writes_color("ERROR", 5, 0x0C);
        system::kernel::env->stddbg.writes_color("]", 1, 0x0F);
        system::kernel::env->stddbg.writes(" ", 1);
        return system::kernel::env->stddbg.writes_color_ln(fmt, s, 0xFF);
    }
    return true;
}
bool dbgio::print_info(const char *fmt)
{
    if (debug_lvl < dbgtype::INFO) return false;

    int s = 0, j = 0;
    while (fmt[j++]) s++;

    if (tty)
    {
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("[");
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_green);
        system::kernel::ttys[0]->write("INFO");
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("]  ");
        system::kernel::ttys[0]->set_fg(system::core::io::color::white);
        system::kernel::ttys[0]->write_line((char *)fmt);
    }

    if (serial)
    {
        system::kernel::env->stddbg.writes_color("[", 1, 0x0F);
        system::kernel::env->stddbg.writes_color("INFO", 4, 0x0A);
        system::kernel::env->stddbg.writes_color("]", 1, 0x0F);
        system::kernel::env->stddbg.writes("  ", 2);
        return system::kernel::env->stddbg.writes_color_ln(fmt, s, 0xFF);
    }
    return true;
}
bool dbgio::print_warn(const char *fmt)
{
    if (debug_lvl < dbgtype::WARNING) return false;

    int s = 0, j = 0;
    while (fmt[j++]) s++;

    if (tty)
    {
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("[");
        system::kernel::ttys[0]->set_fg(system::core::io::color::yellow);
        system::kernel::ttys[0]->write("WARN");
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("]  ");
        system::kernel::ttys[0]->set_fg(system::core::io::color::white);
        system::kernel::ttys[0]->write_line((char *)fmt);
    }

    if (serial)
    {
        system::kernel::env->stddbg.writes_color("[", 1, 0x0F);
        system::kernel::env->stddbg.writes_color("WARN", 4, 0x0E);
        system::kernel::env->stddbg.writes_color("]", 1, 0x0F);
        system::kernel::env->stddbg.writes("  ", 2);
        return system::kernel::env->stddbg.writes_color_ln(fmt, s, 0xFF);
    }
    return true;
}
bool dbgio::fatal_error(const char *fmt)
{
    int s = 0, j = 0;
    while (fmt[j++]) s++;

    if (tty)
    {
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("[");
        system::kernel::ttys[0]->set_fg(system::core::io::color::red);
        system::kernel::ttys[0]->write("ERROR");
        system::kernel::ttys[0]->set_fg(system::core::io::color::light_gray);
        system::kernel::ttys[0]->write("] ");
        system::kernel::ttys[0]->set_fg(system::core::io::color::white);
        system::kernel::ttys[0]->write_line((char *)fmt);
    }

    system::kernel::env->stddbg.writes_color("[", 1, 0x0F);
    system::kernel::env->stddbg.writes_color("ERROR", 5, 0x0C);
    system::kernel::env->stddbg.writes_color("]", 1, 0x0F);
    system::kernel::env->stddbg.writes(" ", 1);
    system::kernel::env->stddbg.writes_color_ln(fmt, s, 0xFF);

    print_warn("The system is about to be halt");
    asm("hlt");
    while(true);
    return true;
}

__cdecl bool dprint_debug(const char *fmt)
{
    return dbgio::print_debug(fmt);
}
__cdecl bool dprint_error(const char *fmt)
{
    return dbgio::print_error(fmt);
}
__cdecl bool dprint_info(const char *fmt)
{
    return dbgio::print_info(fmt);
}
__cdecl bool dprint_warn(const char *fmt)
{
    return dbgio::print_warn(fmt);
}
__cdecl bool fatal(const char *fmt)
{
    return dbgio::fatal_error(fmt);
}