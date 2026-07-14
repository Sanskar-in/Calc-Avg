#include "../include/neural_net.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifndef _WIN32
#include <pthread.h>
#endif
#include <math.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <unistd.h>
#endif

int global_nn_epoch = 0;
double global_nn_loss = 0.0;
int is_training_nn = 0;
char global_nn_final_result[4096] = "";

typedef struct {
    float* data;
    int count;
    int epochs;
    int hidden_nodes;
} NNTrainingData;

double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }
double d_sigmoid(double x) { return x * (1.0 - x); }

// Background Thread Function
#ifdef _WIN32
void train_neural_network_thread(void* arg) {
#else
void* train_neural_network_thread(void* arg) {
#endif
    NNTrainingData* td = (NNTrainingData*)arg;
    int n_samples = td->count - 1;
    if (n_samples <= 0) {
        is_training_nn = 0;
        free(td->data);
        free(td);
        return;
    }

    // Normalization
    float min_val = td->data[0], max_val = td->data[0];
    for (int i = 1; i < td->count; i++) {
        if (td->data[i] < min_val) min_val = td->data[i];
        if (td->data[i] > max_val) max_val = td->data[i];
    }
    float range = max_val - min_val;
    if (range == 0) range = 1.0;

    int hidden_nodes = td->hidden_nodes;
    if (hidden_nodes <= 0 || hidden_nodes > 1000) hidden_nodes = 10;
    
    double *w1 = (double*)malloc(hidden_nodes * sizeof(double));
    double *b1 = (double*)malloc(hidden_nodes * sizeof(double));
    double *w2 = (double*)malloc(hidden_nodes * sizeof(double));
    double *hidden = (double*)malloc(hidden_nodes * sizeof(double));
    double *d_hidden = (double*)malloc(hidden_nodes * sizeof(double));
    double b2 = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
    
    // Initialize weights randomly [-1.0, 1.0]
    for (int i = 0; i < hidden_nodes; i++) {
        w1[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        b1[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        w2[i] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
    }

    double learning_rate = 0.1;
    int total_epochs = td->epochs;
    if (total_epochs <= 0) total_epochs = 10000;

    for (int epoch = 1; epoch <= total_epochs; epoch++) {
        double total_loss = 0.0;

        for (int i = 0; i < n_samples; i++) {
            // Forward propagation
            double x = (td->data[i] - min_val) / range;
            double target = (td->data[i+1] - min_val) / range;
            

            for (int h = 0; h < hidden_nodes; h++) {
                hidden[h] = sigmoid(x * w1[h] + b1[h]);
            }
            
            double output = b2;
            for (int h = 0; h < hidden_nodes; h++) {
                output += hidden[h] * w2[h];
            }
            output = sigmoid(output);
            
            double error = target - output;
            total_loss += 0.5 * error * error;

            // Backpropagation
            double d_output = error * d_sigmoid(output);

            
            for (int h = 0; h < hidden_nodes; h++) {
                d_hidden[h] = d_output * w2[h] * d_sigmoid(hidden[h]);
            }
            
            // Update weights
            for (int h = 0; h < hidden_nodes; h++) {
                w2[h] += learning_rate * d_output * hidden[h];
                w1[h] += learning_rate * d_hidden[h] * x;
                b1[h] += learning_rate * d_hidden[h];
            }
            b2 += learning_rate * d_output;
        }

        global_nn_epoch = epoch;
        global_nn_loss = total_loss / n_samples;
        
        // Prevent 100% CPU lockup and allow WebSocket time to broadcast
        #ifdef _WIN32
        if (epoch % 100 == 0) Sleep(5);
        #else
        if (epoch % 100 == 0) usleep(5000);
        #endif
    }
    
    // Predict next value (using the very last item in the sequence)
    double x = (td->data[td->count - 1] - min_val) / range;

    for (int h = 0; h < hidden_nodes; h++) {
        hidden[h] = sigmoid(x * w1[h] + b1[h]);
    }
    double output = b2;
    for (int h = 0; h < hidden_nodes; h++) {
        output += hidden[h] * w2[h];
    }
    output = sigmoid(output);
    double predicted = (output * range) + min_val;
    
    snprintf(global_nn_final_result, sizeof(global_nn_final_result), "{\"prediction\": %.4f, \"final_loss\": %.6f, \"epochs\": %d}", predicted, global_nn_loss, total_epochs);

    is_training_nn = 0;
    free(w1); free(b1); free(w2); free(hidden); free(d_hidden);
    free(td->data);
    free(td);
#ifndef _WIN32
    return NULL;
#endif
}

void start_neural_network_training(float* data, int count, int epochs, int hidden_nodes) {
    NNTrainingData* td = (NNTrainingData*)malloc(sizeof(NNTrainingData));
    td->data = (float*)malloc(count * sizeof(float));
    for (int i = 0; i < count; i++) td->data[i] = data[i];
    td->count = count;
    td->epochs = epochs;
    td->hidden_nodes = hidden_nodes;
    
    global_nn_epoch = 0;
    global_nn_loss = 0.0;
    strcpy(global_nn_final_result, "");
    is_training_nn = 1;
    
    #ifdef _WIN32
    _beginthread(train_neural_network_thread, 0, (void*)td);
    #else
    pthread_t ai_thread;
    pthread_create(&ai_thread, NULL, train_neural_network_thread, (void*)td);
    pthread_detach(ai_thread);
    #endif
}
