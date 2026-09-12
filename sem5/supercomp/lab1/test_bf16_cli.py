from __future__ import annotations

import subprocess
import sys
from pathlib import Path
import unittest


SCRIPT = Path(__file__).with_name("bf16_cli.py")


class BFloat16CliTests(unittest.TestCase):
    def run_cli(self, *args: str) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [sys.executable, str(SCRIPT), *args],
            capture_output=True,
            text=True,
            check=False,
        )

    def test_to_bf16_converts_decimal_to_hex_bits(self) -> None:
        result = self.run_cli("to-bf16", "3.14159")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, "0x4049\n0100000001001001\n")

    def test_from_bf16_converts_hex_bits_to_float32_value(self) -> None:
        result = self.run_cli("from-bf16", "0x4049")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout, "3.140625\n0100000001001001\n")

    def test_rejects_unknown_direction(self) -> None:
        result = self.run_cli("sideways", "1")

        self.assertNotEqual(result.returncode, 0)
        self.assertIn("непонятное направление", result.stderr)

    def test_rejects_bad_bf16_value(self) -> None:
        result = self.run_cli("from-bf16", "abc")

        self.assertNotEqual(result.returncode, 0)
        self.assertEqual(result.stderr, "нужно число вроде 0x4049\n")


if __name__ == "__main__":
    unittest.main()
