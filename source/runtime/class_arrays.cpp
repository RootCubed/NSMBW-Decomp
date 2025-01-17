#include <types.h>
#include <runtime/class_arrays.hpp>
/// @file

extern "C" {

/**
 * @brief Allocates and constructs an array of objects.
 * @details The allocated memory must be @p size * @p n + 0x10.
 * @details [Calls to this function are generated as part of the @p new[] macro].
 *
 * @param pBlock A pointer to the allocated memory, or @p nullptr .
 * @param pCtor A pointer to the default constructor, or @p nullptr .
 * @param pDtor A pointer to the default destructor, or @p nullptr .
 * @param size The size of one array element.
 * @param n The number of array elements.
 * @return A pointer to the first array element.
 */
void *__construct_new_array(void *pBlock, ctorPtr pCtor, dtorPtr pDtor, u32 size, u32 n) {
    char *ptr = (char *)pBlock;
    if (ptr != nullptr) {
        u32 *p = (u32 *)ptr;
        p[0] = size;
        p[1] = n;
        ptr += 16;

        if (pCtor != nullptr) {
            __partial_array_destructor pad(ptr, size, n, pDtor);

            char *p = (char *)ptr;
            for (pad.mCurrNum = 0; pad.mCurrNum < n; p += size) {
                pCtor(p, 1);
                pad.mCurrNum++;
            }
        }
    }

    return ptr;
}

/**
 * @brief Constructs an array of objects.
 *
 * @param pBlock A pointer to the array memory.
 * @param pCtor A pointer to the default constructor.
 * @param pDtor A pointer to the default destructor, or @p nullptr .
 * @param size The size of one array element.
 * @param n The number of array elements.
 */
void __construct_array(void *pBlock, ctorPtr pCtor, dtorPtr pDtor, u32 size, u32 n) {
    __partial_array_destructor pad(pBlock, size, n, pDtor);

    char *p = (char *)pBlock;
    for (pad.mCurrNum = 0; pad.mCurrNum < n; p += size) {
        pCtor(p, 1);
        pad.mCurrNum++;
    }
}

/**
 * @brief Destroys an array of objects.
 *
 * @param pBlock A pointer to the array memory.
 * @param pDtor A pointer to the default destructor.
 * @param size The size of one array element.
 * @param n The number of array elements.
 */
void __destroy_arr(void *pBlock, dtorPtr pDtor, u32 size, u32 n) {
    for (char *p = (char *)pBlock + size * n; n > 0; n--) {
        p -= size;
        pDtor(p, -1);
    }
}

/**
 * @brief Destroys and deletes an array of objects.
 * @details [Calls to this function are generated as part of the @p delete[] macro].
 *
 * @param pBlock A pointer to the allocated memory, or @p nullptr .
 * @param pDtor A pointer to the default destructor, or @p nullptr .
 */
void __destroy_new_array(void *pBlock, dtorPtr pDtor) {
    if (pBlock != nullptr) {
        if (pDtor != nullptr) {
            u32 i, objects, objectsize;
            objectsize = ((u32 *) ((char *) pBlock - 16))[0];
            objects = ((u32 *) ((char *) pBlock - 16))[1];

            char *p = (char *) pBlock + objectsize * objects;
            for (i = 0; i < objects; i++) {
                p -= objectsize;
                pDtor(p, -1);
            }
        }

        delete[] ((char *)pBlock - 16);
    }
}

} // extern "C"
