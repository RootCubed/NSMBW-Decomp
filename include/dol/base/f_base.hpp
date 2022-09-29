#pragma once
#include <types.h>
#include <lib/egg/heap/heap.hpp>
#include <dol/profile/profile.hpp>
#include <dol/base/f_helper_unk.hpp>
#include <dol/base/f_manager.hpp>
#include <dol/cLib/c_list.hpp>

class fBase_c {
public:
    u32 mUniqueID;
    u32 mParam;
    Profile mProfName;

    enum LIFECYCLE_STATE_e {
        CREATING, // object has not yet been added to the fManager_c lists
        CREATED, // object has been added to the fManager_c lists
        DELETING // object has been removed from the fManager_c lists
    };

    LIFECYCLE_STATE_e mLifecycleState;
    bool mWantsDelete;
    bool mIsNotDeferred;
    bool mIsDeferred;
    u8 mGroupType; // 1: scene, 2: actor, 3: other
    u8 mProcessFlags; // &1 = connect, &2 = create, &4 = execute, &8 = delete, &16 = draw

    fManager_c mMng;

    fBaHelper_c mUnusedHelper;
    cListMg_c mUnusedList;
    EGG::Heap *mpHeap;

    enum MAIN_STATE_e {
        UNSUCCESSFUL,
        ERROR,
        SUCCESS,
        WAITING
    };

public:
    fBase_c();
    
    static void *operator new(unsigned long);
    static void operator delete(void *);

    virtual int create();
    virtual bool preCreate();
    virtual bool postCreate(MAIN_STATE_e);

    void createPack();

    virtual int doDelete();
    virtual bool preDelete();
    virtual bool postDelete(MAIN_STATE_e);
    
    void deletePack();

    virtual int execute();
    virtual bool preExecute();
    virtual bool postExecute(MAIN_STATE_e);
    
    void executePack();

    virtual int draw();
    virtual bool preDraw();
    virtual bool postDraw(MAIN_STATE_e);
    
    void drawPack();

    virtual void deleteReady();

    void connectProc();
    void deleteRequest();
    
    fBase_c *getConnectParent();
    fBase_c *getConnectChild();
    fBase_c *getConnectBrNext();

    virtual bool entryFrmHeap(u32 size, void *parentHeap);
    virtual bool entryFrmHeapNonAdjust(u32 size, void *parentHeap);
    virtual void createHeap();
    
    virtual ~fBase_c();

    void runCreate();

    static void setTmpCtData(Profile prof, fTrNdBa_c *connectParent, u32 param, u8 groupType);
    static fBase_c *fBase_make(Profile prof, fTrNdBa_c *connectParent, u32 param, u8 groupType);
    static fBase_c *createChild(Profile prof, fBase_c *parent, u32 param, u8 groupType);
    static fBase_c *createRoot(Profile prof, u32 param, u8 groupType);

private:
    int commonPack(bool (fBase_c::*preFunc)(), int (fBase_c::*doFunc)(), bool (fBase_c::*postFunc)(MAIN_STATE_e));

    static u32 m_rootUniqueID;
    static u32 m_tmpCtParam;
    static Profile m_tmpCtProfName;
    static u8 m_tmpCtGroupType;
};