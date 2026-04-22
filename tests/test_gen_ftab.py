#!/usr/bin/env python3
#
# SPDX-FileCopyrightText: 2026 SiFli Technologies(Nanjing) Co., Ltd
#
# SPDX-License-Identifier: Apache-2.0

from __future__ import annotations

import copy
import os
import sys
import unittest


ROOT = os.path.realpath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, os.path.join(ROOT, "tools", "build"))

import gen_ftab  # noqa: E402
import ptab  # noqa: E402


class GenFtabPartitionEntryTests(unittest.TestCase):
    def _load_board(self, rel_path: str):
        ptab_obj = ptab.load_ptab(os.path.join(ROOT, rel_path), fatal=True)
        entries, _ = gen_ftab.build_partition_table_entries(
            ptab_obj.partitions,
            ptab_obj.get_chip_config(),
            ptab_obj.chip,
        )
        return ptab_obj, entries

    def _find_partition(self, ptab_obj, name: str) -> dict:
        for partition in ptab_obj.partitions:
            if str(partition.get("name") or "").strip() == name:
                return partition
        self.fail("Partition not found: {}".format(name))

    def _assert_exec_entry_uses_sbus(self, ptab_obj, entries, part_name: str, entry_index: int) -> None:
        partition = self._find_partition(ptab_obj, part_name)
        exec_def = partition.get("exec")
        self.assertIsInstance(exec_def, dict, "{} must define exec".format(part_name))

        exec_region = str(exec_def.get("region") or "").strip()
        exec_offset = ptab.parse_size(exec_def.get("offset", 0))
        exec_sbus, exec_cbus = ptab.resolve_region_address(
            exec_region,
            exec_offset,
            ptab_obj.get_chip_config(),
            core=partition.get("core"),
        )

        self.assertEqual(entries[entry_index][2], exec_sbus)
        self.assertNotEqual(exec_sbus, exec_cbus)

    def test_nor_exec_targets_use_sbus_in_ftab(self) -> None:
        ptab_obj, entries = self._load_board("customer/boards/sf32lb58-lcd_n16r64n4/ptab.yaml")

        self._assert_exec_entry_uses_sbus(
            ptab_obj,
            entries,
            "hcpu_flash_code",
            gen_ftab.PartitionIndex.HCPU_IMAGE,
        )
        self._assert_exec_entry_uses_sbus(
            ptab_obj,
            entries,
            "dfu_flash_code",
            gen_ftab.PartitionIndex.LCPU_IMAGE_PING,
        )

    def test_nand_exec_targets_keep_using_sbus_in_ftab(self) -> None:
        ptab_obj, entries = self._load_board("customer/boards/sf32lb56-lcd_a128r12n1/ptab.yaml")

        self._assert_exec_entry_uses_sbus(
            ptab_obj,
            entries,
            "hcpu_flash_code",
            gen_ftab.PartitionIndex.HCPU_IMAGE,
        )
        self._assert_exec_entry_uses_sbus(
            ptab_obj,
            entries,
            "dfu_flash_code",
            gen_ftab.PartitionIndex.LCPU_IMAGE_PING,
        )

    def test_xip_partition_without_exec_keeps_cbus(self) -> None:
        ptab_obj, entries = self._load_board("customer/boards/sf32lb56-lcd_n16r12n1/ptab.yaml")
        partition = self._find_partition(ptab_obj, "hcpu_flash_code")
        offset = ptab.parse_size(partition.get("offset", 0))
        sbus_addr, cbus_addr = ptab.resolve_region_address(
            partition.get("region", ""),
            offset,
            ptab_obj.get_chip_config(),
            core=partition.get("core"),
        )

        self.assertEqual(entries[gen_ftab.PartitionIndex.HCPU_IMAGE][2], cbus_addr)
        self.assertNotEqual(sbus_addr, cbus_addr)

    def test_bootloader_exec_uses_sbus(self) -> None:
        board_ptab = ptab.load_ptab(
            os.path.join(ROOT, "customer/boards/sf32lb58-lcd_n16r64n4/ptab.yaml"),
            fatal=True,
        )
        chip_config = board_ptab.get_chip_config()

        partitions = [
            {
                "name": "flash_table",
                "type": "ftab",
                "region": "mpi4",
                "offset": 0,
                "size": "32KB",
            },
            {
                "name": "bootloader",
                "type": "bootloader",
                "region": "mpi4",
                "offset": 0x00020000,
                "size": 0x00020000,
                "exec": {
                    "region": "psram1",
                    "offset": 0,
                },
                "core": "HCPU",
            },
        ]

        entries, _ = gen_ftab.build_partition_table_entries(
            partitions,
            chip_config,
            board_ptab.chip,
        )
        exec_sbus, exec_cbus = ptab.resolve_region_address("psram1", 0, chip_config, core="HCPU")

        self.assertEqual(entries[gen_ftab.PartitionIndex.BOOTLOADER][2], exec_sbus)
        self.assertEqual(entries[gen_ftab.PartitionIndex.BOOTLOADER_IMAGE_PONG][2], exec_sbus)
        self.assertNotEqual(exec_sbus, exec_cbus)

    def test_emmc_storage_uses_sdmmc_base_and_sbus_exec_target(self) -> None:
        chip_config = copy.deepcopy(ptab.load_chip_config("sf32lb58"))
        chip_config["memory_info"] = {
            "sdmmc1": {
                "type": "emmc",
                "size": 64 * 1024 * 1024,
                "sip": False,
            }
        }

        partitions = [
            {
                "name": "flash_table",
                "type": "ftab",
                "region": "sdmmc1",
                "offset": 0x1000,
                "size": "32KB",
            },
            {
                "name": "bootloader",
                "type": "bootloader",
                "region": "sdmmc1",
                "offset": 0x00011000,
                "size": 0x00020000,
                "exec": {
                    "region": "hpsys_ram",
                    "offset": 0x20000,
                },
                "core": "HCPU",
            },
            {
                "name": "hcpu_flash_code",
                "type": "app",
                "subtype": "factory",
                "region": "sdmmc1",
                "offset": 0x00401000,
                "size": 0x00200000,
                "exec": {
                    "region": "psram1",
                    "offset": 0,
                },
                "core": "HCPU",
            },
        ]

        entries, _ = gen_ftab.build_partition_table_entries(
            partitions,
            chip_config,
            "SF32LB586VDD36",
        )

        ftab_sbus, ftab_cbus = ptab.resolve_region_address("sdmmc1", 0x1000, chip_config)
        img_sbus, img_cbus = ptab.resolve_region_address("sdmmc1", 0x00401000, chip_config, core="HCPU")
        exec_sbus, exec_cbus = ptab.resolve_region_address("psram1", 0, chip_config, core="HCPU")

        self.assertEqual(ftab_sbus, 0x68001000)
        self.assertEqual(ftab_sbus, ftab_cbus)
        self.assertEqual(entries[gen_ftab.PartitionIndex.FLASH_PARTITION_TABLE][0], ftab_sbus)
        self.assertEqual(entries[gen_ftab.PartitionIndex.HCPU_IMAGE][0], img_sbus)
        self.assertEqual(img_sbus, img_cbus)
        self.assertEqual(entries[gen_ftab.PartitionIndex.HCPU_IMAGE][2], exec_sbus)
        self.assertNotEqual(exec_sbus, exec_cbus)


if __name__ == "__main__":
    unittest.main()
