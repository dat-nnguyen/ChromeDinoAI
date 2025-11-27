//
// Created by Nguyen Tuan Dat on 26/11/25.
//
#include "Brain.h"
#include <cmath>   // Needed for exp()
#include <cstdlib> // Needed for rand()

Brain::Brain(int in, int hid, int out): inputNodes(in), hiddenNodes(hid), outputNodes(out) {
    // 1. Input -> Hidden Weights
    weights_IH.resize(inputNodes, std::vector<float>(hiddenNodes));

    for(int i=0; i < inputNodes; i++) {
        for(int j=0; j < hiddenNodes; j++) {
            weights_IH[i][j] = ((float)rand() / RAND_MAX) * 2 - 1;
        }
    }

    // 2. Hidden -> Output Weights
    weights_HO.resize(hiddenNodes, std::vector<float>(outputNodes));
    for(int i=0; i < hiddenNodes; i++) {
        for(int j=0; j < outputNodes; j++) {
            weights_HO[i][j] = ((float)rand() / RAND_MAX) * 2 - 1;
        }
    }

    // 3. Biases
    bias_H.resize(hiddenNodes);
    bias_O.resize(outputNodes);
    for(auto &b : bias_H) b = ((float)rand() / RAND_MAX) * 2 - 1;
    for(auto &b : bias_O) b = ((float)rand() / RAND_MAX) * 2 - 1;
}

float Brain::sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

std::vector<float> Brain::feedForward(std::vector<float> inputs) {
    // 1. Calculate Hidden Layer Outputs
    std::vector<float> hidden(hiddenNodes);
    for(int j=0; j < hiddenNodes; j++) {
        float sum = 0;
        for(int i=0; i < inputNodes; i++) {
            sum += inputs[i] * weights_IH[i][j];
        }
        sum += bias_H[j];
        hidden[j] = sigmoid(sum); // Apply activation
    }

    // 2. Calculate Output Layer Outputs
    std::vector<float> outputs(outputNodes);
    for(int j=0; j < outputNodes; j++) {
        float sum = 0;
        for(int i=0; i < hiddenNodes; i++) {
            sum += hidden[i] * weights_HO[i][j];
        }
        sum += bias_O[j];
        outputs[j] = sigmoid(sum);
    }

    return outputs;
}

void Brain::mutate(float rate) {
    auto mutateValue = [&](float &val) {
        if (((float)rand() / RAND_MAX) < rate) {
            // Add a small random nudge
            val += (((float)rand() / RAND_MAX) * 0.4f) - 0.2f;

            // Clamp values between -1 and 1
            if(val > 1) val = 1;
            if(val < -1) val = -1;
        }
    };

    for(auto &row : weights_IH) for(auto &w : row) mutateValue(w);
    for(auto &row : weights_HO) for(auto &w : row) mutateValue(w);
    for(auto &b : bias_H) mutateValue(b);
    for(auto &b : bias_O) mutateValue(b);
}

Brain Brain::clone() {
    Brain copy(inputNodes, hiddenNodes, outputNodes);
    copy.weights_IH = this->weights_IH;
    copy.weights_HO = this->weights_HO;
    copy.bias_H = this->bias_H;
    copy.bias_O = this->bias_O;
    return copy;
}