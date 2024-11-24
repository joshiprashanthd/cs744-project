#include "dependency-injection-container.hpp"

namespace messagebrokerv1
{
    // Define the static container
    std::unordered_map<std::type_index, std::shared_ptr<void>> DependencyInjectionContainer::container;

} // namespace messagebrokerv1
