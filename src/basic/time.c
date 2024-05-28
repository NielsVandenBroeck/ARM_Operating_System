void wait_msec(unsigned int n)
{
    register unsigned long f, t, r;

    // Get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // Read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // Calculate expire value for counter
    t+=((f/1000)*n);
    do{asm volatile ("mrs %0, cntpct_el0" : "=r"(r));}while(r<t);
}

void start_timer(int* start_time)
{
    // Read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(*start_time));
}

int elapsed_time(int start_time)
{
    register unsigned long f, t;
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));

    int elapsed_milliseconds = (((t - start_time) * 1000) / f);
    return elapsed_milliseconds;
}
