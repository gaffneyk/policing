#include <iostream>
#include <enki/PhysicalEngine.h>
#include <enki/Random.h>
#include <viewer/Viewer.h>
#include <QApplication>
#include <random>
#include "Bot.h"
#include "Token.h"

using namespace Enki;
using namespace std;

enum Relatedness {
    r100,
    r75,
    r50,
    r25,
    r0
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

double runGroup(vector<Bot*> bots, int numTokens, int seconds, double benefit, double cost) {

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
    int numTokensShared = 0;
    for (int i = 0; i < numTokens; i++) {
        int status = tokens[i]->getStatus();
        if (status == shared) {
            numTokensShared++;
            for (auto bot : bots) {
                if (bot != tokens[i]->getInitialBotCollided()) {
                    bot->increaseFitnessScore(benefit / (bots.size() - 1));
                }
            }
        } else if (status == kept) {
            tokens[i]->getInitialBotCollided()->increaseFitnessScore(cost);
        }
    }

    return numTokensShared / (double) numTokens;
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
                group.push_back(new Bot(*population.at(2)));
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

vector<Bot*> runGeneration(vector<Bot*> prevPopulation, int numGroups, Relatedness r, double benefit, double cost) {
    vector<Bot*> population;
    population.reserve((unsigned long) numGroups * 8);
    double averageAltruism = 0.0;

    for (int i = 0; i < numGroups; i++) {
        cout << "Running group " << i << endl;

        vector<Bot*> group = createGroup(r, prevPopulation);
        prevPopulation.pop_back();
        population.insert(population.end(), group.begin(), group.end());
        double altruism = runGroup(group, 8, 60, benefit, cost);
        averageAltruism += altruism / numGroups;
    }

    cout << "Average altruism: " << averageAltruism << endl;
    int numBotsNextPopulation = 0;

    switch (r) {
        case r100:
            numBotsNextPopulation = numGroups;
            break;
        case r75:
        case r50:
        case r25:
            numBotsNextPopulation = 3 * numGroups;
            break;
        case r0:
            numBotsNextPopulation = 8 * numGroups;
    }
    vector<Bot*> nextPopulation = rouletteWheelSelection(population, numBotsNextPopulation);
    return nextPopulation;
}

int main(int argc, char *argv[]) {

    const int numGroups = 10;
    const int numGenerations = 10;

    vector<Bot*> population;
    population.reserve(numGroups);

    for (int i = 0; i < numGroups; i++) {
        population.emplace_back(new Bot());
    }

    for (int generation = 0; generation < numGenerations; generation++) {
        cout << "Running generation " << generation << endl;
        population = runGeneration(population, numGroups, r100, 0.9, 0.1);
    }

//    QApplication app(argc, argv);
//    ViewerWidget viewer(world);
//    viewer.show();
//    app.exec();
}