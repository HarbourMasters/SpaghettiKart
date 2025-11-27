#pragma once

#include <libultraship.h>
#include "GameCamera.h"
#include "CoreMath.h"

extern "C" {
#include "camera.h"
}

/**
 * Tour: A cutscene camera system to provide an overview of the track.
 * How this works:
 * 
 * A tour consists of a series of camera shots.
 * A shot consists of a series of keyframes.
 * The camera is smoothly translated between keyframes using a cubic spline algorithm.
 * 
 */
class TourCamera : public GameCamera {
public:
    enum class TOUR_TYPE {
        SEQUENTIAL = 0,
        RANDOM,
        LOOP
    };

    // State for the track's introductory camera sequence
    TOUR_TYPE Type;
    size_t ShotIndex;
    size_t KeyFrameIndex;
    f32 KeyFrameProgress; // The progress from one point to the next (0.0 - 1.0f).
    bool bShotComplete; // Is the current shot complete?
    bool bTourComplete; // Is the whole camera sequence complete?

    // A single camera keyframe.
    // The camera is smoothly translated between keyframes using
    // a cubic spline algorithm.
    struct KeyFrame {
        FVector Pos;
        FVector LookAt;
        f32 Duration;
    };

    // A sequence of keyframes that make up a cutscene shot.
    struct CameraShot {
        FVector Pos; // Start pos
        FVector LookAt; // Start lookat
        std::vector<KeyFrame> Frames;
    };

    TourCamera(FVector pos, s16 rot, u32 mode);

    virtual void Tick() override;
    virtual void Setup() override;
    void NextShot();
    void Reset();
    void Stop();
    bool IsTourComplete();
    bool MoveCameraAlongSpline(f32* arg1, std::vector<KeyFrame>& keyFrame);
private:

std::vector<KeyFrame> Cut0 = {
    { {132, 288, -38}, {-341, 193, -152}, 100 },
    { {132, 288, -38}, {-341, 193, -152}, 100 },
    { {132, 288, -38}, {-341, 193, -152}, 100 },
    { {83, 8, -45}, {-341, 193, -152}, 100 },
    { {83, 8, -45}, {-341, 193, -152}, 100 },
    { {83, 8, -45}, {-341, 193, -152}, 50 },
    { {83, 8, -45}, {-341, 193, -152}, 50 },
};

std::vector<KeyFrame> Cut1 = {
    { {2148, 163, -655}, {2322, 80, -465}, 100 },
    { {2148, 163, -655}, {2322, 80, -465}, 100 },
    { {2491, 163, -652}, {2322, 80, -465}, 100 },
    { {2537, 163, -260}, {2520, 60, 1240}, 150 },
    { {2537, 163, -260}, {2520, 60, 1240}, 25 },
    { {2537, 163, -260}, {2520, 60, 1240}, 25 },
};

std::vector<KeyFrame> Cutscene = {
    { {-127.5f, 163.6f, 443.2f}, {150, 163, -1300}, 100 },
    { {-127.5f, 163.6f, 443.2f}, {150, 163, -1300}, 100 },
    { {-127.5f, 163.6f, 443.2f}, {150, 163, -1300}, 200 },
    { {111, 163, -1135}, {1216, 120, 308}, 120 },
    { {1336, 163, 128}, {2148, 163, -655}, 100 },
    { {2148, 163, -655}, {2322, 80, -465}, 100 },
    { {2491, 163, -652}, {2322, 80, -465}, 100 },
    { {2537, 163, -260}, {2520, 60, 1240}, 150 },
    { {2694, 163, 1207}, {1823, 138, 677}, 100 },
    { {1683, 163, 1492}, {17, 138, 540}, 100 },
    { {0, 163, 596}, {150, 163, -1300}, 100 },
    { {0, 163, 596}, {150, 163, -1300}, 60 },
    { {0, 163, 596}, {150, 163, -1300}, 60 },
    { {0, 163, 596}, {150, 163, -1300}, 60 },
    { {0, 163, 596}, {150, 163, -1300}, 60 }
};

std::vector<CameraShot> Cuts = {
    {{132, 288, -38}, {-341, 193, -152}, Cut0},
    {{1336, 163, 128}, {2148, 163, -655}, Cut1},
    {{0, 0, 0}, {0, 0, 0}, Cutscene},
};

};
