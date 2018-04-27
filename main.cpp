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


void runGroup(vector<Bot*> bots, int numTokens, int seconds, double benefit, double cost) {

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
    for (int i = 0; i < numTokens; i++) {
        int status = tokens[i]->getStatus();
        if (status == shared) {
            for (auto bot : bots) {
                if (bot != tokens[i]->getInitialBotCollided()) {
                    bot->increaseFitnessScore(cost / (bots.size() - 1));
                }
            }
        } else if (status == kept) {
            tokens[i]->getInitialBotCollided()->increaseFitnessScore(benefit);
        }
    }
}

vector<Bot*> createGroup100(Bot *bot) {
    vector<Bot*> group;
    group.reserve(8);
    for (int i = 0; i < 8; i++) {
        group.push_back(new Bot(*bot));
    }
    return group;
}

vector<Bot*> runFirstGeneration(int numGroups) {
    vector<Bot*> population;
    population.reserve((unsigned long) numGroups * 8);
    for (int i = 0; i < numGroups; i++) {
        cout << "Running group " << i << endl;
        vector<Bot*> group = createGroup100(new Bot());
        population.insert(population.end(), group.begin(), group.end());
        runGroup(group, 8, 60, 0.1, 0.9);
    }
    return population;
}

void runGeneration() {

}

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

vector<Bot*> rouletteWheelSelection(vector<Bot*> population, unsigned long newPopulationSize) {
    vector<Bot*> newPopulation;
    newPopulation.reserve(newPopulationSize);

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

int main(int argc, char *argv[]) {

    vector<Bot*> population = runFirstGeneration(10);
    vector<Bot*> newPopulation = rouletteWheelSelection(population, 10);
    for (Bot *bot : newPopulation) {
        cout << bot->getFitnessScore() << endl;
    }

//    QApplication app(argc, argv);
//    ViewerWidget viewer(world);
//    viewer.show();
//    app.exec();

}