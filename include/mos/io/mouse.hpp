#pragma once

#include <glm/glm.hpp>
#include <unordered_set>
#include <array>




namespace mos {
namespace io {

class Mouse {


public:
  Mouse();
  enum class Button : int {
    Button_1 = 0,
    Button_2 = 1,
    Button_3 = 2
  };

  enum class Action : int {
    Release = 0,
    Press = 1,
    Repeat = 2
  };

  struct Event {
    Button button;
    Action action;

    bool operator==(const Event &event) const{
      return button == event.button && action == event.action;
    }

  };

private:
  struct EventHash {
    size_t operator()(const Event& e) const {
      return ((std::hash<int>()(static_cast<int>(e.button)) ^ (std::hash<int>()(static_cast<int>(e.action)) << 1)) >> 1);
    }
  };
public:

  using Events = std::unordered_set<Event, EventHash>;

  glm::dvec2 position{};
  glm::dvec2 old_position{};
  Events events;
  std::array<bool, 3> buttons{false, false, false};
};
}
}


