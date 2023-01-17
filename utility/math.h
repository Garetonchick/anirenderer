#pragma once

template<class T>
T Lerp(const T& val1, const T& val2, float lerp_amount) {
    return val1 + (val2 - val1) * lerp_amount;
}
