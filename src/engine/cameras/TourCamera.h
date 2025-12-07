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
    bool bActivateAudio;
    uint8_t Alpha;

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
    virtual void SetViewProjection() override;
    void NextShot();
    void Reset();
    void Stop();
    bool IsTourComplete();
    bool MoveCameraAlongSpline(f32* arg1, std::vector<KeyFrame>& keyFrame);
    void Draw();
};
