#include "TrackProperties.h"
#include "port/ui/PortMenu.h"
#include "UIWidgets.h"
#include "libultraship/src/Context.h"

#include <imgui.h>
#include <map>
#include <libultraship/libultraship.h>
#include <spdlog/fmt/fmt.h>
#include "spdlog/formatter.h"
#include <common_structs.h>
#include <defines.h>

#include "port/Game.h"

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

        ImGui::InputText("ID", idBuffer, IM_ARRAYSIZE(idBuffer));
        ImGui::InputText("Name", gWorldInstance.CurrentCourse->Props.Name, IM_ARRAYSIZE(nameBuffer));
        ImGui::InputText("Debug Name", gWorldInstance.CurrentCourse->Props.DebugName, IM_ARRAYSIZE(debugNameBuffer));
        ImGui::InputText("Course Length", gWorldInstance.CurrentCourse->Props.CourseLength, IM_ARRAYSIZE(lengthBuffer));
        ImGui::InputFloat("Water Level", &gWorldInstance.CurrentCourse->Props.WaterLevel);

        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::InputFloat("Near Perspective", &gWorldInstance.CurrentCourse->Props.NearPersp);
            ImGui::InputFloat("Far Perspective", &gWorldInstance.CurrentCourse->Props.FarPersp);
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

            ImGui::InputFloat("AI Max Separation", &gWorldInstance.CurrentCourse->Props.AIMaximumSeparation);
            ImGui::InputFloat("AI Min Separation", &gWorldInstance.CurrentCourse->Props.AIMinimumSeparation);
            ImGui::InputInt("AI Steering Sensitivity", (int*)&gWorldInstance.CurrentCourse->Props.AISteeringSensitivity);

            ImGui::Separator();

            for (size_t i = 0; i < 32; i++) {
                ImGui::InputScalar(("Element " + std::to_string(i)).c_str(), ImGuiDataType_S16, &gWorldInstance.CurrentCourse->Props.AIDistance[i]);
            }
        }

        if (ImGui::CollapsingHeader("Random Junk")) {
            for (size_t i = 0; i < 4; i++) {
                ImGui::InputFloat(fmt::format("D_0D009418[{}]", i).c_str(), &gWorldInstance.CurrentCourse->Props.D_0D009418[i]);
            }

            ImGui::Separator();


            for (size_t i = 0; i < 4; i++) {
                ImGui::InputFloat(fmt::format("D_0D009568[{}]", i).c_str(), &gWorldInstance.CurrentCourse->Props.D_0D009568[i]);
            }

            ImGui::Separator();


            for (size_t i = 0; i < 4; i++) {
                ImGui::InputFloat(fmt::format("D_0D0096B8[{}]", i).c_str(), &gWorldInstance.CurrentCourse->Props.D_0D0096B8[i]);
            }

            ImGui::Separator();

            for (size_t i = 0; i < 4; i++) {
                ImGui::InputFloat(fmt::format("D_0D009808[{}]", i).c_str(), &gWorldInstance.CurrentCourse->Props.D_0D009808[i]);
            }
        }

        float minimapColour[3];
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Minimap.Colour, minimapColour);

        if (ImGui::CollapsingHeader("Minimap")) {
            ImGui::Text("Position");
            ImGui::SameLine();


            if (ImGui::DragInt2("##MinimapPosition", &gWorldInstance.CurrentCourse->Props.Minimap.Pos[0].X, 1.0f)) {
            }
            ImGui::Text("P2 Position");
            ImGui::SameLine();
            if (ImGui::DragInt2("##MinimapPosition2p", &gWorldInstance.CurrentCourse->Props.Minimap.Pos[1].X, 1.0f)) {
            }

            ImGui::Text("Player Markers");
            ImGui::SameLine();
            if (ImGui::DragInt2("##MinimapPlayers", &gWorldInstance.CurrentCourse->Props.Minimap.PlayerX, 1.0f)) {
            }

            ImGui::Text("Player Scale Factor");
            ImGui::SameLine();
            if (ImGui::DragFloat("##MinimapScaleFactor", &gWorldInstance.CurrentCourse->Props.Minimap.PlayerScaleFactor, 0.0001f)) {
            }

            ImGui::Text("Finishline");
            ImGui::SameLine();
            ImGui::DragFloat2("##MinimapFinishlineX", &gWorldInstance.CurrentCourse->Props.Minimap.FinishlineX, 1.0f);

            ImGui::Text("Colour");
            ImGui::SameLine();
            ImGui::ColorEdit3("##MinimapColour", minimapColour, 1.0f);
        }

        FloatToRGB8(minimapColour, (u8*)&gWorldInstance.CurrentCourse->Props.Minimap.Colour);

        // Convert and pass to ImGui ColorEdit3
        float topRight[3], bottomRight[3], bottomLeft[3], topLeft[3];
        float floorTopRight[3], floorBottomRight[3], floorBottomLeft[3], floorTopLeft[3];

        // Convert RGB8 (0-255) to float (0.0f to 1.0f)
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Skybox.TopRight, topRight);
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Skybox.BottomRight, bottomRight);
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Skybox.BottomLeft, bottomLeft);
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Skybox.TopLeft, topLeft);
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Skybox.FloorTopRight, floorTopRight);
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Skybox.FloorBottomRight, floorBottomRight);
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Skybox.FloorBottomLeft, floorBottomLeft);
        RGB8ToFloat((u8*)&gWorldInstance.CurrentCourse->Props.Skybox.FloorTopLeft, floorTopLeft);

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
        FloatToRGB8(topRight, (u8*)&gWorldInstance.CurrentCourse->Props.Skybox.TopRight);
        FloatToRGB8(bottomRight, (u8*)&gWorldInstance.CurrentCourse->Props.Skybox.BottomRight);
        FloatToRGB8(bottomLeft, (u8*)&gWorldInstance.CurrentCourse->Props.Skybox.BottomLeft);
        FloatToRGB8(topLeft, (u8*)&gWorldInstance.CurrentCourse->Props.Skybox.TopLeft);
        FloatToRGB8(floorTopRight, (u8*)&gWorldInstance.CurrentCourse->Props.Skybox.FloorTopRight);
        FloatToRGB8(floorBottomRight, (u8*)&gWorldInstance.CurrentCourse->Props.Skybox.FloorBottomRight);
        FloatToRGB8(floorBottomLeft, (u8*)&gWorldInstance.CurrentCourse->Props.Skybox.FloorBottomLeft);
        FloatToRGB8(floorTopLeft, (u8*)&gWorldInstance.CurrentCourse->Props.Skybox.FloorTopLeft);

        TrackPropertiesWindow::DrawMusic();
    }

    void TrackPropertiesWindow::DrawMusic() {
        const char* items[] = {
            "None", "Title Screen", "Main Menu", "Raceways Wario Stadium", "Moo Moo Farm / Yoshi Valley",
            "Choco Mountain", "Koopa Troopa Beach", "Banshee Boardwalk", "Frappe Snowland", "Bowser's Castle",
            "Kalimari Desert", "Start Grid GP/VS", "Final Lap Fanfare", "Finish 1st Place", "Finish 2nd-4th Place",
            "Finish 5th-8th Place", "16", "Star Jingle", "Rainbow Road", "DK Jungle", "Game Over", "Toad's Turnpike",
            "Start Grid Time Attack", "VS Battle Results", "Losing Results", "Battle Arenas", "Award Ceremony Buildup",
            "Award Ceremony 1st-3rd", "Staff Roll", "Award Ceremony 4th-8th"
        };

        const char* currentItem = MusicSeqToString(gWorldInstance.CurrentCourse->Props.Sequence); // Get the current selected value's string
    
        if (ImGui::BeginCombo("Music Sequence", currentItem)) {
            for (int i = 0; i < IM_ARRAYSIZE(items); ++i) {
                bool isSelected = (currentItem == items[i]);
                if (ImGui::Selectable(items[i], isSelected)) {
                    // Update the sequence when an option is selected
                    gWorldInstance.CurrentCourse->Props.Sequence = static_cast<MusicSeq>(i);
                    play_sequence(gWorldInstance.CurrentCourse->Props.Sequence); // Call play_sequence with the updated sequence
        
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
            case MUSIC_SEQ_00: return "None";
            case MUSIC_SEQ_TITLE_SCREEN: return "Title Screen";
            case MUSIC_SEQ_MAIN_MENU: return "Main Menu";
            case MUSIC_SEQ_RACEWAYS_WARIO_STADIUM: return "Raceways Wario Stadium";
            case MUSIC_SEQ_MOO_MOO_FARM_YOSHI_VALLEY: return "Moo Moo Farm / Yoshi Valley";
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
            case MUSIC_SEQ_16: return "16";
            case MUSIC_SEQ_STAR_JINGLE: return "Star Jingle";
            case MUSIC_SEQ_RAINBOW_ROAD: return "Rainbow Road";
            case MUSIC_SEQ_DK_JUNGLE: return "DK Jungle";
            case MUSIC_SEQ_GAME_OVER: return "Game Over";
            case MUSIC_SEQ_TOADS_TURNPIKE: return "Toad's Turnpike";
            case MUSIC_SEQ_START_GIRD_TIME_ATTACK: return "Start Grid Time Attack";
            case MUSIC_SEQ_VS_BATTLE_RESULTS: return "VS Battle Results";
            case MUSIC_SEQ_LOSING_RESULTS: return "Losing Results";
            case MUSIC_SEQ_BATTLE_ARENAS: return "Battle Arenas";
            case MUSIC_SEQ_AWARD_CEREMONY_BUILDUP: return "Award Ceremony Buildup";
            case MUSIC_SEQ_AWARD_CEREMONY_1ST_3RD: return "Award Ceremony 1st-3rd";
            case MUSIC_SEQ_STAFF_ROLL: return "Staff Roll";
            case MUSIC_SEQ_AWARD_CEREMONY_4TH_8TH: return "Award Ceremony 4th-8th";
            default: return "None";
        }
    }

    void TrackPropertiesWindow::DrawLight() {
        // Convert and pass to ImGui ColorEdit3
        
        
        for (int i = 0; i < 2; ++i) {
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
        for (int i = 0; i < 3; ++i) {
            dst[i] = src[i] / 255.0f;  // Normalize to the range [0.0f, 1.0f]
        }
    }

    void TrackPropertiesWindow::FloatToRGB8(const float* src, u8* dst) {
        for (int i = 0; i < 3; ++i) {
            dst[i] = static_cast<u8>(src[i] * 255.0f);  // Scale to [0, 255] range
        }
    }

}
