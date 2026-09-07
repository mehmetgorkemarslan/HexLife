#include "SimulationRenderer.h"
#include "BiomeHandler.h"
#include <cmath>
#include <format>
#include <iostream>
#include <sstream>

namespace visualization {

    namespace {
        constexpr float SQRT3 = 1.7320508075688772f;

        ::Color hex_to_raylib_color(const mapGen::Color& c, unsigned char alpha = 255) {
            return ::Color{ c.r, c.g, c.b, alpha };
        }
    }

    SimulationRenderer::SimulationRenderer(int windowWidth, int windowHeight, const std::string& title)
        : screenWidth(windowWidth), screenHeight(windowHeight), windowTitle(title) {
        
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
        InitWindow(screenWidth, screenHeight, windowTitle.c_str());
        SetTargetFPS(60);

        camera.target = ::Vector2{ 0.0f, 0.0f };
        camera.offset = ::Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
    }

    SimulationRenderer::~SimulationRenderer() {
        if (IsWindowReady()) {
            CloseWindow();
        }
    }

    ::Vector2 SimulationRenderer::hex_to_pixel(const mapGen::Hex& hex) const {
        float x = hex_size * SQRT3 * (hex.q + hex.r / 2.0f);
        float y = hex_size * 1.5f * hex.r;
        return ::Vector2{ x, y };
    }

    mapGen::Hex SimulationRenderer::pixel_to_hex(::Vector2 pixel) const {
        float q_frac = (SQRT3 / 3.0f * pixel.x - 1.0f / 3.0f * pixel.y) / hex_size;
        float r_frac = (2.0f / 3.0f * pixel.y) / hex_size;
        float s_frac = -q_frac - r_frac;

        int q = static_cast<int>(std::round(q_frac));
        int r = static_cast<int>(std::round(r_frac));
        int s = static_cast<int>(std::round(s_frac));

        float q_diff = std::abs(q - q_frac);
        float r_diff = std::abs(r - r_frac);
        float s_diff = std::abs(s - s_frac);

        if (q_diff > r_diff && q_diff > s_diff) {
            q = -r - s;
        } else if (r_diff > s_diff) {
            r = -q - s;
        } else {
            s = -q - r;
        }

        return mapGen::Hex{ q, r, s };
    }

    ::Color SimulationRenderer::get_species_color(int id) const {
        // Generate visually distinct and pleasant palette
        static const ::Color palette[] = {
            ::Color{ 255, 87, 87, 255 },    // Vibrant Red
            ::Color{ 66, 165, 245, 255 },   // Sky Blue
            ::Color{ 102, 187, 106, 255 },  // Mint Green
            ::Color{ 255, 167, 38, 255 },   // Orange
            ::Color{ 171, 71, 188, 255 },   // Purple
            ::Color{ 255, 238, 88, 255 },   // Yellow
            ::Color{ 38, 198, 218, 255 },   // Cyan
            ::Color{ 236, 64, 122, 255 },   // Pink
            ::Color{ 141, 110, 99, 255 },   // Amber Brown
            ::Color{ 120, 144, 156, 255 }   // Blue Grey
        };
        int num_colors = sizeof(palette) / sizeof(palette[0]);
        int idx = std::abs(id) % num_colors;
        return palette[idx];
    }

    void SimulationRenderer::handle_input(eco::SimulationWorld& world) {
        // Update screen dimensions if resized
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        // 1. Mouse Drag Panning (Right Mouse Button or Middle Mouse Button or Shift + Left)
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) ||
            (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT))) {
            ::Vector2 delta = GetMouseDelta();
            camera.target.x -= delta.x / camera.zoom;
            camera.target.y -= delta.y / camera.zoom;
        }

        // 2. Mouse Wheel Zoom (relative to mouse cursor)
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            ::Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.target = mouseWorldPos;

            float zoomFactor = 1.0f + (wheel * 0.15f);
            camera.zoom *= zoomFactor;
            if (camera.zoom < 0.15f) camera.zoom = 0.15f;
            if (camera.zoom > 5.0f) camera.zoom = 5.0f;
        }

        // 3. Reset Camera
        if (IsKeyPressed(KEY_R)) {
            camera.target = ::Vector2{ 0.0f, 0.0f };
            camera.offset = ::Vector2{ screenWidth / 2.0f, screenHeight / 2.0f };
            camera.zoom = 1.0f;
        }

        // 4. Pause / Resume Toggle
        if (IsKeyPressed(KEY_SPACE)) {
            is_paused = !is_paused;
        }

        // 5. Step single frame when paused
        if (is_paused && IsKeyPressed(KEY_N)) {
            world.step(fixed_sim_timestep);
        }

        // 6. Time Multipliers
        if (IsKeyPressed(KEY_ONE)) time_scale = 0.5f;
        if (IsKeyPressed(KEY_TWO)) time_scale = 1.0f;
        if (IsKeyPressed(KEY_THREE)) time_scale = 2.0f;
        if (IsKeyPressed(KEY_FOUR)) time_scale = 5.0f;
        if (IsKeyPressed(KEY_FIVE)) time_scale = 10.0f;

        // 7. Toggle Help
        if (IsKeyPressed(KEY_H)) {
            show_help = !show_help;
        }

        // 8. Hex / Entity Selection on Left Click
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !IsKeyDown(KEY_LEFT_SHIFT)) {
            ::Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
            mapGen::Hex clickedHex = pixel_to_hex(mouseWorld);

            // Check if clicked inside world bounds
            auto& map = world.get_grid().getMap();
            if (map.find(clickedHex) != map.end()) {
                selected_hex = clickedHex;
                has_selection = true;

                // Check if any entity is on this hex
                selected_entity_id = -1;
                for (const auto& entity : world.get_population().get_entities()) {
                    if (entity && entity->getAlive() && entity->getPosition() == clickedHex) {
                        selected_entity_id = entity->getId();
                        break;
                    }
                }
            } else {
                has_selection = false;
                selected_entity_id = -1;
            }
        }
    }

    void SimulationRenderer::update_world(eco::SimulationWorld& world, float dt) {
        if (is_paused) return;

        sim_accumulator += dt * time_scale;
        while (sim_accumulator >= fixed_sim_timestep) {
            world.step(fixed_sim_timestep);
            sim_accumulator -= fixed_sim_timestep;
        }
    }

    void SimulationRenderer::draw_grid(const mapGen::HexGrid& grid) {
        const auto& map = grid.getMap();

        for (const auto& [hex, cell] : map) {
            ::Vector2 center = hex_to_pixel(hex);

            // Biome Color
            mapGen::Color biomeColor = mapGen::BiomeHandler::getBiomeById(cell.getBiomeID()).getColor();
            ::Color fillCol = hex_to_raylib_color(biomeColor, 245);

            // Draw pointy-topped hexagon (30 deg rotation in raylib)
            DrawPoly(center, 6, hex_size, 30.0f, fillCol);

            // Draw Hex Border
            ::Color borderCol = ::Color{
                static_cast<unsigned char>(biomeColor.r > 40 ? biomeColor.r - 40 : 0),
                static_cast<unsigned char>(biomeColor.g > 40 ? biomeColor.g - 40 : 0),
                static_cast<unsigned char>(biomeColor.b > 40 ? biomeColor.b - 40 : 0),
                200
            };
            DrawPolyLinesEx(center, 6, hex_size, 30.0f, 1.2f, borderCol);
        }
    }

    void SimulationRenderer::draw_entities(const eco::PopulationManager& population) {
        const auto& entities = population.get_entities();
        
        // Count entities per hex cell for neat radial distribution
        std::unordered_map<mapGen::Hex, std::vector<const eco::Entity*>, mapGen::HexHash> cell_entities;
        for (const auto& entity : entities) {
            if (entity && entity->getAlive()) {
                cell_entities[entity->getPosition()].push_back(entity.get());
            }
        }

        for (const auto& [hex, list] : cell_entities) {
            ::Vector2 cellCenter = hex_to_pixel(hex);
            size_t count = list.size();

            for (size_t i = 0; i < count; ++i) {
                const auto* entity = list[i];
                ::Vector2 pos = cellCenter;

                if (count > 1) {
                    float angle = (2.0f * 3.14159265f * i) / count;
                    float offsetDist = hex_size * 0.45f;
                    pos.x += std::cos(angle) * offsetDist;
                    pos.y += std::sin(angle) * offsetDist;
                }

                // Entity Color based on species / cohort
                ::Color entColor = get_species_color(entity->getCohort());
                
                // Draw drop shadow / glow
                DrawCircleV(pos, 6.5f, ::Color{ 0, 0, 0, 100 });
                // Draw Entity body
                DrawCircleV(pos, 5.0f, entColor);
                // Draw highlight border
                DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), 5.0f, WHITE);

                // Energy indicator (inner dot)
                float energyRatio = entity->getEnergy() / std::max(1.0f, entity->getMaxEnergy());
                if (energyRatio > 0.6f) {
                    DrawCircleV(pos, 2.0f, GREEN);
                } else if (energyRatio > 0.25f) {
                    DrawCircleV(pos, 2.0f, YELLOW);
                } else {
                    DrawCircleV(pos, 2.0f, RED);
                }

                // If selected, draw highlight ring
                if (has_selection && entity->getId() == selected_entity_id) {
                    float pulse = (std::sin(GetTime() * 6.0f) + 1.0f) * 0.5f;
                    DrawCircleLines(static_cast<int>(pos.x), static_cast<int>(pos.y), 9.0f + pulse * 3.0f, YELLOW);
                }
            }
        }
    }

    void SimulationRenderer::draw_selection_highlight(const mapGen::HexGrid& grid) {
        if (!has_selection) return;

        ::Vector2 center = hex_to_pixel(selected_hex);
        float pulse = (std::sin(GetTime() * 5.0f) + 1.0f) * 0.5f;
        ::Color highlightCol = ::Color{ 255, 235, 59, static_cast<unsigned char>(180 + pulse * 75) };
        DrawPolyLinesEx(center, 6, hex_size + 1.0f, 30.0f, 2.5f, highlightCol);
    }

    void SimulationRenderer::draw_hud(const eco::SimulationWorld& world) {
        int panelWidth = 330;
        int panelHeight = 160;
        int margin = 16;

        // Top Left Glassmorphic HUD
        DrawRectangleRounded(::Rectangle{ static_cast<float>(margin), static_cast<float>(margin), static_cast<float>(panelWidth), static_cast<float>(panelHeight) }, 0.12f, 8, ::Color{ 20, 24, 34, 220 });
        DrawRectangleRoundedLinesEx(::Rectangle{ static_cast<float>(margin), static_cast<float>(margin), static_cast<float>(panelWidth), static_cast<float>(panelHeight) }, 0.12f, 8, 1.5f, ::Color{ 60, 70, 95, 255 });

        int textX = margin + 14;
        int textY = margin + 12;
        int lineSpacing = 22;

        DrawText("HexLife Eco-Simulation", textX, textY, 18, ::Color{ 255, 255, 255, 255 });
        textY += lineSpacing + 4;

        std::string timeStr = std::format("Sim Time: {:.2f}s  (Frame {})", world.get_elapsed_time(), world.get_frame_count());
        DrawText(timeStr.c_str(), textX, textY, 14, ::Color{ 180, 200, 220, 255 });
        textY += lineSpacing;

        const auto& pop = world.get_population();
        std::string popStr = std::format("Entities: {}  |  Species: {}  |  Gen: {}", pop.get_population_size(), pop.get_species_count(), pop.get_generation_count());
        DrawText(popStr.c_str(), textX, textY, 14, ::Color{ 120, 220, 140, 255 });
        textY += lineSpacing;

        std::string stateStr = is_paused ? "PAUSED (Press [Space] to Resume)" : std::format("Speed: {:.1f}x (1-5 to change)", time_scale);
        DrawText(stateStr.c_str(), textX, textY, 14, is_paused ? ::Color{ 255, 100, 100, 255 } : ::Color{ 255, 200, 80, 255 });
        textY += lineSpacing;

        std::string fpsStr = std::format("FPS: {}  |  Zoom: {:.1f}x", GetFPS(), camera.zoom);
        DrawText(fpsStr.c_str(), textX, textY, 12, ::Color{ 140, 150, 170, 255 });
    }

    void SimulationRenderer::draw_inspector(const eco::SimulationWorld& world) {
        if (!has_selection) return;

        const auto& map = world.get_grid().getMap();
        auto it = map.find(selected_hex);
        if (it == map.end()) return;

        const auto& cell = it->second;
        auto& biome = mapGen::BiomeHandler::getBiomeById(cell.getBiomeID());

        int panelWidth = 320;
        int panelHeight = 220;
        int posX = screenWidth - panelWidth - 16;
        int posY = 16;

        DrawRectangleRounded(::Rectangle{ static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(panelWidth), static_cast<float>(panelHeight) }, 0.12f, 8, ::Color{ 20, 24, 34, 220 });
        DrawRectangleRoundedLinesEx(::Rectangle{ static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(panelWidth), static_cast<float>(panelHeight) }, 0.12f, 8, 1.5f, ::Color{ 60, 70, 95, 255 });

        int textX = posX + 14;
        int textY = posY + 12;
        int lineSpacing = 20;

        DrawText("Hex Cell Inspector", textX, textY, 16, WHITE);
        textY += lineSpacing + 4;

        std::string coordStr = std::format("Coord: ({}, {}, {})", selected_hex.q, selected_hex.r, selected_hex.s);
        DrawText(coordStr.c_str(), textX, textY, 13, ::Color{ 200, 210, 230, 255 });
        textY += lineSpacing;

        std::string biomeStr = std::format("Biome: {}", biome.getName());
        DrawText(biomeStr.c_str(), textX, textY, 13, hex_to_raylib_color(biome.getColor()));
        textY += lineSpacing;

        std::string tempMoistStr = std::format("Temp: {:.2f}  |  Moisture: {:.2f}", cell.temperature, cell.moisture);
        DrawText(tempMoistStr.c_str(), textX, textY, 13, ::Color{ 180, 200, 220, 255 });
        textY += lineSpacing;

        std::string entCountStr = std::format("Entities on Tile: {}", cell.entities.size());
        DrawText(entCountStr.c_str(), textX, textY, 13, ::Color{ 120, 220, 140, 255 });
        textY += lineSpacing;

        // If an entity was found
        if (selected_entity_id != -1) {
            for (const auto& ent : world.get_population().get_entities()) {
                if (ent && ent->getId() == selected_entity_id) {
                    DrawLine(textX, textY + 2, posX + panelWidth - 14, textY + 2, ::Color{ 80, 90, 120, 255 });
                    textY += 8;
                    std::string entInfo = std::format("Entity #{}: Cohort {} (Alive: {:.1f}s)", ent->getId(), ent->getCohort(), ent->getTimeAlive());
                    DrawText(entInfo.c_str(), textX, textY, 13, YELLOW);
                    textY += lineSpacing;
                    std::string statsInfo = std::format("Energy: {:.1f}/{}  Fit: {:.1f}", ent->getEnergy(), ent->getMaxEnergy(), ent->get_fitness());
                    DrawText(statsInfo.c_str(), textX, textY, 12, ::Color{ 200, 220, 240, 255 });
                    break;
                }
            }
        }
    }

    void SimulationRenderer::draw_help_panel() {
        if (!show_help) return;

        int panelWidth = 420;
        int panelHeight = 100;
        int posX = (screenWidth - panelWidth) / 2;
        int posY = screenHeight - panelHeight - 16;

        DrawRectangleRounded(::Rectangle{ static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(panelWidth), static_cast<float>(panelHeight) }, 0.12f, 8, ::Color{ 15, 18, 26, 200 });
        DrawRectangleRoundedLinesEx(::Rectangle{ static_cast<float>(posX), static_cast<float>(posY), static_cast<float>(panelWidth), static_cast<float>(panelHeight) }, 0.12f, 8, 1.0f, ::Color{ 50, 60, 80, 200 });

        int textX = posX + 14;
        int textY = posY + 10;

        DrawText("Keybindings & Navigation (Press [H] to hide)", textX, textY, 13, ::Color{ 220, 220, 220, 255 });
        textY += 18;
        DrawText("- [Space]: Pause / Resume   |  [N]: Step 1 frame", textX, textY, 12, ::Color{ 160, 180, 200, 255 });
        textY += 16;
        DrawText("- [1 - 5]: Simulation Speed (0.5x - 10x)  |  [R]: Reset View", textX, textY, 12, ::Color{ 160, 180, 200, 255 });
        textY += 16;
        DrawText("- Right-Click Drag: Pan Camera  |  Scroll Wheel: Zoom", textX, textY, 12, ::Color{ 160, 180, 200, 255 });
    }

    void SimulationRenderer::draw_frame(const eco::SimulationWorld& world) {
        BeginDrawing();
        ClearBackground(::Color{ 18, 22, 32, 255 }); // Dark sleek background

        // 2D World Space
        BeginMode2D(camera);
        {
            draw_grid(world.get_grid());
            draw_selection_highlight(world.get_grid());
            draw_entities(world.get_population());
        }
        EndMode2D();

        // Screen Space HUD & UI Overlays
        draw_hud(world);
        draw_inspector(world);
        draw_help_panel();

        EndDrawing();
    }

    void SimulationRenderer::run(eco::SimulationWorld& world) {
        while (!WindowShouldClose()) {
            float dt = GetFrameTime();
            if (dt > 0.1f) dt = 0.1f; // Cap delta time during lag spikes

            handle_input(world);
            update_world(world, dt);
            draw_frame(world);
        }
    }

}
