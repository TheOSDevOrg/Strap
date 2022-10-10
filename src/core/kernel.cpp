#include <core/kernel.hpp>
#include <core/io/debug.hpp>
#include <hal/interrupts/gdt.h>
#include <hal/interrupts/idt.h>
#include <core/io/layouts/english.hpp>
using namespace system::hal::drivers;
using namespace system::core::memory;
using namespace system::hal::interrupts;
using namespace system::core::io;
using namespace system::core::io::ttys;
using namespace system::hal::drivers;

system::kernel::environment *system::kernel::env = nullptr;
heap_t system::kernel::heap;
std::tty *system::kernel::current_tty;
std::arraylist<std::tty *> system::kernel::ttys;
kb::driver_t *system::kernel::kb;
system::hal::drivers::vbe::Driver system::kernel::vesa;
void system::kernel::tests::alloc_routine_check()
{
    using namespace std;

    arraylist<char> test = arraylist<char>();

    ttys[3]->write_line("insert test");
    test.add('a');
    test.add('c');
    test.add('\0');
    test.insert('b', 1);
    ttys[3]->write_line(test.c_list());
    heap.defragment();
    heap.print_table();
}

void system::kernel::bootstrap(system::grub::multiboot_hdr *mboot)
{
    std::dbgio::serial = false;
    std::dbgio::debug_lvl = std::dbgio::dbgtype::WARNING;
    system::grub::init(mboot);

    dprint_info("kernel environment has been loaded!");
    dprint_info("bootstrap complete!");
}

void system::kernel::beforerun()
{
    dprint_info("initializing kernel features...");

    gdt::manager_t::load_gdt();
    dprint_info("initializing gdt... OK");

    idt::manager_t::init();
    dprint_info("initializing idt... OK");

    idt::manager_t::enable_interrupts();
    static kb::driver_t drv = kb::driver_t();
    kb = &drv;
}
void system::kernel::run()
{
    using namespace std;

    out::sinit();

    tty::init();
    layouts::english kb_layout = layouts::english();

    ttys = arraylist<tty *>(5, true);
    debug_tty kernel_tty = debug_tty(&kb_layout);
    terminal_tty terminal_tty_i = terminal_tty(&kb_layout);
    debug_tty tests_tty = debug_tty(&kb_layout);
    debug_tty debug_tty_i = debug_tty(&kb_layout);
    vbe_tty vbe_tty_i = vbe_tty(&kb_layout);

    ttys.add((tty*)&kernel_tty);
    ttys.add(((tty*)&terminal_tty_i));
    ttys.add(((tty*)&debug_tty_i));
    //ttys.add(((tty*)&tests_tty));
    ttys.add(((tty*)&vbe_tty_i));
    ttys[1]->set(true);
    std::ctty_num = 1;
    std::dbgio::tty = true;
    std::dbgio::serial = true;

    terminal_tty_i.register_command("TEST", [](arraylist<string> &args) {
        current_tty->write_line("Hello, World!");
    });
    terminal_tty_i.register_command("ECHO", [](arraylist<string> &args){
        if (args.size() < 1)
        {
            current_tty->write_line("This command takes at least 1 argument");
            return;
        }

        for (auto s : args){
            current_tty->write(s);
            current_tty->write(' ');
        }
        current_tty->write_line("");
    });
    terminal_tty_i.register_command("CLS", [](arraylist<string> &args){
        if (args.size() == 0) current_tty->clear();
        else
        {
            if (args[0] == "all")
            {
                for (auto t : ttys)
                {
                    t->clear();
                }
                return;
            }
            for (auto arg : args)
            {
                int n = arg.to_number();
                if (n < 1 | n > 5) return current_tty->write_line("The tty argument goes from 1 to 5");
                ttys.at(n-1)->clear();
            }
        }
    });
    terminal_tty_i.register_command("DBGLVL", [](arraylist<string> &args){
        if (args.size() != 1)
        {
            current_tty->write_line("This command doesn't take any argument");
            return;
        }
        args[0].to_upper();
        if (args[0] == "DEBUG") {
            dbgio::debug_lvl = dbgio::dbgtype::DEBUG;
        } else if (args[0] == "INFO") {
            dbgio::debug_lvl = dbgio::dbgtype::INFO;
        } else if (args[0] == "WARNING" || args[0] == "WARN") {
            dbgio::debug_lvl = dbgio::dbgtype::WARNING;
        } else if (args[0] == "ERROR") {
            dbgio::debug_lvl = dbgio::dbgtype::ERROR;
        } else if (args[0] == "NONE") {
            dbgio::debug_lvl = dbgio::dbgtype::NONE;
        }
    });
    terminal_tty_i.register_command("TBLPRINT", [](arraylist<string> &args){
        heap.defragment();
        heap.print_table();
        current_tty->write_line("printed table on debug tty (CTRL+F3)");
    });
    terminal_tty_i.register_command("TTY", [](arraylist<string> &args){
        if (args.size() != 1) return current_tty->write_line("This command takes 1 argument");
        int n = args[0].to_number();
        if (n < 1 | n > 5) return current_tty->write_line("The tty argument goes from 1 to 5");
        ttys[--n]->set();
        ctty_num = n;
    });
    terminal_tty_i.register_command("MEMINFO", [](arraylist<string> &args){
        if (args.size() != 0) return current_tty->write_line("This command doesn't take any arguments");
        auto frees = string::from_number(heap.unused_kb());
        current_tty->write_line(frees);
        frees.dispose();
    });
    terminal_tty_i.register_command("ALLOC", [](arraylist<string> &args){
        if (args.size() != 0) return current_tty->write_line("This command doesn't take any arguments");
        alloc(200);
        current_tty->write_line("alloc was successfull");
    });
    terminal_tty_i.register_command("MBOOT", [](arraylist<string> &args){
        current_tty->write("Bootloader: ");
        current_tty->write_line(system::grub::get()->bootloader_name);
    });
    terminal_tty_i.register_command("VBE", [](arraylist<string> &args){
        vesa.Init();
            vesa.Clear(VBE_COLOR::black);
            vesa.FilledRect(200,200,20,20,(uint32_t)VBE_COLOR::dark_cyan);
            vesa.DrawString(0,0,"Strap VBE Test",system::kernel::gfx::Fonts::System8x16,(uint32_t)VBE_COLOR::white,(uint32_t)VBE_COLOR::black);
            vesa.Render(); //vesa.Disable();
    });
    terminal_tty_i.register_command("ABOUT", [](arraylist<string> &args){
        if (args.size() != 0) return current_tty->write_line("This command doesn't take any arguments");
        current_tty->write_line("Strap kernel [v 0.1.2022.09.10]");
    });
    terminal_tty_i.register_command("INFO", [](arraylist<string> &args){
        if (args.size() != 0) return current_tty->write_line("This command doesn't take any arguments");
        current_tty->write("StrapTerm running on tty ");
        char s[2];
        numstr(ctty_num+1, 10, s);
        current_tty->write_line(s);
    });
    array<key_t> seq = array<key_t>(1);

    seq[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CUR_UP } };
    terminal_tty_i.register_sequence(seq, []() { kernel::ttys[2]->write_line("test curr up"); });

    idt::manager_t::register_irq(0, [](idt::regs32_t *r) {
        current_tty->render();
    });

    kb->start();

    heap.defragment();
    
    array<key_t> dbgseq = array<key_t>(2);

    dbgseq[0] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::CTRL_L } };
    dbgseq[1] = key_t { .focus = focus_t::PRESS, .type = keytype_t::SPECIAL, .data = { .special = special_t::ALT_L } };
    tests_tty.register_sequence(dbgseq, []() { heap.print_table(); });

    //tests::alloc_routine_check();

    while(true);
}
