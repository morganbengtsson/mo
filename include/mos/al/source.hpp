#pragma once

#include <chrono>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/efx-presets.h>

#include <mos/aud/source.hpp>
#include <mos/apu/source.hpp>
#include <mos/al/resource.hpp>

namespace mos::al {

/** Source living on the APU */
class Source final : public Resource {
  friend class Renderer;
public:
  using Time_point = std::chrono::time_point<std::chrono::system_clock,
                                             std::chrono::nanoseconds>;

public:
  void update(const mos::apu::Source &Source);
  void stop();

private:
  Source(const mos::apu::Source &Source);
};
}
