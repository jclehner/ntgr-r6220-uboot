/*
 * $Id: //WIFI_SOC/MP/SDK_5_0_0_0/RT288x_SDK/source/linux-2.6.36.x/drivers/staging/cxt1e1/sbecom_inline_linux.h#1 $
 */

#ifndef _INC_SBECOM_INLNX_H_
#define _INC_SBECOM_INLNX_H_

/*-----------------------------------------------------------------------------
 * sbecom_inline_linux.h - SBE common Linux inlined routines
 *
 * Copyright (C) 2007  One Stop Systems, Inc.
 * Copyright (C) 2005  SBE, Inc.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 * For further information, contact via email: support@onestopsystems.com
 * One Stop Systems, Inc.  Escondido, California  U.S.A.
 *-----------------------------------------------------------------------------
 * RCS info:
 * RCS revision: $Revision: #1 $
 * Last changed on $Date: 2015/03/04 $
 * Changed by $Author: jianghua.qin $
 *-----------------------------------------------------------------------------
 * $Log: sbecom_inline_linux.h,v $
 * Revision 1.2  2007/08/15 22:51:35  rickd
 * Remove duplicate version.h entry.
 *
 * Revision 1.1  2007/08/15 22:50:29  rickd
 * Update linux/config for 2.6.18 and later.
 *
 * Revision 1.0  2005/09/28 00:10:09  rickd
 * Initial revision
 *
 *-----------------------------------------------------------------------------
 */


#if defined (__FreeBSD__) || defined (__NetBSD__)
#include <sys/types.h>
#else
#include <linux/types.h>
#include <linux/version.h>
#if defined(CONFIG_SMP) && ! defined(__SMP__)
#define __SMP__
#endif
#if defined(CONFIG_MODVERSIONS) && defined(MODULE) && ! defined(MODVERSIONS)
#define MODVERSIONS
#endif

#ifdef MODULE
#ifdef MODVERSIONS
#include <config/modversions.h>
#endif
#include <linux/module.h>
#endif
#endif

#include <linux/kernel.h>       /* resolves kmalloc references */
#include <linux/skbuff.h>       /* resolves skb references */
#include <linux/netdevice.h>    /* resolves dev_kree_skb_any */
#include <asm/byteorder.h>      /* resolves cpu_to_le32 */

#if 0

/*** PORT POINT WARNING
 ***
 *** Under Linux 2.6 it has been found that compiler is re-ordering
 *** in-lined pci_write_32() functions to the detrement of correct
 *** hardware setup.  Therefore, inlining of PCI accesses has been
 *** de-implemented, and subroutine calls have been implemented.
 ***/

static inline u_int32_t
pci_read_32 (u_int32_t *p)
{
#ifdef FLOW_DEBUG
    u_int32_t   v;

    FLUSH_PCI_READ ();
    v = le32_to_cpu (*p);
    if (log_level >= LOG_DEBUG)
        pr_info("pci_read : %x = %x\n", (u_int32_t) p, v);
    return v;
#else
                FLUSH_PCI_READ ();      /* */
    return le32_to_cpu (*p);
#endif
}

static inline void
pci_write_32 (u_int32_t *p, u_int32_t v)
{
#ifdef FLOW_DEBUG
    if (log_level >= LOG_DEBUG)
        pr_info("pci_write: %x = %x\n", (u_int32_t) p, v);
#endif
    *p = cpu_to_le32 (v);
    FLUSH_PCI_WRITE ();             /* This routine is called from routines
                                     * which do multiple register writes
                                     * which themselves need flushing between
                                     * writes in order to guarantee write
                                     * ordering.  It is less code-cumbersome
                                     * to flush here-in then to investigate
                                     * and code the many other register
                                     * writing routines. */
}
#else
/* forward reference */
u_int32_t   pci_read_32 (u_int32_t *p);
void        pci_write_32 (u_int32_t *p, u_int32_t v);

#endif


/*
 * system dependent callbacks
 */

/**********/
/* malloc */
/**********/

static inline void *
OS_kmalloc (size_t size)
{
    char       *ptr = kmalloc (size, GFP_KERNEL | GFP_DMA);

    if (ptr)
        memset (ptr, 0, size);
    return ptr;
}

static inline void
OS_kfree (void *x)
{
    kfree (x);
}


/****************/
/* memory token */
/****************/

static inline void *
OS_mem_token_alloc (size_t size)
{
    struct sk_buff *skb;

    skb = dev_alloc_skb (size);
    if (!skb)
    {
        //pr_warning("no mem in OS_mem_token_alloc !\n");
        return 0;
    }
    return skb;
}


static inline void
OS_mem_token_free (void *token)
{
    dev_kfree_skb_any (token);
}


static inline void
OS_mem_token_free_irq (void *token)
{
    dev_kfree_skb_irq (token);
}


static inline void *
OS_mem_token_data (void *token)
{
    return ((struct sk_buff *) token)->data;
}


static inline void *
OS_mem_token_next (void *token)
{
    return 0;
}


static inline int
OS_mem_token_len (void *token)
{
    return ((struct sk_buff *) token)->len;
}


static inline int
OS_mem_token_tlen (void *token)
{
    return ((struct sk_buff *) token)->len;
}


/***************************************/
/* virtual to physical addr conversion */
/***************************************/

static inline u_long
OS_phystov (void *addr)
{
    return (u_long) __va (addr);
}


static inline u_long
OS_vtophys (void *addr)
{
    return __pa (addr);
}


/**********/
/* semops */
/**********/

void        OS_sem_init (void *, int);


static inline void
OS_sem_free (void *sem)
{
    /*
     * NOOP - since semaphores structures predeclared w/in structures, no
     * longer malloc'd
     */
}

#define SD_SEM_TAKE(sem,desc)  down(sem)
#define SD_SEM_GIVE(sem)       up(sem)
#define SEM_AVAILABLE     1
#define SEM_TAKEN         0


/**********************/
/* watchdog functions */
/**********************/

struct watchdog
{
    struct timer_list h;
    struct work_struct work;
    void       *softc;
    void        (*func) (void *softc);
    int         ticks;
    int         init_tq;
};


static inline int
OS_start_watchdog (struct watchdog * wd)
{
    wd->h.expires = jiffies + wd->ticks;
    add_timer (&wd->h);
    return 0;
}


static inline int
OS_stop_watchdog (struct watchdog * wd)
{
    del_timer_sync (&wd->h);
    return 0;
}


static inline int
OS_free_watchdog (struct watchdog * wd)
{
    OS_stop_watchdog (wd);
    OS_kfree (wd);
    return 0;
}


/* sleep in microseconds */
void        OS_uwait (int usec, char *description);
void        OS_uwait_dummy (void);


/* watchdog functions */
int OS_init_watchdog(struct watchdog *wdp, void (*f) (void *), void *ci, int usec);


#endif                          /*** _INC_SBECOM_INLNX_H_ ***/
