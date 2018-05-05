#include <iostream>
#include <enki/PhysicalEngine.h>
#include <enki/Random.h>
#include <viewer/Viewer.h>
#include <QApplication>
#include <random>
#include <fstream>
#include <sstream>
#include <chrono>
#include "Bot.h"
#include "Token.h"

using namespace Enki;
using namespace std;

enum Relatedness {
    r0,
    r25,
    r50,
    r75,
    r100
};

Bot* crossover(Bot *parent1, Bot *parent2) {
    double probability = 0.005;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0, 1);

    auto *child = new Bot(*parent1);
    vector<bitset<8>> childGenome = child->getController()->getBinaryGenome();
    vector<bitset<8>> parent1Genome = parent1->getController()->getBinaryGenome();
    vector<bitset<8>> parent2Genome = parent2->getController()->getBinaryGenome();
    bool parentSwitch = true;

    for (unsigned long i = 0; i < childGenome.size(); i++) {

        for (int j = 0; j < childGenome.at(i).size(); j++) {

            if (dist(mt) < probability) {
                // Crossover, switch parents
                parentSwitch = !parentSwitch;
            }

            if (parentSwitch) {
                childGenome.at(i)[j] = parent1Genome.at(i)[j];
            } else {
                childGenome.at(i)[j] = parent2Genome.at(i)[j];
            }
        }
    }

    child->getController()->setBinaryGenome(childGenome);

    return child;
}

void mutate(Bot* bot) {
    double probability = 0.005;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0, 1);

    vector<bitset<8>> genome = bot->getController()->getBinaryGenome();

    for (unsigned long i = 0; i < genome.size(); i++) {

        for (int j = 0; j < genome.at(i).size(); j++) {

            if (dist(mt) < probability) {
                // Mutation, flip bit
                genome.at(i)[j].flip();
            }
        }
    }

    bot->getController()->setBinaryGenome(genome);
}

vector<Bot*> rouletteWheelSelection(vector<Bot*> population, int newPopulationSize) {
    vector<Bot*> newPopulation;
    newPopulation.reserve((unsigned long) newPopulationSize);

    double sumFitness = 0;
    for (Bot *bot : population) {
        sumFitness += bot->getFitnessScore();
    }

    vector<double> probabilities;
    probabilities.reserve(population.size());
    double previousProbability = 0.0;
    for (Bot *bot : population) {
        double probability = previousProbability + bot->getFitnessScore() / sumFitness;
        probabilities.push_back(probability);
        previousProbability = probability;
    }

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0, 1);

    while (newPopulation.size() < newPopulationSize) {
        Bot *parents[2];
        for (int parent = 0; parent < 2; parent++) {
            double number = dist(mt);
            unsigned long i = 0;
            while (i < probabilities.size() && probabilities.at(i) < number) {
                i++;
            }
            parents[parent] = population.at(i);
        }
        Bot *child = crossover(parents[0], parents[1]);
        mutate(child);
        newPopulation.push_back(child);
    }

    return newPopulation;
}

tuple<double, double> runGroup(vector<Bot*> bots, int numTokens, int seconds, double benefit, double cost) {

    // Generate the world
    auto world = new World(100, 100, Color(0.2, 0.2, 0.2));

    // Create uniform distributions for position and angle
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> posDist(0, 100);
    uniform_real_distribution<double> angleDist(-M_PI, M_PI);

    // Add bots to the world
    for (auto bot : bots) {
        bot->pos = Point(posDist(mt), posDist(mt));
        bot->angle = angleDist(mt);
        world->addObject(bot);
    }

    // Add tokens to the world
    Token *tokens[numTokens];
    for (int i = 0; i < numTokens; i++) {
        auto *token = new Token();
        token->pos = Point(posDist(mt), posDist(mt));
        world->addObject(token);
        tokens[i] = token;
    }

    // Run the world for the specified number of seconds
    const double timeStep = 0.1;
    for (auto i = 0; i < (int) round(seconds / timeStep); i++) {
        world->step(timeStep);
    }

    // Determine fitness score for each bot
    int numTokensVisited = 0;
    int numTokensShared = 0;
    for (int i = 0; i < numTokens; i++) {

        int status = tokens[i]->getStatus();
        if (status == shared) {
            numTokensVisited++;
            numTokensShared++;
            for (auto bot : bots) {
                if (bot != tokens[i]->getInitialBotCollided()) {
                    bot->increaseFitnessScore(benefit / (bots.size() - 1));
                }
            }
        } else if (status == kept) {
            numTokensVisited++;
            tokens[i]->getInitialBotCollided()->increaseFitnessScore(cost);
        }
    }

    double efficiency = numTokensVisited / (double) numTokens;
    double altruism = numTokensShared / (double) numTokens;

    return make_tuple(efficiency, altruism);
}

vector<Bot*> createGroup(Relatedness r, vector<Bot*> population) {
    vector<Bot*> group;
    group.reserve(8);

    switch (r) {
        case r100:
            for (int i = 0; i < 8; i++) {
                group.push_back(new Bot(*population.back()));
            }
            break;
        case r75:
            group.push_back(new Bot(*population.back()));
            population.pop_back();
            for (int i = 0; i < 7; i++) {
                group.push_back(new Bot(*population.back()));
            }
            population.pop_back();
            break;
        case r50:
            group.push_back(new Bot(*population.back()));
            population.pop_back();
            group.push_back(new Bot(*population.back()));
            population.pop_back();
            for (int i = 0; i < 6; i++) {
                group.push_back(new Bot(*population.back()));
            }
            population.pop_back();
            break;
        case r25:
            for (int i = 0; i < 2; i++) {
                group.push_back(new Bot(*population.back()));
            }
            population.pop_back();
            for (int i = 0; i < 3; i++) {
                group.push_back(new Bot(*population.back()));
            }
            population.pop_back();
            for (int i = 0; i < 3; i++) {
                group.push_back(new Bot(*population.back()));
            }
            population.pop_back();
            break;
        case r0:
            for (unsigned long i = 0; i < 8; i++) {
                group.push_back(new Bot(*population.back()));
                population.pop_back();
            }
            break;
    }

    return group;
}

tuple<double, double> runGeneration(vector<Bot*> prevPopulation, int numGroups, Relatedness r, double benefit, double cost) {
    vector<Bot*> population;
    population.reserve((unsigned long) numGroups * 8);
    double averageEfficiency = 0.0;
    double averageAltruism = 0.0;

    for (int i = 0; i < numGroups; i++) {
        cout << "Group " << i << endl;
        vector<Bot*> group = createGroup(r, prevPopulation);
        population.insert(population.end(), group.begin(), group.end());
        double efficiency, altruism;
        tie(efficiency, altruism) = runGroup(group, 8, 60, benefit, cost);
        averageEfficiency += efficiency / numGroups;
        averageAltruism += altruism / numGroups;
    }

    return make_tuple(averageEfficiency, averageAltruism);
}

int getNumBotsInPopulation(int numGroups, Relatedness r) {
    int numBotsInPopulation;
    switch (r) {
        case r100:
            numBotsInPopulation = numGroups;
            break;
        case r75:
        case r50:
        case r25:
            numBotsInPopulation = 3 * numGroups;
            break;
        case r0:
            numBotsInPopulation = 8 * numGroups;
    }
    return numBotsInPopulation;
};

template<typename T>
void writeToFile(ofstream &file, vector<T> data, bool endWithNewline) {
    for (size_t i = 0; i < data.size(); i++) {
        file << data[i];
        if (i == data.size() - 1 && endWithNewline) {
            file << "\n";
        } else {
            file << ",";
        }
    }
}

int main(int argc, char *argv[]) {

    // Set experimental parameters
    const int numGroups = 200;
    const unsigned long numGenerations = 1;
    const int numReplicates = 1;
    const Relatedness rs[5] = {r0, r25, r50, r75, r100};
    double cbRatios[5] = {0.01, 0.25, 0.54, 0.75, 1.0};

    // Open the file to which data will be written
    ofstream file("data.csv");

    file << "treatment,";
    vector<string> header(2 * numGenerations);
    for (int generation = 0; generation < numGenerations; generation++) {
        header[generation] = "efficiency" + to_string(generation);
        header[generation + numGenerations] = "altruism" + to_string(generation);
    }
    writeToFile(file, header, true);

    // Run the experiment, recording average efficiency and altruism for each generation
    auto start = chrono::high_resolution_clock::now();
    for (Relatedness r : rs) {
        for (double cbRatio : cbRatios) {

            cout << "Treatment " << "r" << r << "cb" << cbRatio << endl;

            // Create initial population
            vector<Bot *> population;
            int numBotsInPopulation = getNumBotsInPopulation(numGroups, r);
            population.reserve((unsigned long) numBotsInPopulation);
            for (int i = 0; i < numBotsInPopulation; i++) {
                population.emplace_back(new Bot());
            }

            double benefit = 1 / (1 + cbRatio);
            double cost = 1 - benefit;
            vector<double> efficiencies;
            vector<double> altruisms;
            efficiencies.assign(numGenerations, 0.0);
            altruisms.assign(numGenerations, 0.0);
            for (int i = 0; i < numReplicates; i++) {
                cout << "Replicate " << i << endl;
                for (unsigned long generation = 0; generation < numGenerations; generation++) {
                    cout << "Generation " << generation << endl;
                    double averageEfficiency, averageAltruism;
                    tie(averageEfficiency, averageAltruism) = runGeneration(population, numGroups, r, benefit, cost);
                    efficiencies[generation] += averageEfficiency / numReplicates;
                    altruisms[generation] += averageAltruism / numReplicates;
                    population = rouletteWheelSelection(population, getNumBotsInPopulation(numGroups, r));
                }
            }
            file << "r" << r << "cb" << cbRatio << ",";
            writeToFile(file, efficiencies, false);
            writeToFile(file, altruisms, true);
        }
    }
    auto stop = chrono::high_resolution_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<chrono::seconds>(stop - start).count() << endl;
    file.close();

    return 0;

//    QApplication app(argc, argv);
//    ViewerWidget viewer(world);
//    viewer.show();
//    app.exec();
}