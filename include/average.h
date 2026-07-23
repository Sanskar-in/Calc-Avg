// =============================================================================
// (c) Copyright Sanskar. All rights reserved.
// Made by the Sanskar.
// See this open source project created via Sanskar on GitHub to calculate the average of any numbers via using this link:- "https://www.github.com/sanskarIN/Calc-Avg"
// 
// Follow Sanskar or Creator on:-
// GitHub:- "https://www.github.com/sanskarIN"
// YouTube:- "https://youtube.com/@sanskarIN"
// LinkedIn:- "https://www.linkedin.com/in/sanskar-in/"
// X (Formally known as Twitter):- "https://www.x.com/Sanskar_in"
// Need support:- supportramsandesh@gmail.com
// Want to make your own projects, apps, and games via Sanskar:- sanskaryadavfrom2012to2026@gmail.com
// =============================================================================
// File Purpose: Declarations for average calculation functions.

#ifndef AVERAGE_H
#define AVERAGE_H

#include <stdbool.h>

/**
 * Calculates the average of a given array of numbers.
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 * @return The calculated average.
 */
double calc_avg_array(double numbers[], int count);

/**
 * Calculates the average of the first N natural numbers.
 * @param n The number of natural numbers.
 * @return The calculated average.
 */
double calc_avg_natural(int n);

/**
 * Calculates the average of the first N odd numbers.
 * @param n The number of odd numbers.
 * @return The calculated average.
 */
double calc_avg_odd(int n);

/**
 * Calculates the average of the first N even numbers.
 * @param n The number of even numbers.
 * @return The calculated average.
 */
double calc_avg_even(int n);

/**
 * Calculates the average of the first N prime numbers.
 * @param n The number of prime numbers.
 * @return The calculated average.
 */
double calc_avg_prime(int n);

/**
 * Calculates the average of the first N numbers in the Fibonacci sequence.
 * @param n The number of Fibonacci numbers.
 * @return The calculated average.
 */
double calc_avg_fibonacci(int n);

/**
 * Calculates the average of numbers loaded directly from a text file.
 * The file should contain numbers separated by whitespace.
 * @param filename The path to the text file.
 * @return The calculated average.
 */
double calc_avg_from_file(const char *filename);

/**
 * Calculates the median of a given array of numbers.
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 * @return The calculated median.
 */
double calc_median(double numbers[], int count);

/**
 * Calculates the mode of a given array of numbers.
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 * @return The calculated mode.
 */
double calc_mode(double numbers[], int count);

/**
 * Calculates the geometric mean of a given array of numbers.
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 * @return The calculated geometric mean.
 */
double calc_geometric_mean(double numbers[], int count);

/**
 * Calculates the harmonic mean of a given array of numbers.
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 * @return The calculated harmonic mean.
 */
double calc_harmonic_mean(double numbers[], int count);

/**
 * Calculates the average of numbers from multiple text files.
 * @param filenames Array of filenames (up to 256 chars each).
 * @param file_count Number of files to process.
 * @return The overall calculated average of all numbers across all files.
 */
double calc_avg_multi_file(char filenames[][256], int file_count);

/**
 * Calculates the average of numbers from a CSV (Comma-Separated Values) file.
 * @param filename Name of the CSV file.
 * @return The calculated average, or 0.0 on error.
 */
double calc_avg_from_csv(const char *filename);

/**
 * Calculates the average of numbers from a JSON file.
 * Intelligently extracts raw numbers while ignoring structural characters.
 * @param filename Name of the JSON file.
 * @return The calculated average, or 0.0 on error.
 */
double calc_avg_from_json(const char *filename);

/**
 * Connects to a live API over HTTP, downloads JSON data, and calculates the average.
 * @param hostname The API hostname (e.g., api.example.com)
 * @param path The API path (e.g., /data)
 * @return The calculated average, or 0.0 on error.
 */
double calc_avg_from_api(const char *hostname, const char *path);

/**
 * Connects to a local SQLite Database, executes an SQL Query, and calculates the average of a specific numerical column.
 * @param db_filename The path to the SQLite database (e.g., database.db)
 * @param table_name The name of the table to query.
 * @param column_name The name of the numerical column to average.
 * @return The calculated average, or 0.0 on error.
 */
double calc_avg_from_sqlite(const char *db_filename, const char *table_name, const char *column_name);

/**
 * Calculates the average of numbers from a batch of files using threads.
 * @param filenames Array of filenames.
 * @param file_count Number of files.
 * @return The calculated average.
 */
double calc_avg_from_batch_threaded(char *filenames[], int file_count);

/**
 * Calculates the total, row, and column averages of a 2D matrix.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @param matrix The 2D array matrix.
 * @return The total average of the entire matrix.
 */
double calc_avg_matrix(int rows, int cols, double **matrix);

/**
 * Calculates and prints the Simple Moving Average (SMA) and Exponential Moving Average (EMA).
 * @param prices Array of sequential prices.
 * @param count Number of prices in the array.
 * @param window_size The sliding window period (e.g. 5 days).
 */
void calc_financial_moving_averages(double prices[], int count, int window_size);

/**
 * Calculates the Numerical Integral and Derivative (Engineering Calculus).
 * @param data_points Array of data points.
 * @param count Number of data points.
 * @param step_size The step interval between data points (e.g. 1.0 seconds).
 */
void calc_engineering_calculus(double data_points[], int count, double step_size);

/**
 * Calculates the Line of Best Fit (Linear Regression) and predicts future data points.
 * @param data_points Array of historical data points.
 * @param count Number of historical data points.
 * @param future_predictions Number of future periods to predict.
 */
void calc_linear_regression(double data_points[], int count, int future_predictions);

/**
 * Compresses an array of numbers using Run-Length Encoding (RLE) and calculates the average from the compressed stream.
 * @param data_points Array of data points.
 * @param count Number of data points.
 */
void calc_rle_compression(double data_points[], int count);

/**
 * Calculates the weighted average.
 * @param values Array of values.
 * @param weights Array of weights corresponding to the values.
 * @param count Number of elements in the arrays.
 * @return The calculated weighted average.
 */
double calc_weighted_average(double values[], double weights[], int count);

/**
 * Calculates the average of numbers that fall within a specific range [min_val, max_val].
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 * @param min_val The minimum allowed value (inclusive).
 * @param max_val The maximum allowed value (inclusive).
 * @return The calculated average.
 */
double calc_filtered_average(double numbers[], int count, double min_val, double max_val);

/**
 * Calculates the variance of a given array of numbers.
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 * @param is_population Set to true for population variance, false for sample variance.
 * @return The calculated variance.
 */
double calc_variance(double numbers[], int count, bool is_population);

/**
 * Calculates the standard deviation of a given array of numbers.
 * @param numbers Array of numbers.
 * @param count Number of elements in the array.
 * @param is_population Set to true for population std dev, false for sample std dev.
 * @return The calculated standard deviation.
 */
double calc_standard_deviation(double numbers[], int count, bool is_population);

#endif // AVERAGE_H
