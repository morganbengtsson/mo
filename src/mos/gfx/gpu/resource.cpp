#include <mos/gfx/gpu/resource.hpp>

namespace mos::gfx::gpu {

Resource::Resource(const int id) : id_(id) {

}

bool Resource::valid() const {
  return id_ != invalid_id;
}

int Resource::id() const {
  return id_;
}
}
