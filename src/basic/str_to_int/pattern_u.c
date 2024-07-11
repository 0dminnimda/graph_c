S2I_Result str_to_u@SIZE@(str *string, u@SIZE@ *value) {
    if (string->length == 0) return S2I_NOT_FOUND;
    char c = string->data[0];

    u8 msd;
    if ('0' <= c && c <= '9') {
        msd = c - '0';
    } else {
        return S2I_NOT_FOUND;
    }

    u@SIZE@ msd_multiplier = 1;
    u@SIZE@ rest = 0;
    u8 count = 1;
    for (; count < @MAX_DIGITS@ && count < string->length; ++count) {
        c = string->data[count];
        if ('0' <= c && c <= '9') {
            msd_multiplier *= 10;
            rest = rest * 10 + (c - '0');
        } else {
            break;
        }
    }

    if (count != @MAX_DIGITS@) {
        *value = msd * msd_multiplier + rest;
        string->data += count;
        string->length -= count;
        return S2I_OK;
    }

    if (count < string->length) {
        c = string->data[count];
        if ('0' <= c && c <= '9') {
            /* Got more than max digits, definitely not in range. */
            return S2I_OUT_OF_RANGE;
        }
    }

    if (msd > @MSD_MAX_VALUE@) {
        /* Most significant digit value exceeded it's limit */
        return S2I_OUT_OF_RANGE;
    }
    msd_multiplier *= msd;

    if (cheked_u_add(U@SIZE@, value, msd_multiplier, rest)) {
        return S2I_OUT_OF_RANGE;
    }
    string->data += count;
    string->length -= count;
    return S2I_OK;
}
