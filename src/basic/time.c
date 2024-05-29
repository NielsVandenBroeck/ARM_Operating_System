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
    // Get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // Read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // Calculate elapsed time by extracting starttime
    int elapsed_milliseconds = (((t - start_time) * 1000) / f);
    return elapsed_milliseconds;
}

int uptime_msec()
{
    register unsigned long f, t;
    // Get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // Read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // Calculate uptime in milliseconds
    int uptime_milliseconds = (t * 1000) / f;
    return uptime_milliseconds;
}

int random(unsigned int range) {
    static int seed = 0;

    // If the seed is zero, initialize it with the current time
    if (seed == 0) {
        seed = uptime_msec();
    }

    const unsigned int a = 1664525;
    const unsigned int c = 1013904223;
    const unsigned int m = 0xFFFFFFFF;

    seed = (a * seed + c) % m;

    return seed % range;
}