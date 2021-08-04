#pragma once
#include <Vector3.h>
#include <Quaternion.h>

typedef enum VRfree_BoneIndex {
    VRfree_BoneIndex_None = -1,
    VRfree_BoneIndex_LowerArm = 0,
    VRfree_BoneIndex_Hand,
    VRfree_BoneIndex_Thumb0,
    VRfree_BoneIndex_Thumb1,
    VRfree_BoneIndex_Thumb2,
    VRfree_BoneIndex_ThumbEnd,
    VRfree_BoneIndex_IndexFinger0,
    VRfree_BoneIndex_IndexFinger1,
    VRfree_BoneIndex_IndexFinger2,
    VRfree_BoneIndex_IndexFingerEnd,
    VRfree_BoneIndex_MiddleFinger0,
    VRfree_BoneIndex_MiddleFinger1,
    VRfree_BoneIndex_MiddleFinger2,
    VRfree_BoneIndex_MiddleFingerEnd,
    VRfree_BoneIndex_RingFinger0,
    VRfree_BoneIndex_RingFinger1,
    VRfree_BoneIndex_RingFinger2,
    VRfree_BoneIndex_RingFingerEnd,
    VRfree_BoneIndex_PinkyFinger0,
    VRfree_BoneIndex_PinkyFinger1,
    VRfree_BoneIndex_PinkyFinger2,
    VRfree_BoneIndex_PinkyFingerEnd,
    VRfree_BoneIndex_NumBones
} VRfree_BoneIndex;

typedef struct VRfree_BoneTransform {
    VRfree_BoneIndex parentBone;
    Vector3 localPosition;
    Quaternion localRotation;
} VRfree_BoneTransform;

