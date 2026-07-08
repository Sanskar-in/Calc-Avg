#ifndef NEURAL_NET_H
#define NEURAL_NET_H

extern int global_nn_epoch;
extern double global_nn_loss;
extern int is_training_nn;
extern char global_nn_final_result[4096];

// The function called by API to spawn the background thread
void start_neural_network_training(float* data, int count);

#endif
