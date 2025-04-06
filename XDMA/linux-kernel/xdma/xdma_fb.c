
#define pr_fmt(fmt) KBUILD_MODNAME ":%s:%d: " fmt, __func__, __LINE__

#include <linux/fb.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/types.h>

#include "xdma_fb.h"
#include "xdma_mod.h"
#include "cdev_sgdma.h"

// https://github.com/sudipm-mukherjee/fbset
// see /etc/fb.modes for more
static const unsigned fb_width = 1920;
static const unsigned fb_height = 1080;
static const unsigned fb_fps = 60;
static const unsigned fb_pixel_size_byte = 3;

static int xdma_hardware_init(struct xdma_pci_dev *xpdev) {
    // how to setup /dev/xdma0_user
    // we need xcdev of xdma0_user
    // just from xpdev
    struct xdma_cdev *xcdev = &xpdev->user_cdev;
    struct xdma_dev *xdev = xcdev->xdev;

    u32 val;
    int rv;

    void __iomem *reg;

    // get the physical addr of control bar
    reg = xdev->bar[xcdev->bar];

    // width
    val = 1920;
    iowrite16(val, reg + 0x10);
    // height
    val = 1080;
    iowrite16(val, reg + 0x18);
    // stride
    val = 5760;
    iowrite32(val, reg + 0x20);

    // RGB888
    val = 20;
    iowrite8(val, reg + 0x28);

    // field
    // val = ?;
    // iowrite8(val, reg + 0x48);

    // set autostart
    val = 0x80;
    iowrite8(val, reg + 0x00);

    // set addr
    val = 0x0;
    iowrite32(val, reg + 0x30);
    val = 0x0;
    iowrite32(val, reg + 0x34);

    // set autostart and start
    val = 0x81;
    iowrite8(val, reg + 0x00);

    // init video timing controller
    // control register
    // value from document
    val = 0x01ffff07;
    iowrite32(val, reg + 0x20000);

    pr_info("xdma_hardware_init done\n");

    return 0;
}

static int xdma_fb_open(struct fb_info *info, int user) {
    pr_err("xdma_fb_open unimplemented\n");
    return 0;
}

static int xdma_fb_release(struct fb_info *info, int user) {
    pr_err("xdma_fb_release unimplemented\n");
    return 0;
}

static ssize_t xdma_fb_read(struct fb_info *info, char __user *buf, size_t count, loff_t *ppos) {
    pr_err("xdma_fb_read unimplemented\n");
    return -EINVAL;
    // return fb_io_read(info, buf, count, ppos);
}

static ssize_t xdma_fb_write(struct fb_info *info, const char __user *buf, size_t count, loff_t *ppos) {
    pr_err("xdma_fb_write unimplemented\n");
    return -EINVAL;
    // return fb_io_write(info, buf, count, ppos);
}

static int xdma_fb_ioctl(struct fb_info *info, unsigned int cmd, unsigned long arg) {
    pr_err("xdma_fb_ioctl unimplemented\n");
    return -EINVAL;
    // return 
}

static int xdma_fb_mmap(struct fb_info *info, struct vm_area_struct *vma) {
    struct xdma_pci_dev *xpdev = info->par;
    struct xdma_cdev *xcdev = &xpdev->bypass_cdev_base;
    struct xdma_dev *xdev = xcdev->xdev;

	unsigned long off;
	resource_size_t phys;
	unsigned long vsize;
	resource_size_t psize;
	int rv;

	off = vma->vm_pgoff << PAGE_SHIFT;
	/* BAR physical address */
	phys = pci_resource_start(xdev->pdev, xcdev->bar) + off;
	vsize = vma->vm_end - vma->vm_start;
	/* complete resource */
	psize = pci_resource_end(xdev->pdev, xcdev->bar) -
		pci_resource_start(xdev->pdev, xcdev->bar) + 1 - off;

	dbg_sg("mmap(): xcdev = 0x%08lx\n", (unsigned long)xcdev);
	dbg_sg("mmap(): cdev->bar = %d\n", xcdev->bar);
	dbg_sg("mmap(): xdev = 0x%p\n", xdev);
	dbg_sg("mmap(): pci_dev = 0x%08lx\n", (unsigned long)xdev->pdev);
	dbg_sg("off = 0x%lx, vsize 0x%lu, psize 0x%llu.\n", off, vsize, psize);
	dbg_sg("start = 0x%llx\n",
		(unsigned long long)pci_resource_start(xdev->pdev,
		xcdev->bar));
	dbg_sg("phys = 0x%llx\n", phys);

	if (vsize > psize)
		return -EINVAL;
	/*
	 * pages must not be cached as this would result in cache line sized
	 * accesses to the end point
	 */
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	/*
	 * prevent touching the pages (byte access) for swap-in,
	 * and prevent the pages from being swapped out
	 */
    vm_flags_set(vma, VMEM_FLAGS);
	/* make MMIO accessible to user space */
	rv = io_remap_pfn_range(vma, vma->vm_start, phys >> PAGE_SHIFT,
			vsize, vma->vm_page_prot);
	dbg_sg("vma=0x%p, vma->vm_start=0x%lx, phys=0x%llx, size=%lu = %d\n",
		vma, vma->vm_start, phys >> PAGE_SHIFT, vsize, rv);

	if (rv)
		return -EAGAIN;
	return 0;

    return fb_io_mmap(info, vma);
}

static void xdma_fb_destroy(struct fb_info *info) {
    pr_err("xdma_fb_destroy unimplemented\n");
}

static const struct fb_ops xdma_fb_ops = {
    .owner = THIS_MODULE,
    .fb_open = xdma_fb_open,
    .fb_release = xdma_fb_release,
    .fb_read = xdma_fb_read,
    .fb_write = xdma_fb_write,
    // .fb_set_par = ,
    // .fb_setcolreg = ,
    // .fb_fillrect = ,
    // .fb_copyarea = ,
    // .fb_imageblit = ,
    // .fb_sync = ,
    .fb_ioctl = xdma_fb_ioctl,
    .fb_mmap = xdma_fb_mmap,
    // .fb_get_caps = ,
    .fb_destroy = xdma_fb_destroy,
};

static struct fb_var_screeninfo xdma_fb_var = {
    .xres = fb_width,
    .yres = fb_height,
    .xres_virtual = fb_width,
    .yres_virtual = fb_height,
    .xoffset = 0,
    .yoffset = 0,
    .bits_per_pixel = fb_pixel_size_byte * 8,
    .grayscale = 0,
    .red = {16, 8},
    .green = {8, 8},
    .blue = {0, 8},
    .transp = {0, 0}, // {24, 8}
    .nonstd = 0,
    .activate = 0,
    .height = -1,
    .width = -1,
    .pixclock = 6734, // pixel clock in ps
    .left_margin = 148, // HBP
    .right_margin = 88, // HFP
    .upper_margin = 36, // VBP
    .lower_margin = 4, // VFP
    .hsync_len = 44, // 
    .vsync_len = 5, //
    .sync = 0,
    .vmode = FB_VMODE_NONINTERLACED,
    .rotate = 0,
};

static struct fb_fix_screeninfo xdma_fb_fix = {
    .id = "XDMA FB",
    // .smem_start = ,
    .smem_len = fb_width * fb_height * fb_pixel_size_byte,
    .type = FB_TYPE_PACKED_PIXELS,
    .visual = FB_VISUAL_TRUECOLOR,
    .xpanstep = 0,
    .ypanstep = 0,
    .ywrapstep = 0,
    .line_length = fb_width * fb_pixel_size_byte, // now 1920 * 3, can be modified
    // .mmio_start = ,
    .mmio_len = fb_width * fb_height * fb_pixel_size_byte,
    .accel = FB_ACCEL_NONE,
};

int xdma_register_framebuffer(struct pci_dev *pdev) {
    // i need to store and load pdev, xcdev, xpdev... into info

    int ret, err;
    struct xdma_pci_dev *xpdev = dev_get_drvdata(&pdev->dev);
    struct fb_info *info;


    info = framebuffer_alloc(0, &pdev->dev);
    if (!info) {
        err = -ENOMEM;
        return err;
    }

    info->fbops = &xdma_fb_ops;
    info->var = xdma_fb_var;
    info->fix = xdma_fb_fix;

    // info->node
    // info->flags
    // info->monspecs
    // info->pixmap
    // info->sprite
    // info->cmap
    // info->modelist
    // info->mode
    // info->device
    // info->class_flag

    // use the address of bar2
    pr_info("1\n");
    struct xdma_cdev *xcdev = &xpdev->bypass_cdev_base;
    pr_info("2\n");
    struct xdma_dev *xdev = xcdev->xdev;
    if (!xdev) pr_info("xdev null!!!\n");
    pr_info("3\n");
    struct xdma_engine *engine = xcdev->engine;
    if (!engine) pr_info("engine null!!!\n");
    pr_info("4\n");
    void __iomem *bypass_addr = xdev->bar[xdev->bypass_bar_idx];
    pr_info("5\n");
    pr_info("%s\n", engine->name);
    // bypass_addr = (void __iomem *)((u32 __iomem *)bypass_addr + engine->bypass_offset);
    pr_info("6\n");
    //info->screen_base = bypass_addr;
    pr_info("screen_base: %p\n", bypass_addr);

    // info->screen_buffer
    info->screen_size = fb_width * fb_height * fb_pixel_size_byte;
    // info->pseudo_palette
    // info->state
    // info->fbcon_par
    info->par = xpdev;

    ret = register_framebuffer(info);
    if (ret < 0) {
        pr_err("Unable to register framebuffer\n");
        return ret;
    }

    // initiate hardware for hdmi output
    xdma_hardware_init(xpdev);

    // pr_err("unimplemented\n");
    return 0;

    // return -EINVAL;
}

void xdma_unregister_framebuffer(struct pci_dev *pdev) {
    pr_err("unimplemented\n");
}
