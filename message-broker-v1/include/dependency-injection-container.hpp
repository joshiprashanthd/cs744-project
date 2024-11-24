#pragma once

#ifndef DEPENDENCY_INJECTION_CONTAINER_HPP
#define DEPENDENCY_INJECTION_CONTAINER_HPP

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>

namespace messagebrokerv1
{

    class DependencyInjectionContainer
    {
    private:
        static std::unordered_map<std::type_index, std::shared_ptr<void>> container;

    public:
        template <typename T>
        static void registerObject(std::shared_ptr<T> object)
        {
            if (!object)
            {
                throw std::invalid_argument("Cannot register a nullptr.");
            }
            container[std::type_index(typeid(T))] = object;
        }

        template <typename T>
        static std::shared_ptr<T> resolve()
        {
            auto it = container.find(std::type_index(typeid(T)));
            if (it != container.end())
            {
                return std::static_pointer_cast<T>(it->second);
            }
            throw std::runtime_error("No object registered for the requested type.");
        }
    };

} // namespace messagebrokerv1

#endif // DEPENDENCY_INJECTION_CONTAINER_HPP
