#ifndef __XDMA_FRAMEBUFFER_H__
#define __XDMA_FRAMEBUFFER_H__

#include <linux/fb.h>
#include <linux/kernel.h>
#include <linux/pci.h>

int xdma_register_framebuffer(struct pci_dev *pdev);
void xdma_unregister_framebuffer(struct pci_dev *pdev);


#endif /* ifndef __XDMA_FRAMEBUFFER_H__ */
