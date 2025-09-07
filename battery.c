#include "battery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct battery_info* load_battery_info(const char* filename, int num_terminals) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Warning: Cannot open battery file %s, using default levels\n", filename);
        
        struct battery_info* binfo = malloc(sizeof(struct battery_info));
        binfo->num_terminals = num_terminals;
        binfo->battery_levels = malloc(num_terminals * sizeof(double));
        binfo->battery_weight = 1.0;
        for (int i = 0; i < num_terminals; i++) {
            binfo->battery_levels[i] = 100.0;
        }
        return binfo;
    }
    
    struct battery_info* binfo = malloc(sizeof(struct battery_info));
    binfo->num_terminals = num_terminals;
    binfo->battery_levels = malloc(num_terminals * sizeof(double));
    

    if (fscanf(fp, "%lf", &binfo->battery_weight) != 1) {
        binfo->battery_weight = 1.0;
    }
    

    for (int i = 0; i < num_terminals; i++) {
        if (fscanf(fp, "%lf", &binfo->battery_levels[i]) != 1) {
            fprintf(stderr, "Warning: Missing battery level for terminal %d, using 100%%\n", i);
            binfo->battery_levels[i] = 100.0;
        }
    }
    
    fclose(fp);
    return binfo;
}

void free_battery_info(struct battery_info* binfo) {
    if (binfo) {
        free(binfo->battery_levels);
        free(binfo);
    }
}

double compute_battery_penalty(struct battery_info* binfo, int* terminals, int num_terms) {
    if (!binfo || num_terms == 0) return 0.0;
    
    double total_penalty = 0.0;
    double min_battery = 100.0;
    
   
    for (int i = 0; i < num_terms; i++) {
        int term_idx = terminals[i];
        if (term_idx >= 0 && term_idx < binfo->num_terminals) {
            if (binfo->battery_levels[term_idx] < min_battery) {
                min_battery = binfo->battery_levels[term_idx];
            }
        }
    }
    
   
    total_penalty = binfo->battery_weight * (100.0 - min_battery) / 100.0;
    
    return total_penalty;
}