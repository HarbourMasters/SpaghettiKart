#pragma once

namespace SF64 {
    enum class ResourceType {
    // SF64
        AnimData = 0x414E494D,     // ANIM
        ColPoly = 0x43504C59,      // CPLY
        EnvSettings = 0x454E5653,  // ENVS
        Limb = 0x4C494D42,         // LIMB
        Message = 0x4D534720,      // MSG
        MessageTable = 0x4D534754, // MSGT
        Skeleton = 0x534B454C,     // SKEL
        Script = 0x53435250,       // SCRP
        ScriptCmd = 0x53434D44,    // SCMD
        Hitbox = 0x48544258,       // HTBX
        ObjectInit = 0x4F42494E,   // OBIN
        Vec3f = 0x56433346,        // VC3F
        Vec3s = 0x56433353,        // VC3S
        GenericArray = 0x47415252, // GARR
    };
} // namespace SOH

namespace MK64 {
    enum class ResourceType {
        MK_Array = 0x4F415252,     // OARR
        CourseVertex = 0x43565458, // CVTX
        TrackSection = 0x5343544E, // SCTN
        Waypoints = 0x57505453,    // WPTS
        Metadata  = 0x4D444154,    // MDAT
        SpawnData = 0x53444154,    // SDAT
        UnkSpawnData = 0x55534454,     // USDT
        KartAI = 0x44424856, // DBHV
    };
}