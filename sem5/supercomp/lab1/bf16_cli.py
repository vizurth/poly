import struct
import sys


def float_to_bf16_bits(value: float) -> int:
    try:
        float32_bits = struct.unpack(">I", struct.pack(">f", value))[0]
    except OverflowError as error:
        raise ValueError("число слишком большое") from error

    upper_half = float32_bits >> 16
    return (float32_bits + 0x7FFF + (upper_half & 1)) >> 16


def bf16_bits_to_float(bits: int) -> float:
    if not 0 <= bits <= 0xFFFF:
        raise ValueError("нужно число от 0x0000 до 0xFFFF")
    return struct.unpack(">f", struct.pack(">I", bits << 16))[0]


def parse_bf16(value: str) -> int:
    try:
        bits = int(value, 0)
    except ValueError as error:
        raise ValueError("нужно число вроде 0x4049") from error
    if not 0 <= bits <= 0xFFFF:
        raise ValueError("нужно число от 0x0000 до 0xFFFF")
    return bits


def error(text: str) -> None:
    print(text, file=sys.stderr)
    sys.exit(1)


def main() -> None:
    if len(sys.argv) != 3:
        error("нужно: python3 bf16_cli.py to-bf16 число")

    direction = sys.argv[1]
    value = sys.argv[2]

    if direction == "to-bf16":
        try:
            bits = float_to_bf16_bits(float(value))
        except ValueError as err:
            error(str(err))
        print(f"0x{bits:04X}")
        print(f"{bits:016b}")
    elif direction == "from-bf16":
        try:
            bits = parse_bf16(value)
        except ValueError as err:
            error(str(err))
        print(format(bf16_bits_to_float(bits), ".9g"))
        print(f"{bits:016b}")
    else:
        error("непонятное направление, пиши to-bf16 или from-bf16")


if __name__ == "__main__":
    main()
