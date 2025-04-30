#pragma once

/**
 * A linear, 32-bit/component floating point RGBA color.
 */
struct FLinearColor {
    float	R,
        G,
        B,
        A;
};

struct FColor {
    union { struct { uint8 B, G, R, A; }; uint32 AlignmentDummy; };
};