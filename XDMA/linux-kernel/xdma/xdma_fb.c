
#include <linux/fb.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/types.h>

#include "xdma_fb.h"
#include "xdma_mod.h"
#include "cdev_sgdma.h"

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

    pr_info("xdma: xdma_hardware_init done\n");


    return 0;
}

static int xdma_fb_open(struct fb_info *info, int user) {
    return 0;
}

static int xdma_fb_release(struct fb_info *info, int user) {
    return 0;
}

static ssize_t xdma_fb_read(struct fb_info *info, char __user *buf, size_t count, loff_t *ppos) {
    return 0;
}

static ssize_t xdma_fb_write(struct fb_info *info, const char __user *buf, size_t count, loff_t *ppos) {
    return 0;
}

static int xdma_fb_mmap(struct fb_info *info, struct vm_area_struct *vma) {
    return 0;
}

static void xdma_fb_destroy(struct fb_info *info) {
    ;
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
    // .fb_ioctl = ,
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
    .red = {8, 16},
    .green = {8, 8},
    .blue = {8, 0},
    .transp = {0, 0}, // {8, 24}
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
    // .smem_len = ,
    .type = FB_TYPE_PACKED_PIXELS,
    .visual = FB_VISUAL_TRUECOLOR,
    .xpanstep = 0,
    .ypanstep = 0,
    .ywrapstep = 0,
    .line_length = fb_width * fb_pixel_size_byte, // now 1920 * 3, can be modified
    // .mmio_start = ,
    // .mmio_len = ,
    .accel = FB_ACCEL_NONE,
};

int xdma_register_framebuffer(struct pci_dev *pdev) {
    // i need to store and load pdev, xcdev, xpdev... into info

    struct xdma_pci_dev *xpdev = dev_get_drvdata(&pdev->dev);

    struct fb_info *info;

    int err;


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
    // info->screen_base
    // info->screen_buffer
    // info->screen_size
    // info->pseudo_palette
    // info->state
    // info->fbcon_par
    // info->par

    // initiate hardware for hdmi output
    xdma_hardware_init(xpdev);

    // pr_err("unimplemented\n");
    return 0;
}

void xdma_unregister_framebuffer(struct pci_dev *pdev) {
    pr_err("unimplemented\n");
}
