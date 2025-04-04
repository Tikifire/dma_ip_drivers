Xilinx Frame Buffer Read driver

source code here -> embeddedsw/XilinxProcessorIPLib/drivers/v_frmbuf_rd

the init codes have two version: with Linux(xv_frmbufrd_linux.c) and without Linux(xv_frmbufrd_sinit.c), search macro "__linux__" for more detail (mainly regarding XV_frmbufrd_Initialize)

the purpose is to init frmbuf_rd on baremetal

## init process

start: xv_frmbufrd_example.c/main

all drivers are initiated in xv_frmbufrd_example.c/DriverInit

the init function for frmbuf is xv_frmbufrd_l2.c/XVFrmbufRd_Initialize

xv_frmbufrd_l2.c/XV_frmbufrd_Initialize

xv_frmbufrd.c/XV_frmbufrd_CfgInitialize

xv_frmbufrd_l2.c/SetPowerOnDefaultState (set default state, the real state is set in XVFrmbufRd_SetMemFormat below)
1920*1080@60, RGBX8
XV_frmbufrd_Set_HwReg_width -> 0x10
XV_frmbufrd_Set_HwReg_height -> 0x18
XV_frmbufrd_Set_HwReg_stride -> 0x20
XV_frmbufrd_Set_HwReg_video_format -> 0x28
XV_frmbufrd_Set_HwReg_field_id -> 0x48

xv_frmbufrd_l2.c/XVFrmbufRd_InterruptDisable

xv_frmbufrd.c/XV_frmbufrd_InterruptDisable

xv_frmbufrd.c/XV_frmbufrd_InterruptGlobalDisable

xv_frmbufrd.c/XV_frmbufrd_EnableAutoRestart
XV_frmbufrd_WriteReg(InstancePtr->Config.BaseAddress, XV_FRMBUFRD_CTRL_ADDR_AP_CTRL, 0x80);
-> write 0x80 into 0x00

after that we get back to DriverInit and leave

xv_frmbufrd_example.c/SetupInterrupts

xv_frmbufrd_example.c/Xil_ExceptionEnable
seemed for processor

xv_frmbufrd_example.c/XVFrmbufRd_SetCallback

xv_frmbufrd_example.c/CalcStride + ConfigFrmbuf

in CalcStride
it calculates stride first then set it
stride = ((((width * bpp_numerator) / bpp_denominator) +
    MMWidthBytes - 1) / MMWidthBytes) * MMWidthBytes;
width = HActive = 1920
bpp_numerator = 3 for RGB8
bpp_denominator = 1 for RGB8
MMWidthBytes = AXIMMDataWidth/8 = 64/8 = 8
stride = 5760


in ConfigFrmbuf

XVFrmbufRd_Stop

XVFrmbufRd_WaitForIdle

XVFrmbufRd_SetMemFormat
XV_frmbufrd_Set_HwReg_width
XV_frmbufrd_Set_HwReg_height
XV_frmbufrd_Set_HwReg_stride
XV_frmbufrd_Set_HwReg_video_format -> write 0x14(RGB8) into 0x28

XVFrmbufRd_SetBufferAddr (address is 64bit)
V_frmbufrd_WriteReg(InstancePtr->Config.BaseAddress, XV_FRMBUFRD_CTRL_ADDR_HWREG_FRM_BUFFER_V_DATA, (u32)(Data)); -> 0x30
XV_frmbufrd_WriteReg(InstancePtr->Config.BaseAddress, XV_FRMBUFRD_CTRL_ADDR_HWREG_FRM_BUFFER_V_DATA + 4, (u32)(Data >> 32)); -> 0x34

XVFrmbufRd_InterruptEnable

XVFrmbufRd_Start
Data = XV_frmbufrd_ReadReg(InstancePtr->Config.BaseAddress, XV_FRMBUFRD_CTRL_ADDR_AP_CTRL) & 0x80; -> 0x00 (preserve auto_restart bit)
XV_frmbufrd_WriteReg(InstancePtr->Config.BaseAddress, XV_FRMBUFRD_CTRL_ADDR_AP_CTRL, Data | 0x01); -> 0x00






finish

XilinxProcessorIPLib/drivers/v_frmbuf_rd/









