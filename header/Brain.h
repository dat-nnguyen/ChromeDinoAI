#ifndef BRAIN_H
#define BRAIN_H

#include <vector>

class Brain {
public:
    Brain(int inputSize, int hiddenSize, int outputSize);

    // AI Logic
    std::vector<float> feedForward(std::vector<float> inputs);

    void mutate(float rate);
    Brain clone();

private:
    int inputNodes;
    int hiddenNodes;
    int outputNodes;

    std::vector<std::vector<float>> weights_IH;
    std::vector<std::vector<float>> weights_HO;
    std::vector<float> bias_H;
    std::vector<float> bias_O;

    float sigmoid(float x);
};

#endif