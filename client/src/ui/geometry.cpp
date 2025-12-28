#include "geometry.h"

ME::Geometry::Geometry() {}

ME::Geometry::~Geometry() {}

ME::Vec2 ME::Geometry::GetPosition() const {
    return position;
}

void ME::Geometry::SetPosition(const Vec2& pos) {
    position = pos;
}

ME::Vec2 ME::Geometry::GetAbsolutePosition() const {
    return absolutePosition;
}

void ME::Geometry::SetAbsolutePosition(const Vec2& absPos) {
    absolutePosition = absPos;
}

ME::Vec2 ME::Geometry::GetSize() const {
    return size;
}

void ME::Geometry::SetSize(const Vec2& sz) {
    size = sz;
}

ME::Vec2 ME::Geometry::GetAbsoluteSize() const {
    return absoluteSize;
}

void ME::Geometry::SetAbsoluteSize(const Vec2& absSz) {
    absoluteSize = absSz;
}
