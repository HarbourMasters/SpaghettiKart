#pragma once

#include <libultraship.h>
#include "engine/Actor.h"
#include "src/textures.h"
#include "engine/CoreMath.h"
#include "port/Game.h"

class AText : public AActor {
public:
    enum TextMode : int16_t {
        STATIONARY,
        FOLLOW_PLAYER
    };

    enum FadeMode : int16_t {
        NO_FADE,
        FADE_IN,
        FADE_OUT
    };

    enum DistanceProps : int16_t {
        TOO_CLOSE,
        ACTIVE,
        TOO_FAR
    };

    DistanceProps Dist = ACTIVE;
    DistanceProps PrevState = ACTIVE;
    FadeMode FadeState = NO_FADE;

    struct CharacterList {
        const char* Texture;
        f32 column;
        f32 row;
        u32 width;
        u32 height;
        s32 mode;
        Vtx vtx[4];
    };

    std::vector<CharacterList> TextureList;

    std::string Text; // The text to be displayed
    TextMode Mode;
    uint32_t PlayerIndex;
    float WidthOffset = 0.0f;
    float HeightOffset = 8.0f; // Place text above player
    f32 ScaleX = 1.0f;
    f32 LetterSpacing = 1.0f;
    f32 Far = 14000.0f;
    f32 Close = 350.0f;

    bool FaceCamera = true;
    bool Animate = false;
    bool SingleColour = true; // Only used for imGUI to show more colour options
    // 1 colour for each of the 4 vtx
    // This allows setting each vtx colour individually
    RGBA8 TextColour[4];

    // Constructor
    AText(const SpawnParams& params);
    virtual ~AText() override = default;

    /**
     * This is simply a helper function to keep Spawning code clean
     * Main parameters usage:
     * 
     * PlayerIndex is only used if textMode is FOLLOW_PLAYER
     * 
     * Other available options:
     * 
     * TextColour<RGBA8>      - Colour of the text (1 colour for each vertex)
     * ScaleX<float>          - Font scale Y
     * LetterSpacing<float>   - Space between letters
     * HeightOffset<float>    - Height above player when in FOLLOW_PLAYER mode
     * WidthOffset<float>     - Left/right offset of the text when in FOLLOW_PLAYER mode
     * Far<float>             - Skip rendering if camera is too far away
     * Close<float>           - Skip rendering if too close to the camera
     * Animate<bool>          - Cycle colours similar to the grand prix title text
     * 
     * Other options usage:
     * 
     * AText* text = AText::Spawn("Hello World", FVector(0, 0, 0), etc.);
     * text->Animate = true;
     * text->Far = 14000.0f;
     * text->ScaleX = 0.1f; // Recommend in the range of 0-2.0f. Default 1.0f
     * text->LetterSpacing = 1.0f; // Default 1.0f
     * 
     * For one single colour, set all vertices to the same colour.
     * text->TextColour[0] = {255, 0, 0, 255}; // Red
     * text->TextColour[1] = {0, 255, 0, 255}; // Green
     * text->TextColour[2] = {0, 0, 255, 255}; // Blue
     * text->TextColour[3] = {255, 255, 0, 255}; // Yellow
     * 
     * The above will result in a gradient between red, green, blue, and yellow
     * For transparency {0, 0, 0, 100} <-- alpha value of 100 will render semi-transparent black text.
     *
     */
    static inline AText* Spawn(std::string text, FVector pos, FVector scale, AText::TextMode textMode, int16_t playerIndex) {
        SpawnParams params = {
            .Name = "hm:text",
            .Type = static_cast<int16_t>(textMode),
            .Behaviour = static_cast<int16_t>(playerIndex),
            .Skin = text,
            .Location = pos,
            .Scale = scale,
        };
        return static_cast<AText*>(gWorldInstance.AddActor(new AText(params)));
    }

    // Virtual functions to be overridden by derived classes
    virtual void Tick() override;
    virtual void Draw(Camera* camera) override;
    virtual void SetSpawnParams(SpawnParams& params) override;
    virtual bool IsMod() override;
    virtual void DrawEditorProperties() override;
      void DrawColourEditor(bool* updated);
    void FollowPlayer();

    void SetText(std::string text);
    std::string ValidateString(const std::string_view& text);
    void Refresh();
    void Print3D(char* text, s32 tracking, s32 mode);
    void PrintLetter3D(MenuTexture* glyphTexture, f32 column, f32 row, s32 mode);
    void SetupVtx();
    void DrawText3D(Camera* camera); // Based on func_80095BD0
    void AnimateColour(Vtx* vtx); // Animate the vtx colours
    void FadeIn(Vtx* vtx);
    void FadeOut(Vtx* vtx);

    inline uint8_t FloatToU8(float v) {
        return (uint8_t)(v * 255.0f);
    }

    Vtx myVtx[12] = { // D_02007BB8
        {{{ 0, 16, 0}, 0, {   0, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{26, 16, 0}, 0, {1600, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{26, 0, 0}, 0, {1600, 960}, {0xff, 0xff, 0xff, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 960}, {0xff, 0xff, 0xff, 0xff}}},
        {{{ 0, 16, 0}, 0, {  0, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{16, 16, 0}, 0, {960, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{16, 0, 0}, 0, {960, 960}, {0xff, 0xff, 0xff, 0xff}}},
        {{{ 0, 0, 0}, 0, {  0, 960}, {0xff, 0xff, 0xff, 0xff}}},
        {{{ 0, 32, 0}, 0, {   0, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{30, 32, 0}, 0, {1856, 0}, {0xff, 0xff, 0xff, 0xff}}},
        {{{30, 0, 0}, 0, {1856, 1984}, {0xff, 0xff, 0xff, 0xff}}},
        {{{ 0, 0, 0}, 0, {   0, 1984}, {0xff, 0xff, 0xff, 0xff}}},
    };
};
