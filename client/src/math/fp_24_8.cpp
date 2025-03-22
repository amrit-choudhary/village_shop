#include "fp_24_8.h"

using namespace VG::Math;

FP_24_8::FP_24_8() { rawValue = 0; }

FP_24_8::FP_24_8(int32_t val) { rawValue = static_cast<int32_t>(val * fractionMult); }

FP_24_8::FP_24_8(int32_t val, bool asRaw) { rawValue = val; }

FP_24_8::FP_24_8(float val) {
    if (val >= 0.0f) {
        rawValue = static_cast<int32_t>(val * fractionMult + 0.5f);
    } else {
        rawValue = static_cast<int32_t>(val * fractionMult - 0.5f);
    }
}

FP_24_8::FP_24_8(double val) {
    if (val >= 0.0f) {
        rawValue = static_cast<int32_t>(val * fractionMult + 0.5f);
    } else {
        rawValue = static_cast<int32_t>(val * fractionMult - 0.5f);
    }
}

FP_24_8::FP_24_8(const FP_24_8& val) { rawValue = val.rawValue; }

int32_t FP_24_8::GetRaw() const { return rawValue; }

int32_t FP_24_8::ToInt() const { return rawValue / fractionMult; }

float FP_24_8::ToFloat() const { return static_cast<float>(rawValue) / fractionMult; }

double FP_24_8::ToDouble() const { return static_cast<double>(rawValue) / fractionMult; }

FP_24_8 VG::Math::FP_24_8::FromRawValue(int32_t val) { return FP_24_8(val, true); }

FP_24_8 VG::Math::FP_24_8::operator-() const { return FP_24_8::FromRawValue(-rawValue); }

FP_24_8& FP_24_8::operator+=(const FP_24_8& b) {
    rawValue += b.rawValue;
    return *this;
}

FP_24_8& FP_24_8::operator-=(const FP_24_8& b) {
    rawValue -= b.rawValue;
    return *this;
}

FP_24_8& FP_24_8::operator*=(const FP_24_8& b) {
    int64_t val = (static_cast<int64_t>(rawValue) * b.rawValue) / (fractionMult / 2);
    rawValue = static_cast<int32_t>((val / 2) + (val % 2));
    return *this;
}

FP_24_8& FP_24_8::operator/=(const FP_24_8& b) {
    int64_t val = (static_cast<int64_t>(rawValue) * fractionMult * 2) / b.rawValue;
    rawValue = static_cast<int32_t>((val / 2) + (val % 2));
    return *this;
}

FP_24_8 VG::Math::operator+(const FP_24_8& a, const FP_24_8& b) { return VG::Math::FP_24_8(a) += b; }

FP_24_8 VG::Math::operator-(const FP_24_8& a, const FP_24_8& b) { return FP_24_8(a) -= b; }

FP_24_8 VG::Math::operator*(const FP_24_8& a, const FP_24_8& b) { return FP_24_8(a) *= b; }

FP_24_8 VG::Math::operator/(const FP_24_8& a, const FP_24_8& b) { return FP_24_8(a) /= b; }

bool VG::Math::operator==(const FP_24_8& a, const FP_24_8& b) { return a.GetRaw() == b.GetRaw(); }

bool VG::Math::operator!=(const FP_24_8& a, const FP_24_8& b) { return a.GetRaw() != b.GetRaw(); }

bool VG::Math::operator<(const FP_24_8& a, const FP_24_8& b) { return a.GetRaw() < b.GetRaw(); }

bool VG::Math::operator>(const FP_24_8& a, const FP_24_8& b) { return a.GetRaw() > b.GetRaw(); }

bool VG::Math::operator<=(const FP_24_8& a, const FP_24_8& b) { return a.GetRaw() <= b.GetRaw(); }

bool VG::Math::operator>=(const FP_24_8& a, const FP_24_8& b) { return a.GetRaw() >= b.GetRaw(); }
