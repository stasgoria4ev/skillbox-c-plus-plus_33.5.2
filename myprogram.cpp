#include <iostream>
#include <exception>
#include <ctime>
#include <vector>
#include <cstdlib>

class Fish: public std::exception {
public:
    const char* what() const noexcept override {
        return "CAUGHT A FISH!!";
    }
};

class Boot: public std::exception {
public:
    const char* what() const noexcept override {
        return "CAUGHT Boot...\n";
    }
};

struct Field {
    Fish* fish = nullptr;
    Boot* boot = nullptr;
    std::vector<int> busySectorNumbers;
};

void NewFish(Field (&field)[9]) {
    int fishSector = std::rand() % 9;
    field->busySectorNumbers.push_back(fishSector);
    field[fishSector].fish = new Fish();
}

bool Verification(int& bootsSector, std::vector<int>& busySectorNumbers) {
    bool found = false;
    for (int i = 0; i < busySectorNumbers.size() && !found; ++i) {
        if (bootsSector == busySectorNumbers[i]) {
            found = true;
        }
    }
    return found;
}

void NewBoot(Field (&field)[9]) {
    for (int i = 0; i <= 2; ++i) {
        int bootsSector;
        do {
            bootsSector = std::rand() % 9;
        } while (Verification(bootsSector, field->busySectorNumbers));
        field->busySectorNumbers.push_back(bootsSector);
        field[bootsSector].boot = new Boot();
    }
}

void Fishing (Field (&field)[9], int sector) {
    for (int i = 0; i < sizeof(field) / sizeof(field[0]); ++i)
        if (field[i].fish != nullptr && i + 1 == sector)
            throw Fish();
        else if (field[i].boot != nullptr && i + 1 == sector)
            throw Boot();
}

int main() {
    std::srand(std::time(nullptr));
    Field field[9];

    NewFish(field);

    NewBoot(field);

    bool good = true;
    int count = 1;
    do {
        std::cout << "Choose a sector for fishing(1:9): ";
        int sector;
        std::cin >> sector;

        try {
            Fishing(field, sector);
        }
        catch (const Fish& x) {
            std::cerr << "Cought exception: " << x.what();
            std::cout << " on the " << count << " try.\n";
            good = false;
        }
        catch (const Boot& x) {
            std::cerr << "Cought exception: " << x.what();
            good = false;
        }
        count++;
    } while (good);
} 