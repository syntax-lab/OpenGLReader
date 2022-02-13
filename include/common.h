#pragma once

#ifndef COMMON_H
    #define COMMON_H
    #define DEBUG_BUILD 0
    //NOTE: constants used to initialize application content
    #define SKY_COLOR 0.5, 0.5f, 0.75f

    //NOTE: helper defines used to facilitate implementation
    #define COORDS_OFFSET(n) (void*)((n) * sizeof(float))
#endif /* COMMON_H */
