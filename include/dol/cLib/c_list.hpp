#pragma once

#include <types.h>

// [Unofficial name]
/// @brief A doubly-linked list node.
/// @see cListMg_c
class cListNd_c {
public:
    cListNd_c *mpPrev; ///< The previous node.
    cListNd_c *mpNext; ///< The next node.

    /// @brief Constructs a new cListNd_c.
    cListNd_c() : mpPrev(nullptr), mpNext(nullptr) {}

    cListNd_c *getPrev() const { return mpPrev; } ///< Gets the previous node.
    cListNd_c *getNext() const { return mpNext; } ///< Gets the next node.
};

// [Unofficial name]
/// @brief A doubly-linked list container.
/// @see cListNd_c
class cListMg_c {
public:
    cListNd_c *mpFirst; ///< The first node in the list.
    cListNd_c *mpLast; ///< The last node in the list.

    /// @brief Constructs a new cListMg_c.
    cListMg_c() : mpFirst(nullptr), mpLast(nullptr) {}

    /**
     * @brief Inserts a node after the given previous node.
     *
     * @param node The node to insert.
     * @param prevNode The node to insert it after, or @p nullptr to insert it at the beginning.
     * @return If the operation was successful.
     */
    bool insertAfter(cListNd_c *node, cListNd_c *prevNode);

    /**
     * @brief Removes a node from the list.
     *
     * @param node The node to remove.
     * @return If the operation was successful.
     */
    bool remove(cListNd_c *node);

    /**
     * @brief Adds a node to the end of the list.
     *
     * @param node The node to append.
     * @return If the operation was successful.
     */
    bool append(cListNd_c *node);

    /**
     * @brief Adds a node to the beginning of the list.
     *
     * @param node The node to prepend.
     * @return If the operation was successful.
     */
    bool prepend(cListNd_c *node);

    cListNd_c *getFirst() const { return mpFirst; } ///< Gets the first node.
    cListNd_c *getLast() const { return mpLast; } ///< Gets the last node.
};
