int gcd(int a, int b)
{
    return (a > b) ? a : b;
}

int main(void)
{
    int c = gcd(10, 3);
    c = c + 3;
    return 0;
}