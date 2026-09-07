#include "Test.h"

// Explicitly include the genotype and phenotype headers
#include "genotype/Gene.h"
#include "genotype/Genome.h"
#include "phenotype/Network.h"
#include "phenotype/Node.h"
#include "phenotype/Connection.h"

// Include Entity and Map system
#include "eco/Entity.h"
#include "MapSystem/Hex.h"
#include "MapSystem/HexGrid.h"
#include "Common.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <memory>

void run_neat_diagnostics_test() {
    std::cout << "==========================================\n";
    std::cout << "   RUNNING NEAT PHENOTYPE VALIDATION TEST \n";
    std::cout << "==========================================\n";

    // 1. Manually construct a valid Genome layout
    neat::Genome genome;

    // Add Nodes (2 Inputs, 1 Bias, 1 Hidden, 1 Output)
    genome.nodes.push_back(neat::NodeGene(0, neat::NodeType::INPUT));
    genome.nodes.push_back(neat::NodeGene(1, neat::NodeType::INPUT));
    genome.nodes.push_back(neat::NodeGene(2, neat::NodeType::BIAS));
    genome.nodes.push_back(neat::NodeGene(3, neat::NodeType::HIDDEN));
    genome.nodes.push_back(neat::NodeGene(4, neat::NodeType::OUTPUT));

    // Add Connections explicitly using the full constructor path
    genome.connections.push_back(neat::ConnectionGene(0, 3, 1, 0.5, true));  // Input 1 -> Hidden
    genome.connections.push_back(neat::ConnectionGene(1, 3, 2, -0.5, true)); // Input 2 -> Hidden
    genome.connections.push_back(neat::ConnectionGene(2, 3, 3, 0.1, true));  // Bias    -> Hidden
    genome.connections.push_back(neat::ConnectionGene(3, 4, 4, 1.2, true));  // Hidden -> Output

    // Ensure genes are properly aligned/sorted
    neat::sort_genes(genome);
    std::cout << "[✓] Step 1: Genome created and sorted successfully.\n";

    // 2. Translate Genotype into Phenotype Network
    std::cout << "[ ] Step 2: Attempting network compilation...\n";

    // Explicit compilation step
    neat::Network net = neat::Network::construct_from_genome(genome);

    std::cout << "[✓] Step 2: Network built successfully.\n";

    // 3. Prepare concrete sensory test values
    std::unordered_map<int, double> inputs;
    inputs[0] = 1.0;
    inputs[1] = 1.0;

    net.set_inputs(inputs);
    std::cout << "[✓] Step 3: Inputs injected successfully into network.\n";

    // 4. Activate & Evaluate Layer Logic
    net.activate();
    std::cout << "[✓] Step 4: Network activation cycle completed.\n";

    // 5. Verify the Math manually
    std::vector<double> outputs = net.get_outputs();

    std::cout << "\n--- TEST METRICS RESULTS ---\n";
    std::cout << "Total Output Nodes Found: " << outputs.size() << "\n";
    if (!outputs.empty()) {
        std::cout << "Calculated Output Value:  " << outputs[0] << "\n";

        double expected = std::tanh(std::tanh(0.1) * 1.2);
        std::cout << "Expected Target Value:    " << expected << "\n";

        if (std::abs(outputs[0] - expected) < 0.0001) {
            std::cout << "\n>>> [SUCCESS] NEAT PHENOTYPE ENGINE WORKS PERFECTLY! <<<\n";
        } else {
            std::cout << "\n>>> [FAILURE] Math mismatch. Check activation layer linkages. <<<\n";
        }
    } else {
        std::cout << ">>> [FAILURE] Engine returned an empty output array. <<<\n";
    }
    std::cout << "==========================================\n";
}

void run_entity_generation_and_movement_test() {
    std::cout << "\n==========================================\n";
    std::cout << "   RUNNING ENTITY GENERATION & MOVEMENT   \n";
    std::cout << "           TEST SUITE                     \n";
    std::cout << "==========================================\n\n";

    // Step 1: Create a small hex grid for testing
    std::cout << "[*] Step 1: Initializing hex grid...\n";
    mapGen::HexGrid grid;
    grid.generateWorld(3);  // Create a grid with radius 3 (centered at origin)
    std::cout << "[✓] Step 1: Hex grid created with radius 3.\n\n";

    // Step 2: Create entities
    std::cout << "[*] Step 2: Creating test entities...\n";
    std::vector<std::unique_ptr<eco::Entity>> entities;
    const int NUM_ENTITIES = 5;

    for (int i = 0; i < NUM_ENTITIES; ++i) {
        entities.push_back(std::make_unique<eco::Entity>());
        std::cout << "    [✓] Entity " << i << " created successfully.\n";
    }
    std::cout << "[✓] Step 2: " << NUM_ENTITIES << " entities generated.\n\n";

    // Step 3: Place entities at origin
    std::cout << "[*] Step 3: Placing entities at origin (0,0,0)...\n";
    mapGen::Hex origin{0, 0, 0};
    mapGen::Cell *origin_cell = grid.getCell(origin);

    if (!origin_cell) {
        std::cout << "[✗] Step 3: FAILED - Could not get origin cell!\n";
        return;
    }

    for (size_t i = 0; i < entities.size(); ++i) {
        origin_cell->addEntity(entities[i].get());
        std::cout << "    [✓] Entity " << i << " placed at origin.\n";
    }
    std::cout << "[✓] Step 3: All entities placed at origin.\n";
    std::cout << "    Origin cell now contains " << origin_cell->getEntityCount()
              << " entities.\n\n";

    // Step 4: Test movement - move entities in different directions
    std::cout << "[*] Step 4: Testing entity movement in all directions...\n";
    const mapGen::HexDirection directions[] = {
        mapGen::HexDirection::RIGHT,
        mapGen::HexDirection::BOTTOM_RIGHT,
        mapGen::HexDirection::BOTTOM_LEFT,
        mapGen::HexDirection::LEFT,
        mapGen::HexDirection::TOP_LEFT,
        mapGen::HexDirection::TOP_RIGHT
    };

    const char* direction_names[] = {
        "RIGHT",
        "BOTTOM_RIGHT",
        "BOTTOM_LEFT",
        "LEFT",
        "TOP_LEFT",
        "TOP_RIGHT"
    };

    for (size_t i = 0; i < std::min(entities.size(), static_cast<size_t>(6)); ++i) {
        std::cout << "\n    Moving Entity " << i << " " << direction_names[i] << ":\n";
        entities[i]->move(grid, directions[i]);

        // Verify the move
        mapGen::Hex expected_pos = mapGen::Hex::get_neighbor(origin, directions[i]);
        mapGen::Cell *new_cell = grid.getCell(expected_pos);

        if (new_cell && new_cell->getEntityCount() > 0) {
            std::cout << "      [✓] Entity successfully moved to hex ("
                      << expected_pos.q << "," << expected_pos.r << "," << expected_pos.s << ")\n";
        } else {
            std::cout << "      [✗] Movement verification failed!\n";
        }
    }
    std::cout << "[✓] Step 4: Movement tests completed.\n\n";

    // Step 5: Test multiple movements of same entity
    std::cout << "[*] Step 5: Testing multiple sequential movements...\n";
    mapGen::Hex test_pos = origin;
    eco::Entity test_entity;
    mapGen::Cell *test_cell = grid.getCell(test_pos);
    test_cell->addEntity(&test_entity);

    std::cout << "    Starting position: (" << test_pos.q << ","
              << test_pos.r << "," << test_pos.s << ")\n";

    // Move right, then bottom-right, then left
    std::vector<mapGen::HexDirection> move_sequence = {
        mapGen::HexDirection::RIGHT,
        mapGen::HexDirection::BOTTOM_RIGHT,
        mapGen::HexDirection::LEFT
    };

    for (size_t i = 0; i < move_sequence.size(); ++i) {
        test_entity.move(grid, move_sequence[i]);
        test_pos = mapGen::Hex::get_neighbor(test_pos, move_sequence[i]);
        std::cout << "    [" << i + 1 << "] Moved " << direction_names[static_cast<int>(move_sequence[i])]
                  << " -> Position: (" << test_pos.q << "," << test_pos.r << ","
                  << test_pos.s << ")\n";
    }
    std::cout << "[✓] Step 5: Sequential movement test completed.\n\n";

    // Step 6: Verify entity distribution across grid
    std::cout << "[*] Step 6: Verifying entity distribution across grid...\n";
    int total_entities_on_grid = 0;
    int non_empty_cells = 0;

    for (auto &pair : grid.getMap()) {
        int entity_count = pair.second.getEntityCount();
        if (entity_count > 0) {
            total_entities_on_grid += entity_count;
            non_empty_cells++;
            std::cout << "    Cell (" << pair.first.q << "," << pair.first.r << ","
                      << pair.first.s << ") contains " << entity_count << " entity(ies).\n";
        }
    }
    std::cout << "[✓] Step 6: Distribution Summary\n";
    std::cout << "    Total entities on grid: " << total_entities_on_grid << "\n";
    std::cout << "    Non-empty cells: " << non_empty_cells << "\n";
    std::cout << "    Expected total: " << NUM_ENTITIES + 1 << " (5 original + 1 test entity)\n";

    if (total_entities_on_grid == NUM_ENTITIES + 1) {
        std::cout << "    [✓] Entity count verification PASSED\n";
    } else {
        std::cout << "    [✗] Entity count mismatch - expected " << (NUM_ENTITIES + 1)
                  << " but found " << total_entities_on_grid << "\n";
    }

    std::cout << "\n==========================================\n";
    std::cout << "   ENTITY TEST SUITE COMPLETED\n";
    std::cout << "==========================================\n\n";
}
