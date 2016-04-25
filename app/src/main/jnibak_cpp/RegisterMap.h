//
// Created by Mono on 4/1/16.
//

#ifndef BOX_REGISTERMAP_H
#define BOX_REGISTERMAP_H

#include "common.h"

/*
 * This is a single variable-size structure.  It may be allocated on the
 * heap or mapped out of a (post-dexopt) DEX file.
 *
 * 32-bit alignment of the structure is NOT guaranteed.  This makes it a
 * little awkward to deal with as a structure; to avoid accidents we use
 * only byte types.  Multi-byte values are little-endian.
 *
 * Size of (format==FormatNone): 1 byte
 * Size of (format==FormatCompact8): 4 + (1 + regWidth) * numEntries
 * Size of (format==FormatCompact16): 4 + (2 + regWidth) * numEntries
 */
struct RegisterMap {
    /* header */
    u1      format;         /* enum RegisterMapFormat; MUST be first entry */
    u1      regWidth;       /* bytes per register line, 1+ */
    u1      numEntries[2];  /* number of entries */

    /* raw data starts here; need not be aligned */
    u1      data[1];
};
#endif //BOX_REGISTERMAP_H
