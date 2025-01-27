#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <string>

#include "../Math/Mat4.hpp"

namespace Theater
{
    struct Keyframe 
    {
        double Time;
        Vector3 Position;
        //Quaternion Rotation;
        Vector3 Scale;
    };

    struct AnimationChannel
    {
        std::string BoneName;
        std::vector<Keyframe> Keyframes;
    };

    struct Animation
    {
        std::string Name;
        double Duration;
        double TicksPerSecond;
        std::vector<AnimationChannel> Channels; 
    };

    struct Bone
    {
        std::string Name;
        Matrix4 OffsetMatrix;
        Matrix4 FinalTransform;
        int32_t ParentIndex;
    };

    struct Skeleton 
    {
        std::vector<Bone> bones;
        std::unordered_map<std::string, int32_t> boneMap;
    };
}