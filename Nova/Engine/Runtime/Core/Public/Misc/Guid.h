#pragma once

struct FGuid
{
    int32 A;
    int32 B;
    int32 C;
    int32 D;

    FGuid() : A(0), B(0), C(0), D(0)
    {

    }

    bool operator ==(FGuid other)
    {
        return A == other.A && B == other.B && C == other.C && D == other.D;
    }

    bool operator !=(FGuid other)
    {
        return A != other.A || B != other.B || C != other.C || D != other.D;
    }
};