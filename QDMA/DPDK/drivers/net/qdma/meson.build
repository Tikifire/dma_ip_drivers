#   BSD LICENSE
#
#   Copyright (c) 2021-2022 Xilinx, Inc. All rights reserved.
#   Copyright (c) 2022-2024, Advanced Micro Devices, Inc. All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#     * Neither the name of the copyright holder nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cflags += ['-DRTE_LIBRTE_QDMA_PMD']
cflags += ['-DDMA_BRAM_SIZE=524288']
cflags += ['-DTHROUGHPUT_MEASUREMENT']

# Use QDMA_DPDK_23_11 compiler flag for DPDK v23.11
# Use QDMA_DPDK_22_11 compiler flag for DPDK v22.11
# Use QDMA_DPDK_21_11 compiler flag for DPDK v21.11
# Use QDMA_DPDK_20_11 compiler flag for DPDK v20.11
cflags += ['-DQDMA_DPDK_23_11']

includes += include_directories('.')
includes += include_directories('qdma_access')
includes += include_directories('qdma_access/qdma_soft_access')
includes += include_directories('qdma_access/eqdma_soft_access')
includes += include_directories('qdma_access/qdma_cpm4_access')
includes += include_directories('qdma_access/eqdma_cpm5_access')

headers += files('rte_pmd_qdma.h')

deps += ['mempool_ring']

sources = files(
	'qdma_ethdev.c',
	'qdma_vf_ethdev.c',
	'qdma_devops.c',
	'qdma_common.c',
	'qdma_rxtx.c',
	'qdma_xdebug.c',
	'qdma_user.c',
	'qdma_access/eqdma_soft_access/eqdma_soft_access.c',
	'qdma_access/eqdma_soft_access/eqdma_soft_reg_dump.c',
	'qdma_access/qdma_cpm4_access/qdma_cpm4_access.c',
	'qdma_access/qdma_cpm4_access/qdma_cpm4_reg_dump.c',
	'qdma_access/eqdma_cpm5_access/eqdma_cpm5_access.c',
	'qdma_access/eqdma_cpm5_access/eqdma_cpm5_reg_dump.c',
	'qdma_access/qdma_soft_access/qdma_soft_access.c',
	'qdma_access/qdma_list.c',
	'qdma_access/qdma_resource_mgmt.c',
	'qdma_access/qdma_mbox_protocol.c',
	'qdma_access/qdma_access_common.c',
	'qdma_mbox.c',
	'qdma_platform.c',
	'rte_pmd_qdma.c',
	'qdma_dpdk_compat.c'
)

if arch_subdir == 'x86'
    sources += files('qdma_rxtx_vec_sse.c')
endif
