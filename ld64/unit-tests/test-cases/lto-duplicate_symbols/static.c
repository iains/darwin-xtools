static int same_name(int i)
{
    return i + 10;
}

int 
other (void)
{
    return same_name(100);
}
