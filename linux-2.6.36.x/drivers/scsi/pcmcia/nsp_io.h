/*
  NinjaSCSI I/O funtions 
      By: YOKOTA Hiroshi <yokota@netlab.is.tsukuba.ac.jp>
 
  This software may be used and distributed according to the terms of
  the GNU General Public License.

  */

/* $Id: //WIFI_SOC/MP/SDK_5_0_0_0/RT288x_SDK/source/linux-2.6.36.x/drivers/scsi/pcmcia/nsp_io.h#1 $ */

#ifndef __NSP_IO_H__
#define __NSP_IO_H__

static inline          void nsp_write(unsigned int base,
				      unsigned int index,
				      unsigned char val);
static inline unsigned char nsp_read(unsigned int base,
				     unsigned int index);
static inline          void nsp_index_write(unsigned int BaseAddr,
					    unsigned int Register,
					    unsigned char Value);
static inline unsigned char nsp_index_read(unsigned int BaseAddr,
					   unsigned int Register);

/*******************************************************************
 * Basic IO
 */

static inline void nsp_write(unsigned int  base,
			     unsigned int  index,
			     unsigned char val)
{
	outb(val, (base + index));
}

static inline unsigned char nsp_read(unsigned int base,
				     unsigned int index)
{
	return inb(base + index);
}


/**********************************************************************
 * Indexed IO
 */
static inline unsigned char nsp_index_read(unsigned int BaseAddr,
					   unsigned int Register)
{
	outb(Register, BaseAddr + INDEXREG);
	return inb(BaseAddr + DATAREG);
}

static inline void nsp_index_write(unsigned int  BaseAddr,
				   unsigned int  Register,
				   unsigned char Value)
{
	outb(Register, BaseAddr + INDEXREG);
	outb(Value, BaseAddr + DATAREG);
}

/*********************************************************************
 * fifo func
 */

/* read 8 bit FIFO */
static inline void nsp_multi_read_1(unsigned int   BaseAddr,
				    unsigned int   Register,
				    void          *buf,
				    unsigned long  count)
{
	insb(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo8_read(unsigned int   base,
				  void          *buf,
				  unsigned long  count)
{
	/*nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx", buf, count);*/
	nsp_multi_read_1(base, FIFODATA, buf, count);
}

/*--------------------------------------------------------------*/

/* read 16 bit FIFO */
static inline void nsp_multi_read_2(unsigned int   BaseAddr,
				    unsigned int   Register,
				    void          *buf,
				    unsigned long  count)
{
	insw(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo16_read(unsigned int   base,
				   void          *buf,
				   unsigned long  count)
{
	//nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx*2", buf, count);
	nsp_multi_read_2(base, FIFODATA, buf, count);
}

/*--------------------------------------------------------------*/

/* read 32bit FIFO */
static inline void nsp_multi_read_4(unsigned int   BaseAddr,
				    unsigned int   Register,
				    void          *buf,
				    unsigned long  count)
{
	insl(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo32_read(unsigned int   base,
				   void          *buf,
				   unsigned long  count)
{
	//nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx*4", buf, count);
	nsp_multi_read_4(base, FIFODATA, buf, count);
}

/*----------------------------------------------------------*/

/* write 8bit FIFO */
static inline void nsp_multi_write_1(unsigned int   BaseAddr,
				     unsigned int   Register,
				     void          *buf,
				     unsigned long  count)
{
	outsb(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo8_write(unsigned int   base,
				   void          *buf,
				   unsigned long  count)
{
	nsp_multi_write_1(base, FIFODATA, buf, count);
}

/*---------------------------------------------------------*/

/* write 16bit FIFO */
static inline void nsp_multi_write_2(unsigned int   BaseAddr,
				     unsigned int   Register,
				     void          *buf,
				     unsigned long  count)
{
	outsw(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo16_write(unsigned int   base,
				    void          *buf,
				    unsigned long  count)
{
	nsp_multi_write_2(base, FIFODATA, buf, count);
}

/*---------------------------------------------------------*/

/* write 32bit FIFO */
static inline void nsp_multi_write_4(unsigned int   BaseAddr,
				     unsigned int   Register,
				     void          *buf,
				     unsigned long  count)
{
	outsl(BaseAddr + Register, buf, count);
}

static inline void nsp_fifo32_write(unsigned int   base,
				    void          *buf,
				    unsigned long  count)
{
	nsp_multi_write_4(base, FIFODATA, buf, count);
}


/*====================================================================*/

static inline void nsp_mmio_write(unsigned long base,
				  unsigned int  index,
				  unsigned char val)
{
	unsigned char *ptr = (unsigned char *)(base + NSP_MMIO_OFFSET + index);

	writeb(val, ptr);
}

static inline unsigned char nsp_mmio_read(unsigned long base,
					  unsigned int  index)
{
	unsigned char *ptr = (unsigned char *)(base + NSP_MMIO_OFFSET + index);

	return readb(ptr);
}

/*-----------*/

static inline unsigned char nsp_mmio_index_read(unsigned long base,
						unsigned int  reg)
{
	unsigned char *index_ptr = (unsigned char *)(base + NSP_MMIO_OFFSET + INDEXREG);
	unsigned char *data_ptr  = (unsigned char *)(base + NSP_MMIO_OFFSET + DATAREG);

	writeb((unsigned char)reg, index_ptr);
	return readb(data_ptr);
}

static inline void nsp_mmio_index_write(unsigned long base,
					unsigned int  reg,
					unsigned char val)
{
	unsigned char *index_ptr = (unsigned char *)(base + NSP_MMIO_OFFSET + INDEXREG);
	unsigned char *data_ptr  = (unsigned char *)(base + NSP_MMIO_OFFSET + DATAREG);

	writeb((unsigned char)reg, index_ptr);
	writeb(val,                data_ptr);
}

/* read 32bit FIFO */
static inline void nsp_mmio_multi_read_4(unsigned long  base,
					 unsigned int   Register,
					 void          *buf,
					 unsigned long  count)
{
	unsigned long *ptr = (unsigned long *)(base + Register);
	unsigned long *tmp = (unsigned long *)buf;
	int i;

	//nsp_dbg(NSP_DEBUG_DATA_IO, "base 0x%0lx ptr 0x%p",base,ptr);

	for (i = 0; i < count; i++) {
		*tmp = readl(ptr);
		//nsp_dbg(NSP_DEBUG_DATA_IO, "<%d,%p,%p,%lx>", i, ptr, tmp, *tmp);
		tmp++;
	}
}

static inline void nsp_mmio_fifo32_read(unsigned int   base,
					void          *buf,
					unsigned long  count)
{
	//nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx*4", buf, count);
	nsp_mmio_multi_read_4(base, FIFODATA, buf, count);
}

static inline void nsp_mmio_multi_write_4(unsigned long  base,
					  unsigned int   Register,
					  void          *buf,
					  unsigned long  count)
{
	unsigned long *ptr = (unsigned long *)(base + Register);
	unsigned long *tmp = (unsigned long *)buf;
	int i;

	//nsp_dbg(NSP_DEBUG_DATA_IO, "base 0x%0lx ptr 0x%p",base,ptr);

	for (i = 0; i < count; i++) {
		writel(*tmp, ptr);
		//nsp_dbg(NSP_DEBUG_DATA_IO, "<%d,%p,%p,%lx>", i, ptr, tmp, *tmp);
		tmp++;
	}
}

static inline void nsp_mmio_fifo32_write(unsigned int   base,
					 void          *buf,
					 unsigned long  count)
{
	//nsp_dbg(NSP_DEBUG_DATA_IO, "buf=0x%p, count=0x%lx*4", buf, count);
	nsp_mmio_multi_write_4(base, FIFODATA, buf, count);
}



#endif
/* end */
