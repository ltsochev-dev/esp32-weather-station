#ifndef LTSOCHEV_BOX_H
#define LTSOCHEV_BOX_H

struct box {
    unsigned short x, y, w, h;
    unsigned short background = 0xFFFF;
};

#endif