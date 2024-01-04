long a[4] = {1, 2, 3, 4};

long sum(long *start, long count);

void main()
{
    long val = sum(a, 4);
}

long sum(long *start, long count)
{
    long sum = 0;
    while (count)
    {
        sum += *start;
        start++;
        count--;
    }
    return sum;
}