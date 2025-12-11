#include "TrackProperties.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "ship/Context.h"

#include <imgui.h>
#include <map>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>

#include "port/Game.h"

#include "engine/cameras/TourCamera.h"

extern "C" {
#include "code_800029B0.h"
#include "sounds.h"
#include "external.h"
#include "render_courses.h"
#include "render_objects.h"
}

namespace Editor {

    TrackPropertiesWindow::~TrackPropertiesWindow() {
        SPDLOG_TRACE("destruct track properties window");
    }

    void TrackPropertiesWindow::DrawElement() {
        static char idBuffer[256] = "mk:mario_raceway";
        static char nameBuffer[256] = "Mario Raceway";
        static char debugNameBuffer[256] = "m circuit";
        static char lengthBuffer[256] = "567m";

        if (nullptr == GetWorld()->GetTrack()) {
            return;
        }

        ImGui::InputText("ID", idBuffer, IM_ARRAYSIZE(idBuffer));
        ImGui::InputText("Name", GetWorld()->GetTrack()->Props.Name, IM_ARRAYSIZE(nameBuffer));
        ImGui::InputText("Debug Name", GetWorld()->GetTrack()->Props.DebugName, IM_ARRAYSIZE(debugNameBuffer));
        ImGui::InputText("Track Length", GetWorld()->GetTrack()->Props.TrackLength, IM_ARRAYSIZE(lengthBuffer));
        ImGui::InputFloat("Water Level", &GetWorld()->GetTrack()->Props.WaterLevel);

        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::InputFloat("Near Perspective", &GetWorld()->GetTrack()->Props.NearPersp);
            ImGui::InputFloat("Far Perspective", &GetWorld()->GetTrack()->Props.FarPersp);
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Controls the far clipping distance for perspective rendering.");
                ImGui::Text("Disable culling enhancement needs to be off to see a difference.");
                ImGui::EndTooltip();
            }
        }

        if (ImGui::CollapsingHeader("Environment")) {
            TrackPropertiesWindow::DrawLight();
        }

        if (ImGui::CollapsingHeader("AI")) {

            ImGui::InputFloat("AI Max Separation", &GetWorld()->GetTrack()->Props.AIMaximumSeparation);
            ImGui::InputFloat("AI Min Separation", &GetWorld()->GetTrack()->Props.AIMinimumSeparation);
            ImGui::InputInt("AI Steering Sensitivity", (int*)&GetWorld()->GetTrack()->Props.AISteeringSensitivity);

            ImGui::Separator();

            for (size_t i = 0; i < 32; i++) {
                ImGui::InputScalar(("Element " + std::to_string(i)).c_str(), ImGuiDataType_S16, &GetWorld()->GetTrack()->Props.AIDistance[i]);
            }
        }

        if (ImGui::CollapsingHeader("Random Junk")) {
            for (size_t i = 0; i < 4; i++) {
                ImGui::InputFloat(fmt::format("CurveTargetSpeed[{}]", i).c_str(), &GetWorld()->GetTrack()->Props.CurveTargetSpeed[i]);
            }

            ImGui::Separator();


            for (size_t i = 0; i < 4; i++) {
                ImGui::InputFloat(fmt::format("NormalTargetSpeed[{}]", i).c_str(), &GetWorld()->GetTrack()->Props.NormalTargetSpeed[i]);
            }

            ImGui::Separator();


            for (size_t i = 0; i < 4; i++) {
                ImGui::InputFloat(fmt::format("D_0D0096B8[{}]", i).c_str(), &GetWorld()->GetTrack()->Props.D_0D0096B8[i]);
            }

            ImGui::Separator();

            for (size_t i = 0; i < 4; i++) {
                ImGui::InputFloat(fmt::format("OffTrackTargetSpeed[{}]", i).c_str(), &GetWorld()->GetTrack()->Props.OffTrackTargetSpeed[i]);
            }
        }

        float minimapColour[3];
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Minimap.Colour, minimapColour);

        if (ImGui::CollapsingHeader("Minimap")) {
            ImGui::Text("Position");
            ImGui::SameLine();


            if (ImGui::DragInt2("##MinimapPosition", &GetWorld()->GetTrack()->Props.Minimap.Pos[0].X, 1.0f)) {
            }
            ImGui::Text("P2 Position");
            ImGui::SameLine();
            if (ImGui::DragInt2("##MinimapPosition2p", &GetWorld()->GetTrack()->Props.Minimap.Pos[1].X, 1.0f)) {
            }

            ImGui::Text("Player Markers");
            ImGui::SameLine();
            if (ImGui::DragInt2("##MinimapPlayers", &GetWorld()->GetTrack()->Props.Minimap.PlayerX, 1.0f)) {
            }

            ImGui::Text("Player Scale Factor");
            ImGui::SameLine();
            if (ImGui::DragFloat("##MinimapScaleFactor", &GetWorld()->GetTrack()->Props.Minimap.PlayerScaleFactor, 0.0001f)) {
            }

            ImGui::Text("Finishline");
            ImGui::SameLine();
            ImGui::DragFloat2("##MinimapFinishlineX", &GetWorld()->GetTrack()->Props.Minimap.FinishlineX, 1.0f);

            ImGui::Text("Colour");
            ImGui::SameLine();
            ImGui::ColorEdit3("##MinimapColour", minimapColour, 1.0f);
        }

        FloatToRGB8(minimapColour, (u8*)&GetWorld()->GetTrack()->Props.Minimap.Colour);

        // Convert and pass to ImGui ColorEdit3
        float topRight[3], bottomRight[3], bottomLeft[3], topLeft[3];
        float floorTopRight[3], floorBottomRight[3], floorBottomLeft[3], floorTopLeft[3];

        // Convert RGB8 (0-255) to float (0.0f to 1.0f)
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Skybox.TopRight, topRight);
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Skybox.BottomRight, bottomRight);
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Skybox.BottomLeft, bottomLeft);
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Skybox.TopLeft, topLeft);
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Skybox.FloorTopRight, floorTopRight);
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Skybox.FloorBottomRight, floorBottomRight);
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Skybox.FloorBottomLeft, floorBottomLeft);
        RGB8ToFloat((u8*)&GetWorld()->GetTrack()->Props.Skybox.FloorTopLeft, floorTopLeft);

        if (ImGui::CollapsingHeader("Skybox")) {
            ImGui::ColorEdit3("Skybox Top Right", topRight);
            ImGui::ColorEdit3("Skybox Bottom Right", bottomRight);
            ImGui::ColorEdit3("Skybox Bottom Left", bottomLeft);
            ImGui::ColorEdit3("Skybox Top Left", topLeft);
            ImGui::ColorEdit3("Skybox Floor Top Right", floorTopRight);
            ImGui::ColorEdit3("Skybox Floor Bottom Right", floorBottomRight);
            ImGui::ColorEdit3("Skybox Floor Bottom Left", floorBottomLeft);
            ImGui::ColorEdit3("Skybox Floor Top Left", floorTopLeft);
        }

        // Convert the modified float values back to RGB8 (0-255)
        FloatToRGB8(topRight, (u8*)&GetWorld()->GetTrack()->Props.Skybox.TopRight);
        FloatToRGB8(bottomRight, (u8*)&GetWorld()->GetTrack()->Props.Skybox.BottomRight);
        FloatToRGB8(bottomLeft, (u8*)&GetWorld()->GetTrack()->Props.Skybox.BottomLeft);
        FloatToRGB8(topLeft, (u8*)&GetWorld()->GetTrack()->Props.Skybox.TopLeft);
        FloatToRGB8(floorTopRight, (u8*)&GetWorld()->GetTrack()->Props.Skybox.FloorTopRight);
        FloatToRGB8(floorBottomRight, (u8*)&GetWorld()->GetTrack()->Props.Skybox.FloorBottomRight);
        FloatToRGB8(floorBottomLeft, (u8*)&GetWorld()->GetTrack()->Props.Skybox.FloorBottomLeft);
        FloatToRGB8(floorTopLeft, (u8*)&GetWorld()->GetTrack()->Props.Skybox.FloorTopLeft);

        TrackPropertiesWindow::DrawMusic();
        TrackPropertiesWindow::DrawTourCamera();
    }

    void TrackPropertiesWindow::DrawMusic() {
        const char* items[] = {
            "None", "Title Screen", "Main Menu", "Wario Stadium", "Moo Moo Farm",
            "Choco Mountain", "Koopa Troopa Beach", "Banshee Boardwalk", "Frappe Snowland", "Bowser's Castle",
            "Kalimari Desert", "Start Grid GP/VS", "Final Lap Fanfare", "Finish 1st Place", "Finish 2nd-4th Place",
            "Finish 5th-8th Place", "16", "Star Jingle", "Rainbow Road", "DK Jungle", "Game Over", "Toad's Turnpike",
            "Start Grid Time Attack", "VS Battle Results", "Losing Results", "Battle Arenas", "Award Ceremony Buildup",
            "Award Ceremony 1st-3rd", "Staff Roll", "Award Ceremony 4th-8th", "Luigi Raceway", "Mario Raceway",
            "Royal Raceway", "Yoshi Valley", "Block Fort", "Double Deck"
        };

        const char* currentItem = MusicSeqToString(GetWorld()->GetTrack()->Props.Sequence); // Get the current selected value's string
    
        if (ImGui::BeginCombo("Music Sequence", currentItem)) {
            for (size_t i = 0; i < IM_ARRAYSIZE(items); ++i) {
                bool isSelected = (currentItem == items[i]);
                if (ImGui::Selectable(items[i], isSelected)) {
                    // Update the sequence when an option is selected
                    GetWorld()->GetTrack()->Props.Sequence = static_cast<MusicSeq>(i);
                    play_sequence(GetWorld()->GetTrack()->Props.Sequence); // Call play_sequence with the updated sequence

                    // Update currentItem after selection is made
                    currentItem = items[i];
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    // Enum to string conversion
    const char* TrackPropertiesWindow::MusicSeqToString(MusicSeq seq) {
        switch (seq) {
            case MUSIC_SEQ_SOUND_PLAYER: return "Sound FX Player (Don't use)";
            case MUSIC_SEQ_TITLE_SCREEN: return "Title Screen";
            case MUSIC_SEQ_MAIN_MENU: return "Main Menu";
            case MUSIC_SEQ_WARIO_STADIUM: return "Wario Stadium";
            case MUSIC_SEQ_MOO_MOO_FARM: return "Moo Moo Farm";
            case MUSIC_SEQ_CHOCO_MOUNTAIN: return "Choco Mountain";
            case MUSIC_SEQ_KOOPA_TROOPA_BEACH: return "Koopa Troopa Beach";
            case MUSIC_SEQ_BANSHEE_BOARDWALK: return "Banshee Boardwalk";
            case MUSIC_SEQ_FRAPPE_SNOWLAND: return "Frappe Snowland";
            case MUSIC_SEQ_BOWSERS_CASTLE: return "Bowser's Castle";
            case MUSIC_SEQ_KALIMARI_DESERT: return "Kalimari Desert";
            case MUSIC_SEQ_START_GRID_GP_VS: return "Start Grid GP/VS";
            case MUSIC_SEQ_FINAL_LAP_FANFARE: return "Final Lap Fanfare";
            case MUSIC_SEQ_FINISH_1ST_PLACE: return "Finish 1st Place";
            case MUSIC_SEQ_FINISH_2ND_4TH_PLACE: return "Finish 2nd-4th Place";
            case MUSIC_SEQ_FINISH_5TH_8TH_PLACE: return "Finish 5th-8th Place";
            case MUSIC_SEQ_WINNING_RESULTS: return "Winning Results";
            case MUSIC_SEQ_STAR_JINGLE: return "Star Jingle";
            case MUSIC_SEQ_RAINBOW_ROAD: return "Rainbow Road";
            case MUSIC_SEQ_DK_JUNGLE: return "DK Jungle";
            case MUSIC_SEQ_GAME_OVER: return "Game Over";
            case MUSIC_SEQ_TOADS_TURNPIKE: return "Toad's Turnpike";
            case MUSIC_SEQ_START_GRID_TIME_ATTACK: return "Start Grid Time Attack";
            case MUSIC_SEQ_VS_BATTLE_RESULTS: return "VS Battle Results";
            case MUSIC_SEQ_LOSING_RESULTS: return "Losing Results";
            case MUSIC_SEQ_BATTLE_ARENAS: return "Battle Arenas";
            case MUSIC_SEQ_AWARD_CEREMONY_BUILDUP: return "Award Ceremony Buildup";
            case MUSIC_SEQ_AWARD_CEREMONY_1ST_3RD: return "Award Ceremony 1st-3rd";
            case MUSIC_SEQ_STAFF_ROLL: return "Staff Roll";
            case MUSIC_SEQ_AWARD_CEREMONY_4TH_8TH: return "Award Ceremony 4th-8th";
            case MUSIC_SEQ_LUIGI_RACEWAY: return "Luigi Raceway";
            case MUSIC_SEQ_MARIO_RACEWAY: return "Mario Raceway";
            case MUSIC_SEQ_ROYAL_RACEWAY: return "Royal Raceway";
            case MUSIC_SEQ_YOSHI_VALLEY: return "Yoshi Valley";
            case MUSIC_SEQ_BLOCK_FORT: return "Block Fort";
            case MUSIC_SEQ_DOUBLE_DECK: return "Double Deck";
            default: return "None";
        }
    }

    void TrackPropertiesWindow::DrawLight() {
        // Convert and pass to ImGui ColorEdit3
        
        
        for (size_t i = 0; i < 2; ++i) {
            float ambient[3], diffuse[3], direction[3];

            RGB8ToFloat((u8*)&D_800DC610[i].a.l.col, ambient);
            RGB8ToFloat((u8*)&D_800DC610[i].l->l.col, diffuse);
            RGB8ToFloat((u8*)&D_800DC610[i].l->l.dir, direction);

            // Edit the ambient RGB color
            ImGui::Text("Light %d - Ambient Color", i + 1);
            ImGui::ColorEdit3(("Ambient Color " + std::to_string(i)).c_str(), ambient); // Modify ambient color
    
            // Edit the diffuse RGB color
            ImGui::Text("Light %d - Diffuse Color", i + 1);
            ImGui::ColorEdit3(("Diffuse Color " + std::to_string(i)).c_str(), diffuse); // Modify diffuse color
    
            // Edit the direction RGB color (this could be represented as a direction vector)
            ImGui::Text("Light %d - Direction", i + 1);
            ImGui::ColorEdit3(("Direction Color " + std::to_string(i)).c_str(), direction); // Modify direction vector color

            FloatToRGB8(ambient, (u8*)&D_800DC610[i].a.l.col);
            FloatToRGB8(ambient, (u8*)&D_800DC610[i].a.l.colc);
            FloatToRGB8(diffuse, (u8*)&D_800DC610[i].l->l.col);
            FloatToRGB8(diffuse, (u8*)&D_800DC610[i].l->l.colc);
            FloatToRGB8(direction, (u8*)&D_800DC610[i].l->l.dir);

        }
    }

    // Convert s16 color values to float (normalized to [0, 1] range)
    void TrackPropertiesWindow::RGB8ToFloat(const u8* src, float* dst) {
        for (size_t i = 0; i < 3; ++i) {
            dst[i] = src[i] / 255.0f;  // Normalize to the range [0.0f, 1.0f]
        }
    }

    void TrackPropertiesWindow::FloatToRGB8(const float* src, u8* dst) {
        for (size_t i = 0; i < 3; ++i) {
            dst[i] = static_cast<u8>(src[i] * 255.0f);  // Scale to [0, 255] range
        }
    }

    int32_t TrackPropertiesWindow::SelectedShot = -1;
    int32_t TrackPropertiesWindow::SelectedKeyframe = -1;

    void TrackPropertiesWindow::DrawTourCamera() {

        Track* track = GetWorld()->GetTrack();
        if (nullptr == track) {
            return;
        }

        Camera* camera = gScreenOneCtx->camera;
        if (nullptr == camera) {
            return;
        }


        FVector camPos = FVector(camera->pos[0], camera->pos[1], camera->pos[2]);
        FVector camLookAt = FVector(camera->lookAt[0], camera->lookAt[1], camera->lookAt[2]);

        // Enable / disable
        ImGui::Checkbox("Enable Tour Camera", &track->bTourEnabled);

        if (!track->bTourEnabled) {
            return;
        }

        ImGui::SeparatorText("Tour Camera Controls");

        // Button to add a new empty shot
        if (ImGui::Button("Add CameraShot")) {
            TourCamera::CameraShot shot;
            shot.Pos = camPos;       // start where the camera currently is
            shot.LookAt = camLookAt; // or however your camera exposes these
            track->TourShots.push_back(shot);
        }

        ImGui::Spacing();

        // ============================
        //   Display Camera Shots
        // ============================
        for (int32_t i = 0; i < track->TourShots.size(); i++)
        {
            TourCamera::CameraShot& shot = track->TourShots[i];
            ImGui::PushID(i);

            bool open = ImGui::CollapsingHeader(
                ("CameraShot " + std::to_string(i)).c_str(),
                ImGuiTreeNodeFlags_DefaultOpen
            );

            // Select the shot
            if (ImGui::Selectable("Select Shot", SelectedShot == i)) {
                SelectedShot = i;
            }

            // Delete shot
            ImGui::SameLine();
            if (ImGui::Button("Delete Shot")) {
                track->TourShots.erase(track->TourShots.begin() + i);
                if (SelectedShot == i) SelectedShot = -1;
                ImGui::PopID();
                break;
            }

            if (open) {
                ImGui::Indent();

                // Start pos / lookAt editing
                ImGui::InputFloat3("Start Pos", &shot.Pos.x);
                ImGui::InputFloat3("Start LookAt", &shot.LookAt.x);

                ImGui::Spacing();

                // Record keyframe from current camera
                if (ImGui::Button("Record KeyFrame From Camera")) {
                    TourCamera::KeyFrame kf;
                    kf.Pos = camPos;
                    kf.LookAt = camLookAt;
                    kf.Duration = 60.0f;
                    shot.Frames.push_back(kf);
                }

                ImGui::SeparatorText("KeyFrames");

                // ============================
                //   KeyFrame List
                // ============================
                for (int32_t k = 0; k < shot.Frames.size(); k++)
                {
                    TourCamera::KeyFrame& kf = shot.Frames[k];
                    ImGui::PushID(k);

                    bool kOpen = ImGui::TreeNode(("KeyFrame " + std::to_string(k)).c_str());

                    // Select keyframe
                    if (ImGui::Selectable("Select KeyFrame", SelectedKeyframe == k))
                        SelectedKeyframe = k;

                    ImGui::SameLine();
                    if (ImGui::Button("Delete")) {
                        shot.Frames.erase(shot.Frames.begin() + k);
                        if (SelectedKeyframe == k) SelectedKeyframe = -1;
                        ImGui::PopID();
                        break;
                    }

                    if (kOpen) {
                        ImGui::InputFloat3("Position", &kf.Pos.x);
                        ImGui::InputFloat3("LookAt", &kf.LookAt.x);
                        ImGui::DragFloat("Duration", &kf.Duration, 1.0f, 1.0f, 5000.0f);
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }

                ImGui::Unindent();
            }

            ImGui::PopID();
        }

    }

}
