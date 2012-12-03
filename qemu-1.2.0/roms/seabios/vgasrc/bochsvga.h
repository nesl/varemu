#ifndef __BOCHSVGA_H
#define __BOCHSVGA_H

#include "types.h" // u8
#include "ioport.h" // outw

#define VBE_DISPI_BANK_ADDRESS           0xA0000
#define VBE_DISPI_BANK_SIZE_KB           64

#define VBE_DISPI_MAX_XRES               2560
#define VBE_DISPI_MAX_YRES               1600

#define VBE_DISPI_IOPORT_INDEX           0x01CE
#define VBE_DISPI_IOPORT_DATA            0x01CF

#define VBE_DISPI_INDEX_ID               0x0
#define VBE_DISPI_INDEX_XRES             0x1
#define VBE_DISPI_INDEX_YRES             0x2
#define VBE_DISPI_INDEX_BPP              0x3
#define VBE_DISPI_INDEX_ENABLE           0x4
#define VBE_DISPI_INDEX_BANK             0x5
#define VBE_DISPI_INDEX_VIRT_WIDTH       0x6
#define VBE_DISPI_INDEX_VIRT_HEIGHT      0x7
#define VBE_DISPI_INDEX_X_OFFSET         0x8
#define VBE_DISPI_INDEX_Y_OFFSET         0x9
#define VBE_DISPI_INDEX_VIDEO_MEMORY_64K 0xa

#define VBE_DISPI_ID0                    0xB0C0
#define VBE_DISPI_ID1                    0xB0C1
#define VBE_DISPI_ID2                    0xB0C2
#define VBE_DISPI_ID3                    0xB0C3
#define VBE_DISPI_ID4                    0xB0C4
#define VBE_DISPI_ID5                    0xB0C5

#define VBE_DISPI_DISABLED               0x00
#define VBE_DISPI_ENABLED                0x01
#define VBE_DISPI_GETCAPS                0x02
#define VBE_DISPI_8BIT_DAC               0x20
#define VBE_DISPI_LFB_ENABLED            0x40
#define VBE_DISPI_NOCLEARMEM             0x80

#define VBE_DISPI_LFB_PHYSICAL_ADDRESS   0xE0000000

static inline u16 dispi_read(u16 reg)
{
    outw(reg, VBE_DISPI_IOPORT_INDEX);
    return inw(VBE_DISPI_IOPORT_DATA);
}
static inline void dispi_write(u16 reg, u16 val)
{
    outw(reg, VBE_DISPI_IOPORT_INDEX);
    outw(val, VBE_DISPI_IOPORT_DATA);
}

struct vgamode_s *bochsvga_find_mode(int mode);
void bochsvga_list_modes(u16 seg, u16 *dest, u16 *last);
int bochsvga_get_window(struct vgamode_s *vmode_g, int window);
int bochsvga_set_window(struct vgamode_s *vmode_g, int window, int val);
int bochsvga_get_linelength(struct vgamode_s *vmode_g);
int bochsvga_set_linelength(struct vgamode_s *vmode_g, int val);
int bochsvga_get_displaystart(struct vgamode_s *vmode_g);
int bochsvga_set_displaystart(struct vgamode_s *vmode_g, int val);
int bochsvga_get_dacformat(struct vgamode_s *vmode_g);
int bochsvga_set_dacformat(struct vgamode_s *vmode_g, int val);
int bochsvga_size_state(int states);
int bochsvga_save_state(u16 seg, void *data, int states);
int bochsvga_restore_state(u16 seg, void *data, int states);
int bochsvga_set_mode(struct vgamode_s *vmode_g, int flags);
int bochsvga_init(void);

#endif // bochsvga.h
