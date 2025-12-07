#include <libultraship.h>
#include <libultra/gbi.h>
#include <vector>

#include "Text.h"

#include "port/interpolation/FrameInterpolation.h"
#include "engine/Matrix.h"
#include "engine/editor/EditorMath.h"

extern "C" {
#include "defines.h"
#include "main.h"
#include "menu_items.h"
#include "assets/models/data_segment2.h"
#include "render_player.h"
#include "math_util.h"
#include "assets/textures/texture_data_2.h"
#include "render_objects.h"
#include "common_structs.h"
#include "code_80005FD0.h"
}

AText::AText(const SpawnParams& params) : AActor(params) {
    Name = "Text";
    ResourceName = "hm:text";
    
    SpawnPos = params.Location.value_or(FVector(0.0f, 100.0f, 0.0f));
    Pos[0] = SpawnPos.x;
    Pos[1] = SpawnPos.y;
    Pos[2] = SpawnPos.z;

    SpawnRot = params.Rotation.value_or(IRotator(0, 0, 0));
    Rot[0] = SpawnRot.pitch;
    Rot[1] = SpawnRot.yaw;
    Rot[2] = SpawnRot.roll;

    SpawnScale = params.Scale.value_or(FVector(1.0f, 1.0f, 1.0f));
    Scale = SpawnScale;

    ScaleX = params.FVec2.value_or(FVector{1.0f, 0.0f, 0.0f}).x;

    Mode = static_cast<TextMode>(params.Type.value_or(0)); // STATIONARY

    Animate = params.Bool.value_or(false);
    FaceCamera = params.Bool2.value_or(true);

    WidthOffset = params.Speed.value_or(0.0f);
    HeightOffset = params.SpeedB.value_or(8.0f);

    FVector options = params.Velocity.value_or(FVector{1.0f, 14000.0f, 0.0f});
    LetterSpacing = options.x;
    Far = options.y;
    Close = options.z;

    TextColour[0] = params.Colour.value_or(RGBA8{255, 255, 255, 255});
    TextColour[1] = params.Colour2.value_or(RGBA8{255, 255, 255, 255});
    TextColour[2] = params.Colour3.value_or(RGBA8{255, 255, 255, 255});
    TextColour[3] = params.Colour4.value_or(RGBA8{255, 255, 255, 255});

    PlayerIndex = static_cast<uint32_t>(params.Behaviour.value_or(0));
    if (PlayerIndex < 0 || PlayerIndex >= NUM_PLAYERS) {
        PlayerIndex = 0;
    }

    Text = ValidateString(params.Skin.value_or("Harbour Masters"));
    AText::Print3D((char*)Text.c_str(), 0, CENTER_TEXT_MODE_2);
}

/**
 * Filters out bad characters (allows a-z, A-Z, 0-9, space)
 * Returns "Blank Text" for blank input
 * Returns "Invalid" if no valid input found
 * Limits str to 20 characters
 * 
 * The font does support some symbols and other language characters
 * But these need to be checked thoroughly before white-listing.
 */
std::string AText::ValidateString(const std::string_view& s) {
    if (CVarGetInteger("gIsEditorEnabled", false) == true) {
        if (s.empty()) { return "Blank Text"; }
    } else {
        if (s.empty()) { return ""; }
    }

    Text.clear();

    for (char c : s) {
        if (std::isalpha(static_cast<unsigned char>(c)) || 
            std::isdigit(static_cast<unsigned char>(c)) || 
            c == ' ') 
        {
            Text += c;
            if (Text.size() >= 20) {
                break;
            };
        }
    }

    // No valid characters found
    if (Text.empty()) {
        return "Invalid";
    }
    return Text;
}

void AText::SetText(std::string text) {
    AText::ValidateString(text);
    Refresh();
}

/*
 * Most changes during runtime require a refresh because the text is generated statically
 * with the intention of this code being somewhat performant
 */
void AText::Refresh() {
    AText::TextureList.clear();
    AText::Print3D((char*)Text.c_str(), 0, CENTER_TEXT_MODE_2);
}

bool AText::IsMod() { return true; }

void AText::SetSpawnParams(SpawnParams& params) {
    AActor::SetSpawnParams(params);
    params.Name = ResourceName;
    params.Type = static_cast<int16_t>(Mode);
    params.Behaviour = PlayerIndex;
    params.Skin = Text;

    params.Colour = TextColour[0];
    params.Colour2 = TextColour[1];
    params.Colour3 = TextColour[2];
    params.Colour4 = TextColour[3];

    params.Speed = WidthOffset;
    params.SpeedB = HeightOffset;

    params.Velocity = {LetterSpacing, Far, Close};
    params.FVec2 = {ScaleX, 0.0f, 0.0f};

    params.Bool = Animate;
    params.Bool2 = FaceCamera;
}

void AText::Tick() {
    switch(Mode) {
        case STATIONARY:
            break; // Do nothing
        case FOLLOW_PLAYER:
            AText::FollowPlayer();
            break;
    }
}

void AText::FollowPlayer() {
    Pos[0] = gPlayers[PlayerIndex].pos[0] + WidthOffset;
    Pos[1] = gPlayers[PlayerIndex].pos[1] + HeightOffset;
    Pos[2] = gPlayers[PlayerIndex].pos[2];

    // Animate text if player is in first place
    // if (gGPCurrentRaceRankByPlayerId[PlayerIndex] == 0) {
    //     Animate = true;
    // } else {
    //     Animate = false;
    // }
}

void AText::Draw(Camera* camera) {
    switch(Mode) {
        case STATIONARY:
            break; // Do nothing
        case FOLLOW_PLAYER:
            if (PlayerIndex == camera->playerId) {
                return; // Do not draw the local players own name
            }
            if ((gPlayers[PlayerIndex].effects & BOO_EFFECT) == BOO_EFFECT) {
                FadeState = FADE_OUT;
                AText::DrawText3D(camera);
                return; // Skip expensive calculations below
            }
            break;
    }

    f32 distance = is_within_render_distance(camera->pos, (float*)&Pos[0], camera->rot[1], Close,
                                             gCameraFOV[camera - camera1], Far);

    if (distance == -1.0f) {
        Dist = DistanceProps::TOO_FAR;
    } else if (distance < Close) {
        Dist = DistanceProps::TOO_CLOSE;
    } else {
        Dist = DistanceProps::ACTIVE;
    }

    if (Dist != PrevState) {
        PrevState = Dist;
        if ((distance == -1.0f) || (distance < Close)) {
            FadeState = FADE_OUT;
        } else {
            FadeState = FADE_IN;
        }
    }

    AText::DrawText3D(camera);
}

/**
 * These have been refactored for efficiency purposes.
 * The new method uses 1 matrix to display the whole string
 * And then setting vertex data is done during the setup/constructor phase,
 * instead of during rendering
 * This requires a refresh if the data ever changes
 * 
 * This method is more efficient because the original version does all this work
 * during the render phase. Now it's done during the actor spawn phase with the exception
 * if any changes are made at runtime.
 */
void AText::Print3D(char* text, s32 tracking, s32 mode) {
    char* temp_string = text;
    s32 stringWidth = 0;
    s32 glyphIndex;
    s32 sp60;

    s32 column = 0;
    s32 row = 0;

    if (text == NULL) {
        // @port if invalid text is loaded it will skip rendering it.
        return;
    }

    while (*temp_string != '\0') {
        glyphIndex = char_to_glyph_index(temp_string);
        if (glyphIndex >= 0) {
            stringWidth += ((gGlyphDisplayWidth[glyphIndex] + tracking) * ScaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            stringWidth += ((tracking + 7) * ScaleX);
        } else {
            return;
        }
        if (glyphIndex >= 0x30) {
            temp_string += 2;
        } else {
            temp_string += 1;
        }
    }

    switch (mode) {
        case LEFT_TEXT:
            //! FAKE:
            do {
            } while (0);
        case RIGHT_TEXT:
            column -= stringWidth;
            break;
        case CENTER_TEXT_MODE_1:
        case CENTER_TEXT_MODE_2:
            column -= stringWidth / 2;
            break;
        default:
            break;
    }

    if (mode < 3) {
        sp60 = 1;
    } else {
        sp60 = 2;
    }

    while (*text != '\0') {
        glyphIndex = char_to_glyph_index(text);
        if (glyphIndex >= 0) {
            AText::PrintLetter3D(gGlyphTextureLUT[glyphIndex],
                                            column, row, sp60);
            column = column + (s32) ((gGlyphDisplayWidth[glyphIndex] + tracking) * ScaleX);
        } else if ((glyphIndex != -2) && (glyphIndex == -1)) {
            column = column + (s32) ((tracking + 7) * ScaleX);
        } else {
            return;
        }
        if (glyphIndex >= 0x30) {
            text += 2;
        } else {
            text += 1;
        }
    }
    SetupVtx(); // position each letter
}

void AText::PrintLetter3D(MenuTexture* glyphTexture, f32 column, f32 row, s32 mode) {
    s32 var_v0;
    u8* temp_v0_2;
    f32 thing0;
    f32 thing1;
    MenuTexture* texture;

    texture = glyphTexture;
    while (texture->textureData != NULL) {
        if (texture->textureData != 0) {
            f32 col = texture->dX + column;

            TextureList.emplace_back(CharacterList{
                (const char*) texture->textureData,
                col,
                texture->dY + row,
                texture->width,
                texture->height,
                mode,
            });
        }
        texture++;
    }
}

void AText::SetupVtx() {
    for (CharacterList& character : TextureList) {

        Vtx* vtxPtr;
        switch (character.width) {
            default:
                vtxPtr = (Vtx*)&AText::myVtx[4];
                break;
            case 16:
                vtxPtr = (Vtx*)&AText::myVtx[4];
                break;
            case 26:
                vtxPtr = (Vtx*)&AText::myVtx[0];
                break;
            case 30:
                vtxPtr = (Vtx*)&AText::myVtx[8];
                break;
        }
        // printf("col %f width %d span %d\n", character.column, character.width, character.span);
        // memcpy the vtx data into the unique vtx data for this letter
        Vtx* vtxSrc = (Vtx*)vtxPtr;
        memcpy(&character.vtx, vtxSrc, sizeof(Vtx) * 4);

        for (size_t i = 0; i < 4; i++) {
            // Set the location for this letter (beside the previous letter)     center the text over the anchor point
            float span = (character.column * LetterSpacing);
            character.vtx[i].v.ob[0] += (s16)span;

            // Set the colour for this letter
            character.vtx[i].v.cn[0] = TextColour[i].r;
            character.vtx[i].v.cn[1] = TextColour[i].g;
            character.vtx[i].v.cn[2] = TextColour[i].b;
            character.vtx[i].v.cn[3] = TextColour[i].a;
        }
    }
}

void AText::DrawText3D(Camera* camera) { // Based on func_80095BD0
    Mat4 mtx;

    if (FaceCamera) {
        ApplySphericalBillBoard(mtx, *(FVector*)Pos, Scale, camera->cameraId);
    } else {
        ApplyMatrixTransformations(mtx, *(FVector*)Pos, *(IRotator*)Rot, Scale);
    }

    AddObjectMatrix(mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    FrameInterpolation_RecordOpenChild("actor_text", TAG_LETTER(this));
    gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077A8);

    for (CharacterList& tex : TextureList) {
        switch (tex.mode) {
            case 1:
                gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077F8);
                break;
            case 2:
                gSPDisplayList(gDisplayListHead++, (Gfx*)D_02007818);
                break;
        }

        //printf("tex texture %p width %d height %d mode %d col %f\n", tex.Texture, tex.width, tex.height, tex.mode, tex.column);
        gDPLoadTextureTile_4b(gDisplayListHead++, (Gfx*)tex.Texture, G_IM_FMT_I, tex.width, 0, 0, 0, tex.width, tex.height + 2, 0,
                            G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);

       // gSPClearGeometryMode(gDisplayListHead++, G_ZBUFFER);

        if (Animate) {
            AnimateColour(tex.vtx);
        }

        switch(FadeState) {
            case FADE_IN:
                AText::FadeIn(tex.vtx);
                break;
            case FADE_OUT:
                AText::FadeOut(tex.vtx);
                break;
        }

        gSPVertex(gDisplayListHead++, (uintptr_t)tex.vtx, 4, 0);
        gSP2Triangles(gDisplayListHead++, 0, 2, 1, 0, 0, 3, 2, 0);

        // gSPSetGeometryMode(gDisplayListHead++, G_ZBUFFER);
    }

    gSPDisplayList(gDisplayListHead++, (Gfx*)D_020077D8);
    FrameInterpolation_RecordCloseChild();
}

void AText::AnimateColour(Vtx* vtx) {
    u8 r = (sin(gGlobalTimer * 0.1f) * 0.5f + 0.5f) * 200;
    u8 g = (sin(gGlobalTimer * 0.1f + 2.0f) * 0.5f + 0.5f) * 200;
    u8 b = (sin(gGlobalTimer * 0.1f + 4.0f) * 0.5f + 0.5f) * 200;

    for (size_t i = 0; i < 4; i++) {
        vtx[i].v.cn[0] = r;
        vtx[i].v.cn[1] = g;
        vtx[i].v.cn[2] = b;
    }
}

#define fadeSpeed 16
void AText::FadeIn(Vtx* vtx) {
    uint8_t alpha = vtx[0].v.cn[3];
    if (alpha + fadeSpeed > 255) {
        alpha = 255;
        FadeState = NO_FADE;
    } else {
        alpha += fadeSpeed;
    }

    // Apply alpha to all 4 vertices
    for (size_t i = 0; i < 4; i++) {
        vtx[i].v.cn[3] = alpha;
    }
}

void AText::FadeOut(Vtx* vtx) {
    uint8_t alpha = vtx[0].v.cn[3];
    if (alpha < fadeSpeed) {
        alpha = 0;
        FadeState = NO_FADE;
    } else {
        alpha -= fadeSpeed;
    }

    // Apply alpha to all 4 vertices
    for (size_t i = 0; i < 4; i++) {
        vtx[i].v.cn[3] = alpha;
    }
}
#undef fadeSpeed

void AText::DrawEditorProperties() {
    bool updated = false;
    ImGui::Text("Text");
    ImGui::SameLine();

    char text[21] = "";
    strncpy(text, Text.c_str(), sizeof(text));
    ImGui::InputText("Enter text", text, IM_ARRAYSIZE(text));

    Text = std::string(text);

    ImGui::Text("Mode");
    ImGui::SameLine();

    int32_t mode = static_cast<int32_t>(Mode);
    const char* items[] = { "STATIONARY", "FOLLOW PLAYER" };

    if (ImGui::Combo("##Type", &mode, items, IM_ARRAYSIZE(items))) {
        Mode = static_cast<TextMode>(mode);
        updated = true;
    }

    ImGui::DragFloat("Far Render Dist", &Far);
    ImGui::DragFloat("Close Render Dist", &Close);

    ImGui::Checkbox("Face Camera", &FaceCamera);
    if (!FaceCamera) {
        ImGui::Text("Rotation");
        ImGui::SameLine();

        IRotator objRot = GetRotation();

        // Convert to temporary int values (to prevent writing 32bit values to 16bit variables)
        int rot[3] = {
            objRot.pitch,
            objRot.yaw,
            objRot.roll
        };

        if (ImGui::DragInt3("##Rotation", rot, 5.0f)) {
            for (size_t i = 0; i < 3; i++) {
                // Wrap around 0–65535
                rot[i] = (rot[i] % 65536 + 65536) % 65536;
            }
            IRotator newRot;
            newRot.Set(
                static_cast<uint16_t>(rot[0]),
                static_cast<uint16_t>(rot[1]),
                static_cast<uint16_t>(rot[2])
            );
            Rotate(newRot);
        }
    }

    switch(mode) {
        case STATIONARY: {
            ImGui::Text("Location");
            ImGui::SameLine();
            FVector location = GetLocation();
            if (ImGui::DragFloat3("##Location", (float*)&location)) {
                Translate(location);
                gEditor.eObjectPicker.eGizmo.Pos = location;
            }
            break;
        }
        case FOLLOW_PLAYER:
            // Allow setting PlayerIndex
            int32_t playerIdx = PlayerIndex + 1;

            // Draw the input box (ImGui input limited between 1 and 8)
            ImGui::SetNextItemWidth(100);
            if (ImGui::InputInt("Follow Player", &playerIdx)) {
                // Clamp display value to [1, 8]
                if (playerIdx < 1) playerIdx = 1;
                if (playerIdx > 8) playerIdx = 8;

                // Update the internal value (0–7)
                PlayerIndex = playerIdx - 1;
            }

            ImGui::DragFloat("Width Offset", &WidthOffset);
            ImGui::DragFloat("Height Offset", &HeightOffset);
            break;
    }

    DrawColourEditor(&updated);

    ImGui::Text("Transform Settings");
    ImGui::Separator();

    ImGui::SetNextItemWidth(100);
    ImGui::DragFloat("Scale X", &ScaleX, 0.1f, -5.0f, 5.0f, "%.2f");

    ImGui::SetNextItemWidth(100);
    if (ImGui::DragFloat("Letter Spacing", &LetterSpacing, 0.1f, 0.0f, 5.0f, "%.2f")) {
        updated = true;
    }

    if (updated) {
        Refresh();
    }
}

void AText::DrawColourEditor(bool* updated) {
    ImGui::Checkbox("Use single colour", &SingleColour);

    if (SingleColour)
    {
        // Convert 8bit colours to float
        ImVec4 colour(
            TextColour[0].r / 255.0f,
            TextColour[0].g / 255.0f,
            TextColour[0].b / 255.0f,
            TextColour[0].a / 255.0f
        );

        // Single color input
        ImGui::ColorEdit4("Colour", (float*)&colour);
            // Apply same color to all vertices
            for (int i = 0; i < 4; i++) {
                TextColour[i].r = FloatToU8(colour.x);
                TextColour[i].g = FloatToU8(colour.y);
                TextColour[i].b = FloatToU8(colour.z);
                TextColour[i].a = FloatToU8(colour.w);
            }
            *updated = true;
        
    } else {
        // Separate color pickers for each vertex
        for (int i = 0; i < 4; i++)
        {
            ImVec4 colour2(
                TextColour[i].r / 255.0f,
                TextColour[i].g / 255.0f,
                TextColour[i].b / 255.0f,
                TextColour[i].a / 255.0f
            );
            char label[32];
            snprintf(label, sizeof(label), "Vtx %d Colour", i);
            if (ImGui::ColorEdit4(label, (float*)&colour2)) {
                TextColour[i].r = FloatToU8(colour2.x);
                TextColour[i].g = FloatToU8(colour2.y);
                TextColour[i].b = FloatToU8(colour2.z);
                TextColour[i].a = FloatToU8(colour2.w);
                *updated = true;
            }
        }
    }
}
