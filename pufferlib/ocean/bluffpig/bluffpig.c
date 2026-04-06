/* Pure C demo file for Bluffpig. Build it with:
 * bash scripts/build_ocean.sh target local (debug)
 * bash scripts/build_ocean.sh target fast
 * We suggest building and debugging your env in pure C first. You
 * get faster builds and better error messages. To keep this example
 * simple, it does not include C neural nets. See Target for that.
 */

#include "bluffpig.h"

int main() {
    Bluffpig env = {.target_score = 100};
    env.observations = (float*)calloc(4, sizeof(float));
    env.actions = (int*)calloc(1, sizeof(int));
    env.rewards = (float*)calloc(1, sizeof(float));
    env.terminals = (unsigned char*)calloc(1, sizeof(unsigned char));

    c_reset(&env);
    c_render(&env);
    while (!WindowShouldClose()) {
      if (IsKeyPressed(KEY_R)){
        env.actions[0] = 0;
        c_step(&env);
      }
      if (IsKeyPressed(KEY_H)){
        env.actions[0] = 1;
        c_step(&env);
      }
      c_render(&env);
    }
    free(env.observations);
    free(env.actions);
    free(env.rewards);
    free(env.terminals);
    c_close(&env);
}

