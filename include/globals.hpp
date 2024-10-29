#include <utils/env.hpp>
#include <utils/paths.hpp>
#include <utils/policies.hpp>
#include <cache/memory-storage.hpp>

namespace vs{

namespace globals{
extern policies_t policy;
extern path_env_t path_env;
extern js_rt_t js_rt;
extern cache::memstorage_t memstorage;

}
}