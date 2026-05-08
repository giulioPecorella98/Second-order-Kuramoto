// Function for saving the result of the density simulation in a binary file
#include "saving_density.h"

std::string save() {

    std::filesystem::path savedDir = std::filesystem::path(PROJECT_ROOT) / "save" / "density";
    if (!std::filesystem::is_directory(savedDir)) { std::filesystem::create_directories(savedDir); }
    std::cout << "Enter the name of the file where you want to save the result (type 's' to see saved files, 'q' to quit to main menu): ";
    std::string filename;
    std::cin >> filename;
    while (filename == "s") {
        std::cout << "Saved files in 'save/density' directory:" << std::endl;
        for (const auto& entry : std::filesystem::directory_iterator(savedDir)) {
            if (entry.is_regular_file()) {
                std::cout << entry.path().filename().string() << " , ";
            }
        }
        std::cout << "\nEnter the name of the file where you want to save the result (type 's' to see saved files, 'q' to quit to main menu): ";
        std::cin >> filename;
    }
    if (filename == "q") { return "quit"; }
    // Check if the file already exists and ask for confirmation to overwrite
    std::filesystem::path saving = std::filesystem::path(PROJECT_ROOT) / "save" / "density" / filename;
    if (std::filesystem::exists(saving)) {
        std::cout << "Warning: file name '" << filename << "' already exists. Do you want to overwrite it? (y/n): ";
        char choice;
        std::cin >> choice;
        if ((choice != 'y') && (choice != 'Y')) {
            return save(); // Ask for the filename again
        }
    }

    return filename;
}