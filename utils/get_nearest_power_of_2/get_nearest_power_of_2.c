#include<math.h>
#include<stdio.h>

/**
 * @brief Struct representing limits to be given to the binary search
 * algorithm, to search for nearest power of 2.
 * 
 */
typedef struct binary_search_input
{
    int lower;
    int upper;
} Limits;


/**
 * @brief Get the limits for power of 2 within which the 
 * input lies. We consider 10 integers at a time For 
 * example, for an input 29, we check that 29 is greater 
 * than 10, then greater than 20, then 30, where it fails, 
 * and then return 20 and 30 in the struct.
 * 
 * @param input The number to find the power of 2 near
 * @return struct binary_search_input The limits to search between
 */
struct binary_search_input getLimitsForPowerOf2(int input) {
    int power = 10;
    while (input > pow(2, power))
    {
        power+=10;
    }
    return (Limits){.lower = power - 10, .upper = power};
}

/**
 * @brief Given the limits, it searches for a value which is 
 * power of 2 and is closest to the input value.
 * 
 * @param limits Struct representing the limits to search within.
 * @param input The number to search near.
 * @return int The nearest power of 2, to the input.
 */
int binarySearchNearestPowerOf2(Limits limits, int input) {
    if (limits.lower == limits.upper)
    {
        return pow(2, limits.lower);
    }
    if ((limits.upper - limits.lower) == 1)
    {
        int valueForLowerPowerOf2 = pow(2, limits.lower);
        int valueForUpperPowerOf2 = pow(2, limits.upper);
        int differenceFromValueForLowerPowerOf2 = input - valueForLowerPowerOf2;
        int differenceFromValueForUpperPowerOf2 = valueForUpperPowerOf2 - input;
        return differenceFromValueForLowerPowerOf2 < differenceFromValueForUpperPowerOf2
            ? valueForLowerPowerOf2
            : valueForUpperPowerOf2;
    }
    
    int mid = (limits.lower + limits.upper)/2;
    if (pow(2, mid) > input)
    {
        limits.upper = mid;
    } else {
        limits.lower = mid;
    }
    return binarySearchNearestPowerOf2(limits, input);
}

/**
 * @brief Get the Nearest Power Of 2 to the input.
 * 
 * @param input The input number near which the power of 2 needs to be found.
 * @return int The power of 2 nearest to the input.
 */
int getNearestPowerOf2(int input) {
    if (input == 1)
    {
        return 2;
    }
    Limits limits = getLimitsForPowerOf2(input);
    return binarySearchNearestPowerOf2(limits, input);
}