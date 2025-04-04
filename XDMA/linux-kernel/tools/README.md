
test 32M DMA
assume ddr memory at 0x80000000
''' bash
sudo ./dma_to_device -d /dev/xdma0_h2c_0 -f ../tests/data/datafile_32M.bin -s 33554432 -a 0x80000000 -c 1 -w ./test_32M
diff test_32M ../tests/data/datafile_32M.bin
'''

we can also use dma_from_device
''' bash
sudo ./dma_from_device -d /dev/xdma0_c2h_0 -s 33554432 -a 0x80000000 -c 1 -f ./test_32M
diff ./test_32M ../tests/data/datafile_32M.bin
'''

