S2I_Result str_to_u##SIZE(const str *string, u##SIZE *value) {
    u8 digits[MAX_DIGITS];
    u8 count = 0;
    u##SIZE bigger = 0;
    u8 max_i = MAX_DIGITS;

    for (; count < MAX_DIGITS && count < string->length; ++count) {
        char c = string->data[count];
        if ('0' <= c && c <= '9') {
            digits[MAX_DIGITS - count - 1] = c - '0';
        } else {
            goto less_than_max_digits;
        }
    }
    if (count < string->length) {
        // Got more than max digits, definitely not in range.
        char c = string->data[count];
        if ('0' <= c && c <= '9') {
            return S2I_OUT_OF_RANGE;
        }
    }
    u8 msd = digits[MAX_DIGITS-1];
    if (msd > MSD_MAX_VALUE) {
        return S2I_OUT_OF_RANGE;
    }
    bigger = msd * MSD_MULTIPLIER;
    max_i = MAX_DIGITS - 1;

less_than_max_digits:
    if (count == 0) return S2I_NOT_FOUND;

    u64 smaller = 0;
    for (u8 i = MAX_DIGITS - count; i < max_i; ++i) {
        smaller = smaller * 10 + digits[i];
    }
    if (count != MAX_DIGITS) return S2I_OK;

    if (checked_u_add(U##SIZE, value, bigger, smaller)) {
        return S2I_OUT_OF_RANGE;
    }
    return S2I_OK;
}
