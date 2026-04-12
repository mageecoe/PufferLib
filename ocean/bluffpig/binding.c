#include "bluffpig.h"
#define OBS_SIZE 4
#define NUM_ATNS 1
#define ACT_SIZES {2}
#define OBS_TENSOR_T FloatTensor

#define Env Bluffpig
#include "vecenv.h"

void my_init(Env* env, Dict* kwargs) {
    env->target_score = dict_get(kwargs, "target_score")->value;
    c_reset(env);
}

void my_log(Log* log, Dict* out) {
    dict_set(out, "perf", log->perf);
    dict_set(out, "score", log->score);
    dict_set(out, "episode_return", log->episode_return);
    dict_set(out, "episode_length", log->episode_length);
}
