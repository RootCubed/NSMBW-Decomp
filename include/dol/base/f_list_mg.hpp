#pragma once

#include <types.h>
#include <dol/base/f_profile.hpp>
#include <dol/cLib/c_list.hpp>
#include <dol/base/f_base_id.hpp>
#include <dol/base/f_list_nd.hpp>

class fBase_c;

/// @brief A list of fLiNdBa_c nodes.
class fLiMgBa_c : public cListMg_c {
public:
    /**
     * @brief Count the number of nodes of a given profile in this list.
     * 
     * @param profName The profile name.
     * @return How many nodes were found.
     */
    int countNodeByProfName(ProfileName profName) const;
    
    /**
     * @brief Find a node in this list which has a certain ID.
     * 
     * @param id The ID to search for
     * @return The found node, or nullptr if none were found.
     */
    const fLiNdBa_c *searchNodeByID(fBaseID_e id) const;
    
    inline fLiNdBa_c *getFirst() const {
        return (fLiNdBa_c *) cListMg_c::getFirst();
    }
    inline fLiNdBa_c *getLast() const {
        return (fLiNdBa_c *) cListMg_c::getLast();
    }
};
