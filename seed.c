#include "includes.h"

int randint(int minvalue, int maxvalue)
{
    return rand() % (maxvalue+1-minvalue) + minvalue;
}

unsigned long GetSeed()
{
    //All this is just to guarantee better seed variety without going through an implementation of an already existing RNG engine
    char username[30];
    strcpy(username, getenv("USERNAME"));
    char *ptr;
    unsigned int hash = strtoul(username, &ptr, 36);
    unsigned int programStart = time(NULL);
    unsigned int programNow;
    unsigned long seed = time(NULL);

    //Stage 1
    if (seed % 3 == 0)
        seed += (time(NULL) * hash);
    else
        seed -= (time(NULL) * hash);

    //Stage 2
    unsigned int nbuffer, sum = 0, remainder;
    nbuffer = seed;

    while (nbuffer != 0)
    {
        remainder = nbuffer % 10;
        sum += remainder;
        nbuffer /= 10;
    }
    programNow = time(NULL);

    if (sum % 2 == 0)
    {
        if (seed -= pow(sum, 2) > 0)
            seed -= pow(sum, 2);
        else
            seed += pow(sum, 2);
        seed += 7;  //There is no "Why 7 specifically", I just thought, why not ?
    }
    else
    {
        if ((programNow - programStart) % 3 != 0)
        {
            seed += pow((hash / (programNow - programStart)), 2);
        }
        else
        {
            seed -= hash;
        }
    }

    return seed;
}
