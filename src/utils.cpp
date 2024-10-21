#include "utils.h"

void reading(float *tabVal[],bool *errors)
{
    float measure, error;

    for (int i =0; i< sizeof(*tabVal); i++)
    {
        measure = 0;
    }
}

void addValue(float *oldTab, float newValue)
{
    for (int i =0; i<(sizeof(*oldTab) -1); i++)
    {
        if (oldTab[i] < -40)
        {
            oldTab[i] = newValue;
            return;
        }
        oldTab[i] = oldTab[i+1];
    }
    oldTab[sizeof(*oldTab)-1] = newValue;
}

int mean(float *tab)
{
    int mean = 0;
    for (int i=0; i <sizeof(*tab); i++)
    {
        if (tab[i] > -41)
        {
            mean += tab[i];
        }
    }
    return mean;
}
