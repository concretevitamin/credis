#include <iostream>

#include <cpr/cpr.h>

#include "redismodule.h"

int ChainInitialize_RedisCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  REDISMODULE_NOT_USED(ctx);
  REDISMODULE_NOT_USED(argc);
  REDISMODULE_NOT_USED(argv);

  return REDISMODULE_OK;
}

extern "C" {

int RedisModule_OnLoad(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  REDISMODULE_NOT_USED(argc);
  REDISMODULE_NOT_USED(argv);
  if (RedisModule_Init(ctx,"credis",1,REDISMODULE_APIVER_1)
      == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  std::cout << "Initializing 'credis' module:" << std::endl;

  auto r = cpr::Get(cpr::Url{"localhost:8500/v1/catalog/nodes"},
                    cpr::Header{{"accept", "application/json"}});
  std::cout << "Consul nodes: " << r.text << std::endl;

  if (RedisModule_CreateCommand(ctx,"credis.initialize",
      ChainInitialize_RedisCommand,"write",1,1,1) == REDISMODULE_ERR)
    return REDISMODULE_ERR;

  return REDISMODULE_OK;
}

}