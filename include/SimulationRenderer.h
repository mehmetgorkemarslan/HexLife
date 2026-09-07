#pragma once

#include "SimulationWorld.h"
#include <raylib.h>
#include <string>
#include <vector>

namespace visualization {

    class SimulationRenderer {
    public:
        SimulationRenderer(int windowWidth = 1400, int windowHeight = 900, const std::string& title = "HexLife - Evolution Simulation");
        ~SimulationRenderer();

        /**
         * @brief Run the interactive main loop until window is closed.
         * @param world The SimulationWorld instance to step and render.
         */
        void run(eco::SimulationWorld& world);

    private:
        int screenWidth;
        int screenHeight;
        std::string windowTitle;

        Camera2D camera;
        bool is_paused = false;
        float time_scale = 1.0f;
        float sim_accumulator = 0.0f;
        const float fixed_sim_timestep = 0.02f; // 50 Hz physics/logic rate

        // Selection / Inspection
        bool has_selection = false;
        mapGen::Hex selected_hex{0, 0, 0};
        int selected_entity_id = -1;
        bool show_help = true;

        // Visual constants
        float hex_size = 28.0f;

        void handle_input(eco::SimulationWorld& world);
        void update_world(eco::SimulationWorld& world, float dt);
        void draw_frame(const eco::SimulationWorld& world);

        void draw_grid(const mapGen::HexGrid& grid);
        void draw_entities(const eco::PopulationManager& population);
        void draw_selection_highlight(const mapGen::HexGrid& grid);
        void draw_hud(const eco::SimulationWorld& world);
        void draw_inspector(const eco::SimulationWorld& world);
        void draw_help_panel();

        // Coordinate helpers
        ::Vector2 hex_to_pixel(const mapGen::Hex& hex) const;
        mapGen::Hex pixel_to_hex(::Vector2 pixel) const;
        ::Color get_species_color(int species_or_cohort_id) const;
    };

}
