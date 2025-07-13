#include <kamekLoader.h>

extern "C" void main();
extern "C" char* regionDetect();
void applySpeedPatches(int region);
extern "C" void branchToGravity();
void applyGravityPatches();

u32 TEST_REGION = 0x800CF6CC;
u32 TEST_REGION2 = 0x8000423A;
u32 RETURN_POINT2 = 0x8014688c;
u32 RETURN_POINT2_LOCAL = 0x8014688c;
int REGION;

u32 BToadXMaxRunSpeed = 0x40600000;
u32 BToadXMaxRunSpeedHasStar = 0x40a00000;
// u32 BToadXMaxRunSpeedHasYoshi = 0x40600000;      // equal BToadXMaxRunSpeed
// u32 BToadXMaxRunSpeedHasStarYoshi = 0x40a00000;  // equal BToadXMaxRunSpeedHasStar
u32 BToadGravityDataGravity = 0x40366666;
u32 BToadGravityDataUnk0 = 0x40000000;
u32 BToadGravityDataUnk1 = 0x3eb33333;
u32 BToadGravityDataUnk2 = 0xbd8f5c29;
u32 BToadGravityDataJumpLow = 0xbecccccd;
u32 BToadGravityDataUnk3 = 0xbedc28f6;
u32 BToadGravityDataUnk4 = 0xbec28f5c;
u32 BToadGravityDataUnk5 = 0xbe051eb8;
u32 BToadGravityDataUnk6 = 0xbeb851ec;
u32 BToadGravityDataUnk7 = 0xbe9eb852;
// u32 BToadGravityDataGravityMini = 0x40366666;    // equal BToadGravityDataGravity
u32 BToadWaterMoveSpeedUpSpeed = 0xbd0f5c29;
u32 BToadWaterMoveSpeedSinkSpeed = 0xbd23d70a;
u32 BToadWaterMoveSpeedDiveSpeed = 0xbe000000;
u32 BToadWaterGroundJump = 0x3f800000;

/* ASM */
asm void main() {
    nofralloc               // make disable prologue/epilogue
    // Push stack
    stwu r1, -0x80 (r1)     // 124 + パディング
    mflr r0
    stw r0, 0x84 (r1)
    stmw r3, 0x8 (r1)

    bl regionDetect
    bl applySpeedPatches

    // Pop stack
    lmw r3, 0x8 (r1)
    lwz r3, 0x84 (r1)
    mtlr r3
    addi r1, r1, 0x80

    // restore instruction
    li r0, 0xf

    lis r3, 0x8005
    ori r3, r3, 0xDF10
    mtctr r3
    
    bctr
}

asm char* regionDetect() {
    lis r5, TEST_REGION@ha
    ori r5, r5, TEST_REGION@l
    lwz r5, 0x0(r5)  // load pointer
    lwz r3, 0x0(r5)  // load value
    clrlwi r3, r3, 16

    // version 1
    cmpwi r3, 0x0030
    beq setEU
    cmpwi r3, 0x0465
    beq setUS
    cmpwi r3, 0x00B4
    beq setJP
    // version 2
    cmpwi r3, 0x0038
    beq setEU
    cmpwi r3, 0x0000
    beq setUS
    cmpwi r3, 0x000C
    beq setJP
    // other regions
    cmpwi r3, 0x0001
    lis r5, TEST_REGION2@ha
    ori r5, r5, TEST_REGION2@l
    lbz r3, 0(r5)
    cmpwi r3, 0xC8
    beq setKR
    cmpwi r3, 0xAC
    beq setTW
    blr             // unknown region!!

    setEU:
        li r3, 0x4555  // EU
        b storeRegion
    setUS:
        li r3, 0x5553
        b storeRegion
    setJP:
        li r3, 0x4a50
        b storeRegion
    setKR:
        li r3, 0x4b52
        b storeRegion
    setTW:
        li r3, 0x5457
    
    storeRegion:    // REGION変数使わないなら消せ
        lis r6, REGION@ha
        ori r6, r6, REGION@l
        stw r3, 0x0(r6)

    blr
}

void applySpeedPatches(int region) {
    if (region == 0x4555){  // 0x4555: EU(ASCII)
        *(volatile u32*)0x802ef108 = BToadXMaxRunSpeed;
        *(volatile u32*)0x802ef180 = BToadXMaxRunSpeedHasStar;
        *(volatile u32*)0x802ef1f8 = BToadXMaxRunSpeed;
        *(volatile u32*)0x802ef270 = BToadXMaxRunSpeedHasStar;
        *(volatile u32*)0x8042db68 = BToadWaterMoveSpeedUpSpeed;
        *(volatile u32*)0x8042db70 = BToadWaterMoveSpeedSinkSpeed;
        *(volatile u32*)0x8042db6c = BToadWaterMoveSpeedDiveSpeed;
        *(volatile u32*)0x8042bb3c = BToadWaterGroundJump;
        // Overwrite function with branch
        *(volatile u32*)0x80146888 = 0x4bebf890;      // new: b 80006118, orig: cmpwi r0, 0x3
    }
    else if (region == 0x5553){
        *(volatile u32*)0x802EEE08 = BToadXMaxRunSpeed;
        *(volatile u32*)0x802EEE80 = BToadXMaxRunSpeedHasStar;
        *(volatile u32*)0x802EEEF8 = BToadXMaxRunSpeed;
        *(volatile u32*)0x802EEF70 = BToadXMaxRunSpeedHasStar;
        *(volatile u32*)0x8042D888 = BToadWaterMoveSpeedUpSpeed;
        *(volatile u32*)0x8042D890 = BToadWaterMoveSpeedSinkSpeed;
        *(volatile u32*)0x8042D88C = BToadWaterMoveSpeedDiveSpeed;
        *(volatile u32*)0x8042B85C = BToadWaterGroundJump;
        // Overwrite function with branch
        *(volatile u32*)0x80146748 = 0x4bebf9d0;
    }
    else if (region == 0x4a50){
        *(volatile u32*)0x802EEC28 = BToadXMaxRunSpeed;
        *(volatile u32*)0x802EECA0 = BToadXMaxRunSpeedHasStar;
        *(volatile u32*)0x802EED18 = BToadXMaxRunSpeed;
        *(volatile u32*)0x802EED90 = BToadXMaxRunSpeedHasStar;
        *(volatile u32*)0x8042D5A8 = BToadWaterMoveSpeedUpSpeed;
        *(volatile u32*)0x8042D5B0 = BToadWaterMoveSpeedSinkSpeed;
        *(volatile u32*)0x8042D5AC = BToadWaterMoveSpeedDiveSpeed;
        *(volatile u32*)0x8042B57C = BToadWaterGroundJump;
        // Overwrite function with branch
        *(volatile u32*)0x80146558 = 0x4bebfbc0;
    }
    else {
        *(volatile u32*)0x802ef448 = BToadXMaxRunSpeed;
        *(volatile u32*)0x802ef4c0 = BToadXMaxRunSpeedHasStar;
        *(volatile u32*)0x802ef538 = BToadXMaxRunSpeed;
        *(volatile u32*)0x802ef5b0 = BToadXMaxRunSpeedHasStar;
        if (region == 0x4b52) {
            *(volatile u32*)0x8043a530 = BToadWaterMoveSpeedUpSpeed;
            *(volatile u32*)0x8043a538 = BToadWaterMoveSpeedSinkSpeed;
            *(volatile u32*)0x8043a534 = BToadWaterMoveSpeedDiveSpeed;
            *(volatile u32*)0x804384fc = BToadWaterGroundJump;
        }
        else if (region == 0x5457) {
            *(volatile u32*)0x80438930 = BToadWaterMoveSpeedUpSpeed;
            *(volatile u32*)0x80438938 = BToadWaterMoveSpeedSinkSpeed;
            *(volatile u32*)0x80438934 = BToadWaterMoveSpeedDiveSpeed;
            *(volatile u32*)0x804368fc = BToadWaterGroundJump;
        }
        // Overwrite function with branch
        *(volatile u32*)0x80146c58 = 0x4bebf4c0;
    }
    
    return;
}

asm void branchToGravity() {
    nofralloc               // make disable prologue/epilogue
    // Push stack
    stwu r1, -0x80 (r1)     // 124 + パディング
    mflr r0
    stw r0, 0x84 (r1)
    stmw r3, 0x8 (r1)

    bl applyGravityPatches

    // Pop stack
    lmw r3, 0x8 (r1)
    lwz r4, 0x84 (r1)
    mtlr r4
    addi r1, r1, 0x80

    // prepare return
    lis r4, RETURN_POINT2_LOCAL@ha
    ori r4, r4, RETURN_POINT2_LOCAL@l
    lwz r4, 0x0(r4)
    mtctr r4

    // restore instruction
    cmpwi r0, 0x3

    bctr
}

void applyGravityPatches() {
    if (REGION == 0x4555){
        *(volatile u32*)0x802f591c = BToadGravityDataGravity;
        *(volatile u32*)0x802f5920 = BToadGravityDataUnk0;
        *(volatile u32*)0x802f5924 = BToadGravityDataUnk1;
        *(volatile u32*)0x802f5928 = BToadGravityDataUnk2;
        *(volatile u32*)0x802f5930 = BToadGravityDataJumpLow;
        *(volatile u32*)0x802f5934 = BToadGravityDataUnk3;
        *(volatile u32*)0x802f5950 = BToadGravityDataUnk4;
        *(volatile u32*)0x802f5954 = BToadGravityDataUnk5;
        *(volatile u32*)0x802f5958 = BToadGravityDataUnk6;
        *(volatile u32*)0x802f595c = BToadGravityDataUnk7;
        *(volatile u32*)0x802f5964 = BToadGravityDataGravity;
    }
    else if (REGION == 0x5553){
        *(volatile u32*)0x802F561C = BToadGravityDataGravity;
        *(volatile u32*)0x802F5620 = BToadGravityDataUnk0;
        *(volatile u32*)0x802F5624 = BToadGravityDataUnk1;
        *(volatile u32*)0x802F5628 = BToadGravityDataUnk2;
        *(volatile u32*)0x802F5630 = BToadGravityDataJumpLow;
        *(volatile u32*)0x802F5634 = BToadGravityDataUnk3;
        *(volatile u32*)0x802F5650 = BToadGravityDataUnk4;
        *(volatile u32*)0x802F5654 = BToadGravityDataUnk5;
        *(volatile u32*)0x802F5658 = BToadGravityDataUnk6;
        *(volatile u32*)0x802F565C = BToadGravityDataUnk7;
        *(volatile u32*)0x802F5664 = BToadGravityDataGravity;
        RETURN_POINT2_LOCAL = RETURN_POINT2 - 0x140; // 0x8014688c -> 0x8014674c
    }
    else if (REGION == 0x4a50){
        *(volatile u32*)0x802F543C = BToadGravityDataGravity;
        *(volatile u32*)0x802F5440 = BToadGravityDataUnk0;
        *(volatile u32*)0x802F5444 = BToadGravityDataUnk1;
        *(volatile u32*)0x802F5448 = BToadGravityDataUnk2;
        *(volatile u32*)0x802F5450 = BToadGravityDataJumpLow;
        *(volatile u32*)0x802F5454 = BToadGravityDataUnk3;
        *(volatile u32*)0x802F5470 = BToadGravityDataUnk4;
        *(volatile u32*)0x802F5474 = BToadGravityDataUnk5;
        *(volatile u32*)0x802F5478 = BToadGravityDataUnk6;
        *(volatile u32*)0x802F547C = BToadGravityDataUnk7;
        *(volatile u32*)0x802F5484 = BToadGravityDataGravity;
        RETURN_POINT2_LOCAL = RETURN_POINT2 - 0x330; // 0x8014688c -> 0x8014655c
    }
    else if (REGION == 0x4b52 || REGION == 0x5457){
        *(volatile u32*)0x802f5c74 = BToadGravityDataGravity;
        *(volatile u32*)0x802f5c78 = BToadGravityDataUnk0;
        *(volatile u32*)0x802f5c7c = BToadGravityDataUnk1;
        *(volatile u32*)0x802f5c80 = BToadGravityDataUnk2;
        *(volatile u32*)0x802f5c88 = BToadGravityDataJumpLow;
        *(volatile u32*)0x802f5c8c = BToadGravityDataUnk3;
        *(volatile u32*)0x802f5ca8 = BToadGravityDataUnk4;
        *(volatile u32*)0x802f5cac = BToadGravityDataUnk5;
        *(volatile u32*)0x802f5cb0 = BToadGravityDataUnk6;
        *(volatile u32*)0x802f5cb4 = BToadGravityDataUnk7;
        *(volatile u32*)0x802f5cbc = BToadGravityDataGravity;
        RETURN_POINT2_LOCAL = RETURN_POINT2 - 0x3D0; // 0x8014688c -> 0x80146c5c
    }

    return;
}