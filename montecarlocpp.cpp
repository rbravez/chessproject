#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <algorithm>

double elo_win_probability(double elo1, double elo2) {
    return 1.0 / (1.0 + std::pow(10.0, (elo2 - elo1) / 400.0));
}

bool simulate_game(double white_elo, double black_elo, bool is_hikaru_white) {
    double win_prob_white = elo_win_probability(white_elo, black_elo);
    double random_value = static_cast<double>(rand()) / RAND_MAX;
    return is_hikaru_white ? random_value < win_prob_white : random_value > win_prob_white;
}

std::vector<int> monte_carlo_simulation(const std::vector<std::vector<std::string>>& data, int n_simulations) {
    std::vector<int> max_streaks;
    
    for (int sim = 0; sim < n_simulations; ++sim) {
        int current_streak = 0;
        int max_streak = 0;
        
        for (const auto& row : data) {
            double white_elo = 0.0;
            double black_elo = 0.0;
            bool is_hikaru_white = (row[2] == "Hikaru");

            try {
                white_elo = std::stod(row[0]);
                black_elo = std::stod(row[1]);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid data format: " << row[0] << ", " << row[1] << std::endl;
                continue;
            }

            bool result = simulate_game(white_elo, black_elo, is_hikaru_white);
            
            if (result) {
                ++current_streak;
                if (current_streak > max_streak) {
                    max_streak = current_streak;
                }
            } else {
                current_streak = 0;
            }
        }
        max_streaks.push_back(max_streak);
    }
    return max_streaks;
}

int main() {
    std::ifstream file("filtered_hikaru_games.csv");
    std::string line;
    std::vector<std::vector<std::string>> data;

    // Read CSV file
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    // Skip the header
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        data.push_back(row);
    }
    file.close();

    // Set random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int n_simulations = 100;
    auto max_streaks = monte_carlo_simulation(data, n_simulations);

    // Analyze results
    int streaks_over_30 = std::count_if(max_streaks.begin(), max_streaks.end(), [](int streak) {
        return streak > 50;
    });

    double probability = static_cast<double>(streaks_over_30) / n_simulations;
    std::cout << "Probability of Hikaru having a winning streak of more than 30 games: " << probability << std::endl;

    return 0;
}
