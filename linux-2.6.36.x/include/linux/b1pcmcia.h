/* $Id: //WIFI_SOC/MP/SDK_5_0_0_0/RT288x_SDK/source/linux-2.6.36.x/include/linux/b1pcmcia.h#1 $
 *
 * Exported functions of module b1pcmcia to be called by
 * avm_cs card services module.
 *
 * Copyright 1999 by Carsten Paeth (calle@calle.in-berlin.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef _B1PCMCIA_H_
#define _B1PCMCIA_H_

int b1pcmcia_addcard_b1(unsigned int port, unsigned irq);
int b1pcmcia_addcard_m1(unsigned int port, unsigned irq);
int b1pcmcia_addcard_m2(unsigned int port, unsigned irq);
int b1pcmcia_delcard(unsigned int port, unsigned irq);

#endif	/* _B1PCMCIA_H_ */
