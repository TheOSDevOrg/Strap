#include <boot/mboot.h>
namespace system::grub
{
    multiboot_hdr *mboot_ptr = nullptr;
    void init(multiboot_hdr *mboot)
    {
        mboot_ptr = mboot;
    }
    multiboot_hdr* get()
    {
        return mboot_ptr;
    }
}