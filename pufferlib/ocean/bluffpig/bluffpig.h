/*
 * File: bluffpig.h
 *
 * Description: Bluffpig is a variant of the classic dice game Pig, augmented
 * with a bluffing mechanism. This file is where the actual game lives.
 */

#include <stdlib.h>
#include <string.h>
#include "raylib.h"

const unsigned char ROLL = 0;
const unsigned char HOLD = 1;

// Required struct. Only use floats!
typedef struct {
    float perf; // Recommended 0-1 normalized single real number perf metric
    float score; // Recommended unnormalized single real number perf metric
    float episode_return; // Recommended metric: sum of agent rewards over episode
    float episode_length; // Recommended metric: number of steps of agent episode
    // Any extra fields you add here may be exported to Python in binding.c
    float n; // Required as the last field 
} Log;

// Required that you have some struct for your env
// Recommended that you name it the same as the env file
typedef struct {
    Log log;
    float* observations; // [agent_score, opponent_score, turn_score, last_roll, current_player]
    int* actions; // 0=roll, 1=hold
    float* rewards;
    unsigned char* terminals; // Required. We don't yet have truncations as
                              // standard yet
  int target_score; // winning threshold, typically 100
  int scores[2];  // total banked scores: [agent, opponent]
  int turn_score;  // points accumulated this turn, forfeited on a 1
  int current_player; // 0=agent, 1=opponent
  int last_roll;  // last die result (1-6), part of the observation
  int n_rolls_turn; // number of rolls this turn.
  int tick;       // step counter for episode length logging.
  int wins[2];    // number of wins for each player.
} Bluffpig;

void add_log(Bluffpig* env) {
    env->log.perf += (env->rewards[0] > 0) ? 1 : 0;
    env->log.score += env->rewards[0];
    env->log.episode_length += env->tick;
    env->log.episode_return += env->rewards[0];
    env->log.n++;
}

// Required function
void c_reset(Bluffpig* env) {
    env->turn_score = 0;
    env->n_rolls_turn = 0;
    env->scores[0] = env->scores[1] = 0;
    env->tick = 0;
    env->current_player = rand() % 2;
    env->last_roll = rand() % 6 + 1;
    env->observations[0] = env->scores[0] / (float)env->target_score;
    env->observations[1] = env->scores[1] / (float)env->target_score;
    env->observations[2] = env->turn_score / (float)env->target_score;
    env->observations[3] = env->last_roll / 6.0F;
}

// Required function
void c_step(Bluffpig* env) {
    env->tick += 1;

    int action = env->actions[0];
    env->terminals[0] = 0;
    env->rewards[0] = 0;

    if(env->current_player == 0){
      // Agent strategy is given by the policy network.
      if (action == HOLD) {  // Hold
        env->scores[0] += env->turn_score;
        env->current_player = (env->current_player + 1) % 2;
        env->turn_score = 0;
        env->n_rolls_turn = 0;
      }
      else if (action == ROLL) {
        env->last_roll = (rand() % 6) + 1;
        env->n_rolls_turn += 1;
        if(env->last_roll == 1){
          env->turn_score = 0;
          env->current_player = (env->current_player + 1) % 2;
          goto cleanup;
        }
        env->turn_score += env->last_roll;
      }
    }

    if(env->current_player == 1){
      // Opponent strategy
      while(env->turn_score <= 20 && env->scores[1] + env->turn_score < env->target_score){
        env->last_roll = (rand() % 6) + 1;
        if(env->last_roll == 1){
          env->turn_score = 0;
          env->n_rolls_turn = 0;
          env->current_player = (env->current_player + 1) % 2;
          goto cleanup;
        }
        env->turn_score += env->last_roll;
        env->n_rolls_turn += 1;
      }
      env->scores[1] += env->turn_score;
      env->current_player = (env->current_player + 1) % 2;
      env->turn_score = 0;
    }

    if(env->scores[0] >= env->target_score){
      env->rewards[0] = 1;
      env->wins[0] += 1;
    }
    else if(env->scores[1] >= env->target_score){
      env->rewards[0] = -1;
      env->wins[1] += 1;
    }

    if(env->scores[0] >= env->target_score ||
       env->scores[1] >= env->target_score){
      add_log(env);
      c_reset(env);
      env->terminals[0] = 1;
    }

 cleanup:
    env->observations[0] = env->scores[0] / (float)env->target_score;
    env->observations[1] = env->scores[1] / (float)env->target_score;
    env->observations[2] = env->turn_score / (float)env->target_score;
    env->observations[3] = env->last_roll / 6.0F;
}

// Required function. Should handle creating the client on first call
void c_render(Bluffpig* env) {
    if (!IsWindowReady()) {
        InitWindow(400, 260, "PufferLib Bluffpig");
        SetTargetFPS(5);
    }

    if (IsKeyDown(KEY_ESCAPE)) {
        exit(0);
    }

    BeginDrawing();
    ClearBackground((Color){6, 24, 24, 255});

    DrawText(TextFormat("Agent score:    %d", env->scores[0]), 20, 20,  20, (Color){0, 187, 187, 255});
    DrawText(TextFormat("Opponent score: %d", env->scores[1]), 20, 50,  20, (Color){187, 0, 0, 255});
    DrawText(TextFormat("Turn score:     %d", env->turn_score), 20, 80,  20, (Color){187, 187, 0, 255});
    DrawText(TextFormat("Last roll:      %d", env->last_roll),  20, 110, 20, (Color){187, 187, 187, 255});
    DrawText(TextFormat("Turn: %s", env->current_player == 0 ? "Agent" : "Opponent"), 20, 140, 20, WHITE);
    DrawText(TextFormat("Num rolls:      %d", env->n_rolls_turn), 20, 170, 20, WHITE);
    DrawText(TextFormat("Wins: %d - %d", env->wins[0], env->wins[1]), 20, 200, 20, WHITE);

    EndDrawing();
}

// Required function. Should clean up anything you allocated
// Do not free env->observations, actions, rewards, terminals
void c_close(Bluffpig* env) {
    if (IsWindowReady()) {
        CloseWindow();
    }
}
