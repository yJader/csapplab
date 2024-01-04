long a[4] = {0xa000a000, 0x0b000b00, 0x00c000c0, 0x000d000d};

long rsum(long *start, long count);

void main()
{
    long val = rsum(a, 4);
}

long rsum(long *start, long count)
{
    if (count <= 0)
        return 0;
    return *start + rsum(start + 1, count - 1);
}