#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""generate a test vi file"""

import struct
from warnings import filterwarnings as filter_warnings


def main() -> int:
    """Entry/main function"""

    with open("test.vi", "wb") as f:
        print(f"generating {f.name!r}")

        f.write(
            b"vi"  # magic (in all reality ignored, just needs to be 2 bytes)
            + struct.pack("<I", 30)  # fps
            + struct.pack("<I", 100)  # width
            + struct.pack("<I", 100)  # height
            + bytearray((255, 255, 255) * 100 * 100 * 60)  # 60 100x100 frames
        )

    return 0


if __name__ == "__main__":
    assert main.__annotations__.get("return") is int, "main() should return an integer"

    filter_warnings("error", category=Warning)
    raise SystemExit(main())
