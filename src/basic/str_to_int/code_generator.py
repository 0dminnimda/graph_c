from pathlib import Path


BIT_SIZES = [8, 16, 32, 64]

FILE = Path(__file__)
DIR = FILE.parent
PATTERN_UNSIGNED = DIR / "pattern_u.c"
PATTERN_SIGNED = DIR / "pattern_s.c"
PATTERN_WHOLE = DIR / "pattern.c"
RESULT = DIR.parent / "str_to_int.c"


def to_base(x: int, b: int) -> list[int]:
    out = []
    while x:
        x, r = divmod(x, b)
        out.append(r)
    return out[::-1]


def signed_range(bit_size: int) -> tuple[int, int]:
    if bit_size <= 0:
        return (0, 0)
    if bit_size == 1:
        return (0, 1)
    return (-(1 << (bit_size - 1)), (1 << (bit_size - 1)) - 1)


def unsigned_range(bit_size: int) -> tuple[int, int]:
    return (0, (1 << bit_size) - 1)


def format_pattern(pattern: str, data: dict[str, str]) -> str:
    for name, repl in data.items():
        name = f"@{name.upper()}@"
        pattern = pattern.replace(name, repl)
    return pattern


def generate_functions(signed: bool):
    if signed:
        pattern = PATTERN_SIGNED.read_text()
    else:
        pattern = PATTERN_UNSIGNED.read_text()

    for bit_size in BIT_SIZES:
        base = 10

        data = {}
        data["size"] = str(bit_size)

        if signed:
            mn, mx = signed_range(bit_size)
            min_digits = to_base(abs(mn), base)
            max_digits = to_base(mx, base)
            assert len(min_digits) == len(max_digits), (
                f"differing digit lengths for {bit_size}, {signed=} integer"
            )
            assert min_digits[0] == max_digits[0], (
                f"differing most significant digit for {bit_size}, {signed=} integer"
            )
            data["max_digits"] = str(len(max_digits))
            data["msd_max_value"] = str(max_digits[0])
        else:
            mn, mx = unsigned_range(bit_size)
            digits = to_base(mx, base)
            data["max_digits"] = str(len(digits))
            data["msd_max_value"] = str(digits[0])

        yield format_pattern(pattern, data)


def generate_main_file() -> str:
    pattern = PATTERN_WHOLE.read_text()

    data = {
        "s2u": "\n".join(generate_functions(signed=False)),
        "s2s": "\n".join(generate_functions(signed=True)),
        "self": FILE.relative_to(RESULT.parent).as_posix(),
    }

    return format_pattern(pattern, data)


if __name__ == "__main__":
    RESULT.write_text(generate_main_file())

