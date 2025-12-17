/* file:          enc28j60.c
 * description:   Microchip ENC28J60 MAC/Ethernet network driver
 */

#include <ucx.h>
#include <device.h>
#include <enc28j60.h>


static struct device_s *sel_dev = 0;

/* low level SPI bus access */

static uint8_t enc28j60_readop(uint8_t op, uint8_t address)
{
	uint8_t data, cmd;
	struct eth_enc28j60_config_s *config;
	
	config = (struct eth_enc28j60_config_s *)sel_dev->config;

	if (dev_open(config->spi_dev, 0) < 0)
		return 0;
	
	// issue read command
	cmd = op | (address & ADDR_MASK);
	dev_write(config->spi_dev, &cmd, 1);
	// read data
	dev_read(config->spi_dev, &data, 1);
	// do dummy read if needed (for mac and mii, see datasheet page 29)
	if (address & 0x80)
		dev_read(config->spi_dev, &cmd, 1);
	dev_close(config->spi_dev);

	return data;
}

static void enc28j60_writeop(uint8_t op, uint8_t address, uint8_t data)
{
	uint8_t cmd;
	struct eth_enc28j60_config_s *config;
	
	config = (struct eth_enc28j60_config_s *)sel_dev->config;

	if (dev_open(config->spi_dev, 0) < 0)
		return;
	
	// issue write command
	cmd = op | (address & ADDR_MASK);
	dev_write(config->spi_dev, &cmd, 1);
	// write data
	dev_write(config->spi_dev, &data, 1);
	dev_close(config->spi_dev);
}

static void enc28j60_readbuf(uint8_t *data, uint16_t len)
{
	uint8_t cmd;
	struct eth_enc28j60_config_s *config;
	
	config = (struct eth_enc28j60_config_s *)sel_dev->config;

	if (dev_open(config->spi_dev, 0) < 0)
		return;

	// issue read command
	cmd = ENC28J60_READ_BUF_MEM;
	dev_write(config->spi_dev, &cmd, 1);
	dev_read(config->spi_dev, data, len);
	dev_close(config->spi_dev);
}

static void enc28j60_writebuf(uint8_t *data, uint16_t len)
{
	uint8_t cmd;
	struct eth_enc28j60_config_s *config;
	
	config = (struct eth_enc28j60_config_s *)sel_dev->config;

	if (dev_open(config->spi_dev, 0) < 0)
		return;

	// issue write command
	cmd = ENC28J60_WRITE_BUF_MEM;
	dev_write(config->spi_dev, &cmd, 1);
	dev_write(config->spi_dev, data, len);
	dev_close(config->spi_dev);
}


/* register read and write operations, PHY control */

static void enc28j60_setbank(uint8_t address)
{
	static uint8_t encbank = 0;
	
	if ((address & BANK_MASK) != encbank){
		// set the bank
		enc28j60_writeop(ENC28J60_BIT_FIELD_CLR, ECON1, (ECON1_BSEL1|ECON1_BSEL0));
		enc28j60_writeop(ENC28J60_BIT_FIELD_SET, ECON1, (address & BANK_MASK) >> 5);
		encbank = (address & BANK_MASK);
	}
}

static uint8_t enc28j60_read(uint8_t address)
{
	uint8_t tmp;

	// set the bank
	enc28j60_setbank(address);
	// do the read
	tmp = enc28j60_readop(ENC28J60_READ_CTRL_REG, address);

	return tmp;
}

static void enc28j60_write(uint8_t address, uint8_t data)
{
	// set the bank
	enc28j60_setbank(address);
	// do the write
	enc28j60_writeop(ENC28J60_WRITE_CTRL_REG, address, data);
}

static void enc28j60_phywrite(uint8_t address, uint16_t data)
{
	uint16_t i = 0;
	// set the PHY register address
	enc28j60_write(MIREGADR, address);
	// write the PHY data
	enc28j60_write(MIWRL, data);
	enc28j60_write(MIWRH, data >> 8);
	// wait until the PHY write completes
	while (enc28j60_read(MISTAT) & MISTAT_BUSY){
		_delay_us(50);
		if (i++ > 1000) break;
	}
}

static uint16_t enc28j60_phyread(uint8_t address)
{
	uint16_t data, i = 0;

	// set the PHY register address
	enc28j60_write(MIREGADR, address);
	// read the PHY data
	enc28j60_write(MICMD, MICMD_MIIRD);
	// wait until the PHY read completes
	while (enc28j60_read(MISTAT) & MISTAT_BUSY){
		_delay_us(50);
		if (i++ > 1000) break;
	}
	// clear command register
	enc28j60_write(MICMD, 0x00);
	data = enc28j60_read(MIRDL);
	data |= enc28j60_read(MIRDH);

	return data;
}


/* helper functions */

/*
static void enc28j60_powerdown(void) {
	enc28j60_writeop(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_RXEN);
	_delay_ms(50);
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_VRPS);
	_delay_ms(50);
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PWRSV);
}

static void enc28j60_powerup(void) {
	enc28j60_writeop(ENC28J60_BIT_FIELD_CLR, ECON2, ECON2_PWRSV);
	_delay_ms(50);
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
	_delay_ms(50);
}
*/


/* device initialization and frame transmission/reception */

static int enc28j60_id(void)
{
	uint16_t phyid1, phyid2;
	uint8_t rev;
	
	/* check chip id */
	phyid1 = enc28j60_phyread(PHHID1);
	phyid2 = enc28j60_phyread(PHHID2) << 8;
	rev = enc28j60_read(EREVID);

	if (!(rev > 0 && rev < 255))
		return -1;

	printf("ENC28J60: phy 0x%x, revision %d\n", phyid2 | phyid1, rev);
	
	return 0;
}

static int enc28j60_linkup(void)
{
	if (!((ntohs(enc28j60_phyread(PHSTAT2)) >> 8) & 4))
		return 0;
		
	return 1;
}

static int enc28j60_init(void)
{
	uint8_t rev;
	struct eth_enc28j60_config_s *config;
	
	config = (struct eth_enc28j60_config_s *)sel_dev->config;

	// perform system reset (see Rev. B4 Silicon Errata point 2)
	enc28j60_writeop(ENC28J60_SOFT_RESET, 0, ENC28J60_SOFT_RESET);
	_delay_ms(2);
	
	/* check chip revision */
	rev = enc28j60_read(EREVID);

	if (!(rev > 0 && rev < 255))
		return -1;

	// do bank 0 stuff
	// initialize receive buffer
	// 16-bit transfers, must write low byte first
	// Rx start
	enc28j60_write(ERXSTL, RXSTART_INIT & 0xFF);
	enc28j60_write(ERXSTH, RXSTART_INIT >> 8);
	// set receive pointer address
	enc28j60_write(ERXRDPTL, RXSTART_INIT & 0xFF);
	enc28j60_write(ERXRDPTH, RXSTART_INIT >> 8);
	// RX end
	enc28j60_write(ERXNDL, RXSTOP_INIT & 0xFF);
	enc28j60_write(ERXNDH, RXSTOP_INIT >> 8);
	// TX start
	enc28j60_write(ETXSTL, TXSTART_INIT & 0xFF);
	enc28j60_write(ETXSTH, TXSTART_INIT >> 8);
	// TX end
	enc28j60_write(ETXNDL, TXSTOP_INIT & 0xFF);
	enc28j60_write(ETXNDH, TXSTOP_INIT >> 8);

	// do bank 1 stuff, packet filter:
	// For broadcast packets we allow only ARP packets
	// All other packets should be unicast only for our mac (MAADR)
	//
	// The pattern to match on is therefore
	// Type     ETH.DST
	// ARP      BROADCAST
	// 06 08 -- ff ff ff ff ff ff -> ip checksum for theses bytes=f7f9
	// in binary these poitions are:11 0000 0011 1111
	// This is hex 303F->EPMM0=0x3f,EPMM1=0x30
	// Unfortunately, broadcasts are needed for BOOTP.
/*	enc28j60_write(ERXFCON, ERXFCON_UCEN | ERXFCON_CRCEN | ERXFCON_PMEN);
	enc28j60_write(EPMM0, 0x3f);
	enc28j60_write(EPMM1, 0x30);
	enc28j60_write(EPMCSL, 0xf9);
	enc28j60_write(EPMCSH, 0xf7);
*/
	// set promiscuous mode
	enc28j60_write(ERXFCON, 0);

	// do bank 2 stuff
	// enable MAC receive
	enc28j60_write(MACON1, MACON1_MARXEN | MACON1_TXPAUS | MACON1_RXPAUS);
	// bring MAC out of reset
	enc28j60_write(MACON2, 0x00);
	// enable automatic padding to 60bytes and CRC operations
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN);
	// set inter-frame gap (non-back-to-back)
	enc28j60_write(MAIPGL, 0x12);
	enc28j60_write(MAIPGH, 0x0C);
	// set inter-frame gap (back-to-back)
	enc28j60_write(MABBIPG, 0x12);
	// Set the maximum packet size which the controller will accept
	// Do not send packets longer than MAX_FRAMELEN:
	enc28j60_write(MAMXFLL, MAX_FRAMELEN & 0xFF);
	enc28j60_write(MAMXFLH, MAX_FRAMELEN >> 8);

	// do bank 3 stuff
	// write MAC address
	// NOTE: MAC address in ENC28J60 is byte-backward
	enc28j60_write(MAADR5, config->mac_addr[0]);
	enc28j60_write(MAADR4, config->mac_addr[1]);
	enc28j60_write(MAADR3, config->mac_addr[2]);
	enc28j60_write(MAADR2, config->mac_addr[3]);
	enc28j60_write(MAADR1, config->mac_addr[4]);
	enc28j60_write(MAADR0, config->mac_addr[5]);

	// no loopback of transmitted frames
	enc28j60_phywrite(PHCON2, PHCON2_HDLDIS);
	// switch to bank 0
	enc28j60_setbank(ECON1);
	// enable interrupts
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, EIE, EIE_INTIE | EIE_PKTIE);
	// enable packet reception
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_RXEN);
	
	return rev;
}

static int enc28j60_receive(uint8_t *frame, uint16_t size)
{
	static uint16_t encpktptr = RXSTART_INIT;
	uint16_t rxstat;
	uint16_t length;
	struct eth_enc28j60_data_s *data;
	
	data = (struct eth_enc28j60_data_s *)sel_dev->data;

	// RX not enabled, reset
	if (!(enc28j60_read(ECON1) & ECON1_RXEN)) {
		if (data->mode)
			printf("ENC28J60: reinit\n");
			
		enc28j60_init();
	}

	// check if a frame has been received
	if (enc28j60_read(EPKTCNT) == 0)
		return 0;

	// Set the read pointer to the start of the received frame
	enc28j60_write(ERDPTL, (encpktptr));
	enc28j60_write(ERDPTH, (encpktptr) >> 8);

	// read the next frame pointer
	encpktptr = enc28j60_readop(ENC28J60_READ_BUF_MEM, 0);
	encpktptr |= enc28j60_readop(ENC28J60_READ_BUF_MEM, 0) << 8;

	// read frame length (see datasheet page 43)
	length = enc28j60_readop(ENC28J60_READ_BUF_MEM, 0);
	length |= enc28j60_readop(ENC28J60_READ_BUF_MEM, 0) << 8;
	//remove the CRC count
	length -= 4;
	if (length > size) length = 0;
	// read the receive status (see datasheet page 43)
	rxstat = enc28j60_readop(ENC28J60_READ_BUF_MEM, 0);
	rxstat |= enc28j60_readop(ENC28J60_READ_BUF_MEM, 0) << 8;

	// check CRC and symbol errors (see datasheet page 44, table 7-3):
	// The ERXFCON.CRCEN is set by default. Normally we should not
	// need to check this.
	if ((rxstat & 0x80) == 0)	// invalid	TODO: check this logic (0 or not) on the 3rd link
		length = 0;
	else				// copy frame from the receive buffer
		enc28j60_readbuf(frame, length);

	// Move the RX read pointer to the start of the next received frame, freeing memory
	enc28j60_write(ERXRDPTL, (encpktptr));
	enc28j60_write(ERXRDPTH, (encpktptr) >> 8);
	// decrement the packet counter indicates we are done with this frame
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, ECON2, ECON2_PKTDEC);

	return length;
}

static int enc28j60_transmit(uint8_t *frame, uint16_t size)
{
	uint16_t timeout = 50;
	
	// Set the write pointer to start of transmit buffer area
	enc28j60_write(EWRPTL, TXSTART_INIT & 0xFF);
	enc28j60_write(EWRPTH, TXSTART_INIT >> 8);

	// Set the TXND pointer to correspond to the frame size given
	enc28j60_write(ETXNDL, (TXSTART_INIT + size) & 0xFF);
	enc28j60_write(ETXNDH, (TXSTART_INIT + size) >> 8);

	// write per-packet control byte (0x00 means use macon3 settings)
	enc28j60_writeop(ENC28J60_WRITE_BUF_MEM, 0, 0x00);

	// copy the packet into the transmit buffer
	enc28j60_writebuf(frame, size);

	// Reset the transmit logic. See Rev. B4 Silicon Errata points 12 and 13.
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRST);
	enc28j60_writeop(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRST);
	enc28j60_writeop(ENC28J60_BIT_FIELD_CLR, EIR, EIR_TXERIF | EIR_TXIF);

	// send the contents of the transmit buffer onto the network
	enc28j60_writeop(ENC28J60_BIT_FIELD_SET, ECON1, ECON1_TXRTS);
	
	// wait for packet transmission
	while ((enc28j60_read(EIR) & (EIR_TXIF | EIR_TXERIF)) == 0) {
		_delay_ms(1);
		if (--timeout == 0) break;
	}

	enc28j60_writeop(ENC28J60_BIT_FIELD_CLR, ECON1, ECON1_TXRTS);
	
	return size;
}


/* device driver function implementation */
static int eth_init(const struct device_s *dev)
{
	struct eth_enc28j60_data_s *pdata;
	
	printf("ENC28J60: device init\n");
	
	NOSCHED_ENTER();
	pdata = (struct eth_enc28j60_data_s *)dev->data;
	pdata->mode = 0;
	pdata->busy = 1;
	sel_dev = (struct device_s *)dev;
	pdata->init = enc28j60_init();
	if (pdata->init > -1)
		enc28j60_id();
	NOSCHED_LEAVE();
	_delay_ms(100);
	NOSCHED_ENTER();
	pdata->busy = 0;
	if (enc28j60_linkup())
		printf("ENC28J60: link up\n");
	else
		printf("ENC28J60: link down\n");
	sel_dev = 0;
	NOSCHED_LEAVE();

	return pdata->init;
}

static int eth_open(const struct device_s *dev, int mode)
{
	struct eth_enc28j60_data_s *pdata;
	int retval = 0;

	NOSCHED_ENTER();
	pdata = (struct eth_enc28j60_data_s *)dev->data;

	pdata->mode = mode;

	if (!pdata->busy && pdata->init >= 0 && sel_dev == 0) {
		pdata->busy = 1;
		sel_dev = (struct device_s *)dev;
		if (pdata->mode)
			printf("ENC28J60: device open\n");
	} else {
		if (pdata->mode)
			printf("ENC28J60: device open failed\n");

		retval = -1;
	}
	NOSCHED_LEAVE();

	return retval;
}

static int eth_close(const struct device_s *dev)
{
	struct eth_enc28j60_data_s *pdata;
	int retval = 0;

	NOSCHED_ENTER();
	pdata = (struct eth_enc28j60_data_s *)dev->data;

	if (pdata->busy && sel_dev != 0) {
		if (pdata->mode)
			printf("ENC28J60: device close\n");
		
		pdata->mode = 0;
		pdata->busy = 0;
		sel_dev = 0;
	} else {
		if (pdata->mode)
			printf("ENC28J60: device close failed\n");
		
		retval = -1;
	}
	NOSCHED_LEAVE();

	return retval;
}

static size_t eth_read(const struct device_s *dev, void *buf, size_t count)
{
	int val;

	val = enc28j60_receive(buf, count);
	ucx_task_yield();
	
	return val;
}

static size_t eth_write(const struct device_s *dev, void *buf, size_t count)
{
	int val;

	val = enc28j60_transmit(buf, count);
	ucx_task_yield();
	
	return val;
}

/* device driver function mapping for generic API */
const struct device_api_s eth_enc28j60_api = {
	.dev_init = eth_init,
	.dev_open = eth_open,
	.dev_close = eth_close,
	.dev_read = eth_read,
	.dev_write = eth_write
};
