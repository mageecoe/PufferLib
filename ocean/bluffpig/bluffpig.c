/* Pure C demo file for Bluffpig. Build it with:
 * bash build.sh bluffpig --local (debug)
 * bash build.sh bluffpig --fast
 * We suggest building and debugging your env in pure C first. You
 * get faster builds and better error messages. To keep this example
 * simple, it does not include C neural nets. See Target for that.
 */

#include "bluffpig.h"
#include "puffernet.h"

int main() {
    Bluffpig env = {.target_score = 100};
    env.observations = (float*)calloc(4, sizeof(float));
    env.actions = (float*)calloc(1, sizeof(float));
    env.rewards = (float*)calloc(1, sizeof(float));
    env.terminals = (float*)calloc(1, sizeof(float));

    int logit_sizes[1] = {2};
    Weights* weights = load_weights("resources/bluffpig/puffer_bluffpig_weights.bin", 133123);
    LinearLSTM* net = make_linearlstm(weights, 1, 4, logit_sizes, 1); // 4 obs, 2 actions

    c_reset(&env);
    c_render(&env);
    while (!WindowShouldClose()) {
      if(env.current_player == 0){
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
          if (IsKeyPressed(KEY_R)){
            env.actions[0] = 0;
            c_step(&env);
          }
          if (IsKeyPressed(KEY_H)){
            env.actions[0] = 1;
            c_step(&env);
          }
        }
        else {
          forward_linearlstm(net, env.observations, env.actions);
          c_step(&env);
        }
      }
      else {
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
