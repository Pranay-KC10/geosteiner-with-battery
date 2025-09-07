#ifndef BATTERY_H
#define BATTERY_H

struct battery_info {
    int num_terminals;
    double* battery_levels;  
    double battery_weight;  
};

struct battery_info* load_battery_info(const char* filename, int num_terminals);
void free_battery_info(struct battery_info* binfo);
double compute_battery_penalty(struct battery_info* binfo, int* terminals, int num_terms);

#endif