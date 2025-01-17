#pragma once
#include <types.h>
#include <lib/egg/heap/frm_heap.hpp>
#include <dol/base/f_profile.hpp>
#include <dol/base/f_base_id.hpp>
#include <dol/base/f_helper_unk.hpp>
#include <dol/base/f_manager.hpp>
#include <dol/base/f_list_mg.hpp>

/// @brief The base class for all scenes, actors and various other processes.
/// @details A class that inherits from fBase_c is called a "base".
class fBase_c {
public:

    /// @brief The possible states in the lifecycle.
    enum LIFECYCLE_e {
        WAITING_FOR_CREATE,
        ACTIVE,
        TO_BE_DELETED
    };

    /// @brief The possible group types.
    enum GROUP_TYPE_e {
        OTHER, ///< [Value used by generic bases].
        SCENE, ///< [Set by all bases inheriting from dScene_c].
        ACTOR ///< [Set by all bases inheriting from dBaseActor_c].
    };

    /// @brief The states ::commonPack can be in after running the @e pre and @e do methods.
    enum MAIN_STATE_e {
        CANCELED, ///< The @e pre method failed.
        ERROR, ///< The @e do method failed.
        SUCCESS, ///< The @e do method succeeded.
        WAITING ///< The @e pre / @e do method needs to be run again [only applies to ::create].
    };

    /// @brief See ::commonPack.
    enum PACK_RESULT_e {
        NOT_READY, ///< Execution has to be repeated later (only for ::create).
        SUCCEEDED, ///< Execution succeeded.
        FAILED, ///< Execution failed.
    };

    fBaseID_e mUniqueID; ///< A unique identifier for every base.
    u32 mParam; ///< A bitfield that configures the base's behaviour. [Represents nybbles 5 to 12 of Reggie's spritedata].
    ProfileName mProfName; ///< The base's profile name.

    u8 mLifecycleState; ///< The current lifecycle state. Value is a ::LIFECYCLE_e.
    bool mDeleteRequested; ///< If the base is to be deleted.
    bool mDelayManageAdd; ///< If the adding of the base should be delayed until the next ::connectProc call.
    bool mRetryCreate; ///< If the next ::connectProc call should add the base to fManager_c::m_createManage.

    u8 mGroupType; ///< The base's group type. Value is a ::GROUP_TYPE_e.
    u8 mProcessFlags; ///< Which process is to be executed. Value is a bitfield induced by fManager_c::PROC_FLAGS.

    fManager_c mMng; ///< The base's process manager.

    fBaHelper_c *mpUnusedHelper; ///< [Unused].
    fLiMgBa_c mUnusedList; ///< [Unused].
    EGG::FrmHeap *mpHeap; ///< The base's own FrmHeap. [Unused].

    fBase_c(); ///< Constructs a new base.

    int createPack(); ///< See ::commonPack.
    int deletePack(); ///< See ::commonPack.
    int executePack(); ///< See ::commonPack.
    int drawPack(); ///< See ::commonPack.

    /**
     * @brief Deals with the connect tree and object lifecycle.
     *  @details Main tasks include:
     *  * Moving bases marked for deletion to the deletion list (and deleting its children);
     *  * Updating process flags;
     *  * Reordering the execute and draw list on priority changes;
     *  * Processing bases that deferred their addition to the manager lists;
     *
     *  The function always returns 1.
     */
    int connectProc();

    /// @brief Requests deletion of the base.
    void deleteRequest();

    fBase_c *getConnectParent(); ///< Gets the base's parent.
    fBase_c *getConnectChild(); ///< Gets the base's first child.
    fBase_c *getConnectBrNext(); ///< Gets the base's next sibling.

    /// @brief Checks if the base has a child waiting to be added to fManager_c::m_createManage.
    /// @return If such a child base exists.
    /// @note Unofficial name.
    bool hasNonReadyChild() const;

    /// @brief @p new operator override for all bases.
    /// @details Every base is allocated in mHeap::g_gameHeaps[0], and memory is cleared
    /// before construction.
    static void *operator new(size_t);
    static void operator delete(void *); ///< @p delete operator override for all bases.

    /**
     * @brief Creates a child base with the given parent.
     *
     * @param profName The base's profile name.
     * @param parent The base's parent.
     * @param param The base's parameters.
     * @param groupType The base's group type.
     * @return A pointer to the instantiated base, or @p nullptr .
     */
    static fBase_c *createChild(ProfileName profName, fBase_c *parent, unsigned long param, u8 groupType);

    /**
     * @brief Creates a base without a parent.
     *
     * @param profName The base's profile name.
     * @param param The base's parameters.
     * @param groupType The base's group type.
     * @return A pointer to the instantiated base, or @p nullptr .
     */
    static fBase_c *createRoot(ProfileName profName, unsigned long param, u8 groupType);

    static void (*sLoadAsyncCallback)(); ///< [Unused].
    static void (*sUnloadCallback)(); ///< [Unused].

private:

    /// @brief Attempts to finalize creation of the base.
    void runCreate();

    /// @brief Gets a child of the base that is waiting to be added to fManager_c::m_createManage.
    /// @return A child satisfying this condition, else @p nullptr .
    /// @note Unofficial name.
    fBase_c *getNonReadyChild() const;

    /// @brief Checks if a flag is set in ::mProcessFlags.
    bool isProcessFlag(u8 flag) const { return (mProcessFlags & flag) != 0; }

    /**
     * @brief Sets temporary data to be used for the next base's construction.
     *
     * @param profName The base's profile name.
     * @param connectParent The connect node of the base's parent, or @p nullptr .
     * @param param The base's parameters.
     * @param groupType The base's group type.
     */
    static void setTmpCtData(ProfileName profName, fTrNdBa_c *connectParent, unsigned long param, u8 groupType);

    /**
     * @brief Common code for the pack tasks.
     *
     * @param doFunc The main function for this task.
     * @param preFunc The function that is called before the main function.
     * @param postFunc The function that is called after the main function.
     * @return A ::PACK_RESULT_e value returned from doFunc, or preFunc if doFunc was not executed.
     */
    int commonPack(int (fBase_c::*doFunc)(), int (fBase_c::*preFunc)(), void (fBase_c::*postFunc)(MAIN_STATE_e));

    /// @brief Code to be executed on base creation.
    /// @details This function usually runs once and is used for initialization of
    /// settings and/or assets.
    /// @return A ::PACK_RESULT_e value.
    virtual int create();

    /// @brief Code to be executed before ::create.
    /// @details This function is usually overridden to check for pre-conditions
    /// before running ::create.
    /// @return A ::PACK_RESULT_e value.
    virtual int preCreate();

    /// @brief Code to be executed after ::create.
    /// @details Allows running different code based on the result of the previous two functions.
    /// For fBase_c, this function will:
    /// * Mark the base as created if creation was successful.
    /// * Request deletion of the base if creation resulted in an error.
    /// * Other states will not produce any result.
    virtual void postCreate(MAIN_STATE_e state);

    /// @brief Code to be executed on base deletion.
    /// @details This function usually runs once, and is used for disabling functionalities
    /// such as collision or for deallocating resources.
    /// @return A ::PACK_RESULT_e value.
    virtual int doDelete();

    /// @brief Code to be executed before ::doDelete.
    /// @details This function is usually overridden to check for pre-conditions
    /// before running ::doDelete.
    /// @return A ::PACK_RESULT_e value.
    virtual int preDelete();

    /// @brief Code to be executed after ::doDelete.
    /// @details Allows running different code based on the result of the previous two functions.
    /// For fBase_c, if deletion was successful, this function will:
    /// * Remove the base from the lists it's still in.
    /// * Delete the base's heap and ultimately the base itself.
    ///
    /// Other states will not produce any result.
    virtual void postDelete(MAIN_STATE_e state);

    /// @brief Contains the base's main logic.
    /// @details This function runs every frame and can be considered the base's own
    /// main loop.
    /// @return A ::PACK_RESULT_e value.
    virtual int execute();

    /// @brief Code to be executed before ::execute.
    /// @details This function is usually overridden to check for pre-conditions
    /// before running ::execute (such as checking zone boundaries or the game being paused).
    /// @return A ::PACK_RESULT_e value.
    virtual int preExecute();

    /// @brief Code to be executed after ::execute.
    /// @details Allows running different code based on the result of the previous two functions.
    virtual void postExecute(MAIN_STATE_e state);

    /// @brief Contains the base's drawing logic.
    /// @details This function runs every frame and contains code for drawing the base on screen
    /// (if the base doesn't need drawing, the function is empty).
    /// @return A ::PACK_RESULT_e value.
    virtual int draw();

    /// @brief Code to be executed before ::draw.
    /// @details This function is usually overridden to check for pre-conditions
    /// before running ::draw (such as checking if the base is effectively on screen).
    /// @return A ::PACK_RESULT_e value.
    virtual int preDraw();

    /// @brief Code to be executed after ::draw.
    /// @details Allows running different code based on the result of the previous two functions.
    virtual void postDraw(MAIN_STATE_e state);

    /// @brief Informs the base that it will be deleted.
    virtual void deleteReady();

    /**
     * @brief Tries to create a heap for the base [Unused].
     * @details Should the heap creation fail, the function will try to create a smaller heap.
     * If that also fails, the function will request the deletion of the base.
     * @param size The heap's size.
     * @param parentHeap The parent heap.
     * @return If the heap creation was successful.
     */
    virtual bool entryFrmHeap(unsigned long size, EGG::Heap *parentHeap);

    /**
     * @brief Tries to create a heap for the base [Unused].
     * @details Unlike ::entryFrmHeap, this function will give up immediately if heap creation fails.
     * @param size The heap's size.
     * @param parentHeap The parent heap.
     * @return If the heap creation was successful.
     */
    virtual bool entryFrmHeapNonAdjust(unsigned long size, EGG::Heap *parentHeap);
    virtual bool createHeap(); ///< [Unused, does nothing].

    virtual ~fBase_c(); ///< Destroys the base.

    /**
     * @brief Internal function for base construction.
     *
     * @param profName The base's profile name.
     * @param connectParent The parent base's connect node.
     * @param param The base's parameters.
     * @param groupType The base's group type.
     * @return A pointer to the instantiated base, or @p nullptr .
     */
    static fBase_c *fBase_make(ProfileName profName, fTrNdBa_c *connectParent, unsigned long param, u8 groupType);

    static fBaseID_e m_rootUniqueID; ///< Unique ID counter for base construction. @see ::mUniqueID
    static u32 m_tmpCtParam; ///< Temporary storage for the next constructed base's params. @see ::mParam
    static ProfileName m_tmpCtProfName; ///< Temporary storage for the next constructed base's profile name. @see ::mProfName
    static u8 m_tmpCtGroupType; ///< Temporary storage for the next constructed base's group type. @see ::mGroupType
    static fTrNdBa_c *m_tmpCtConnectParent; ///< Temporary storage for the next constructed base's parent connect node.
};
