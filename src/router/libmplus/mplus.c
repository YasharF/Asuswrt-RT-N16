#include <math.h>

long long int llrint(double x)
{
    return rint(x);
}

long int lrint(double x)
{
    return rint(x);
}

long int lrintf(float x)
{
    return (int)(rint(x));
}

double round(double x)
{
    return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}

float roundf(float x)
{
    return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}

long int lroundf(float x)
{
    return (x > 0) ? floor(x + 0.5) : ceil(x - 0.5);
}

float truncf(float x)
{
    return (x > 0) ? floor(x) : ceil(x);
}

double log2(double x)
{
    return log(x) * 1.44269504088896340736;
}

float log2f(float x)
{
    return log(x) * 1.44269504088896340736;
}

