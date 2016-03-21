#define PHYSADDR_OFFSET 0x3F200000

#define GPFSEL1         (PHYSADDR_OFFSET + 0x00000004)
#define GPSET0          (PHYSADDR_OFFSET + 0x0000001C)
#define GPCLR0          (PHYSADDR_OFFSET + 0x00000028)
#define GPPUD           (PHYSADDR_OFFSET + 0x00000094)
#define GPPUDCLK0       (PHYSADDR_OFFSET + 0x00000098)

#define AUX_ENABLES     (PHYSADDR_OFFSET + 0x00015004)
#define AUX_MU_IO_REG   (PHYSADDR_OFFSET + 0x00015040)
#define AUX_MU_IER_REG  (PHYSADDR_OFFSET + 0x00015044)
#define AUX_MU_IIR_REG  (PHYSADDR_OFFSET + 0x00015048)
#define AUX_MU_LCR_REG  (PHYSADDR_OFFSET + 0x0001504C)
#define AUX_MU_MCR_REG  (PHYSADDR_OFFSET + 0x00015050)
#define AUX_MU_LSR_REG  (PHYSADDR_OFFSET + 0x00015054)
#define AUX_MU_MSR_REG  (PHYSADDR_OFFSET + 0x00015058)
#define AUX_MU_SCRATCH  (PHYSADDR_OFFSET + 0x0001505C)
#define AUX_MU_CNTL_REG (PHYSADDR_OFFSET + 0x00015060)
#define AUX_MU_STAT_REG (PHYSADDR_OFFSET + 0x00015064)
#define AUX_MU_BAUD_REG (PHYSADDR_OFFSET + 0x00015068)

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

void uart_init (void)
{
    unsigned int ra;

    PUT32(AUX_ENABLES, 1);
    PUT32(AUX_MU_IER_REG, 0);
    PUT32(AUX_MU_CNTL_REG, 0);
    PUT32(AUX_MU_LCR_REG, 3);
    PUT32(AUX_MU_MCR_REG, 0);
    PUT32(AUX_MU_IER_REG, 0);
    PUT32(AUX_MU_IIR_REG, 0xC6);
    PUT32(AUX_MU_BAUD_REG, 270);
    ra = GET32(GPFSEL1);
    ra &= ~(7 << 12);   //gpio14
    ra |= 2 << 12;      //alt5
    ra &= ~(7 << 15);   //gpio15
    ra |= 2 << 15;      //alt5
    PUT32(GPFSEL1, ra);
    PUT32(GPPUD, 0);
    for (ra = 0; ra < 150; ra++)
    {
        dummy(ra);
    }
    PUT32(GPPUDCLK0, (1 << 14) | (1 << 15));
    for (ra = 0; ra < 150; ra++)
    {
        dummy(ra);
    }
    PUT32(GPPUDCLK0, 0);
    PUT32(AUX_MU_CNTL_REG, 3);
}

unsigned int uart_lcr (void)
{
    return (GET32(AUX_MU_LSR_REG));
}

unsigned int uart_recv(void)
{
    while (1)
    {
        if (uart_lcr() & 0x01)
        {
            break;
        }
    }

    return (GET32(AUX_MU_IO_REG) & 0xFF);
}

void uart_send (unsigned int c)
{
    while (1)
    {
        if (uart_lcr() & 0x20)
        {
            break;
        }
    }
    PUT32(AUX_MU_IO_REG, c);
}

void uart_flush(void)
{
    while (1)
    {
        if ((uart_lcr() & 0x100) == 0)
        {
            break;
        }
    }
}