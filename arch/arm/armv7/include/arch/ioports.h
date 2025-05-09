
#ifndef __POK_ARMV7_IOPORTS_H__
#define __POK_ARMV7_IOPORTS_H__

#define JET_ARCH_DECLARE_IO_PORT 1

#define ja_outb(port, data)             \
({                                      \
    *(volatile uint8_t*)(port) = data;  \
})

#define ja_inb(port)            \
({                              \
    *(volatile uint8_t*)(port); \
})

#define ja_outw(port, data)             \
({                                      \
    *(volatile uint16_t*)(port) = data;  \
})

#define ja_inw(port)            \
({                              \
    *(volatile uint16_t*)(port); \
})

#define ja_outl(port, data)             \
({                                      \
    *(volatile uint32_t*)(port) = data;  \
})

#define ja_inl(port)            \
({                              \
    *(volatile uint32_t*)(port); \
})

#endif /* __POK_ARMV7_IOPORTS_H__ */
