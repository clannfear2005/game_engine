#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

/* Summary:
   > Entity represents a game object (player, enemy). Component represents behaviours of that entity (position, rendering).
   > Entities own components, and components define the entity's behaviour.
   > Component is a base class - derived classes implement their own custom logic 
   > Virtual functions allow different types of components to update/draw differently
*/

using ComponentID = std::size_t; // Component ID is size t - the max size of a theoretically possible array or object

inline ComponentID getComponentTypeID() // Where an inline method is called, it is replaced with the code within it - making it faster for small functions
{
    static ComponentID lastID = 0;
    return lastID++;
}

/* A template allows you to specify the datatype as a parameter (typename T). noexcept makes it not throw exceptions, which can make it more efficient if
   you are sure it will not create an exception.*/

template <typename T> inline ComponentID getComponentTypeID() noexcept // For specific data types
{
    static_assert (std::is_base_of<Component, T>::value, "");
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

/* constexpr tells the compiler that the value 32 is known at compile time, and it cannot change during run time. Used for constants that are fundamental to 
   program logic.*/

constexpr std::size_t maxComponents = 32;

/* The 'using' keyword makes the variable shorthand for std::bitset<maxComponents>, or something else. It is for specifying type names in shorthand*/

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
    public:

        Entity* entity; // Pointer to the entity that owns this component. Links the component to the entity its a part of.
        
        // Virtual methods allow the derived components to override behaviour

        virtual void init() {}
        virtual void update() {}
        virtual void draw() {}

        virtual ~Component() {}

    private:
};

class Entity
{
    public:
        
        void update() // Loops through all components, updating them and redrawing them all.
        {
            /*
                The auto& tells the compiler to deduce the type, and use a REFERENCE to the element, which avoids copying each element
                This makes it much more efficient for large objects, and means that modifications to the element will affect the original
            */
            for (auto& c : components ) c->update();
        }

        void draw()
        {
            for ( auto& c : components ) c->draw();
        }

        bool isActive() const { return active; }
        
        void destroy() { active = false; }

        // The const keyword at the end means that the member variables of the class cannot be altered.
        template <typename T> bool hasComponent() const
        {
            return componentBitSet[getComponentTypeID<T>()]; // Checks if there is a component attached to this point.
        }

        /*
            TArgs is a parameter pack that allows the function to accept a variable number of additional arguments of any type.
            The ... indicates that TArgs is a pack, which can expand to zero or more types

            'T& addComponent' tells us that addComponent will return a reference to an object of type T.
            'TArgs&&... mArgs' is the functions parameter list, where mArgs represents a parameter pack of forwarded arguments.
            Each argument in this pack is of type TArgs&&, which is a 'forwarding reference'. This allows you to perfectly forward the arguments
            to the constructor of T.
            TArgs&& is a special type of reference that can bind to both lvalues and rvalues. It allows for efficient forwarding without unnecessary copies.
        */

        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs)
        {
            // Creates a pointer c of type T, pointing to the newly created object.
            // Forwards each element of the parameter pack (mArgs) to the constructor of T. Ensures that each argument is forwarded exactly as it was passed to
            // addComponent (either as an lvalue or an rvalue)

            T* c(new T(std::forward<TArgs>(mArgs)...));
            c->entity = this;
            std::unique_ptr<Component> uPtr{ c };
            components.emplace_back(std::move(uPtr));

            componentArray[getComponentTypeID<T>()] = c;
            componentBitSet[getComponentTypeID<T>()] = true;

            c->init();
            
            return *c;
        }

        template<typename T> T& getComponent() const
        {
            auto ptr(componentArray[getComponentTypeID<T>()]);
            return *static_cast<T*>(ptr);
        }
        
    private:

        bool active = true; // Tracks whether an entity is active

        /*
            The vector is a dynamic array that holds the components - different to a linked list as elements are still stored contiguously in memory - like an array,
            making access by index O(1) fast.
            When the vector grows above its capacity, it allocates a larger block of memory
            There is a smart pointer that owns the component; when the entity is destroyed, unique_ptr will automatically deallocate the memeory for the component
        */
        std::vector<std::unique_ptr<Component>> components;

        ComponentArray componentArray; // Array for fast component access

        /*
            A bitset which tracks which components are attached to the entity - efficient for lookups. There is a max of 32 components for each entity
            The bitset is a fixed size array of bits, each bit representing whether a specific component type exists on the entity 
        */
        ComponentBitSet componentBitSet; 
};

class Manager
{
    public:

        void update()
        {
            for (auto& e : entities)
            {
                e->update();
            }
        }

        void draw()
        {
            for (auto& e : entities)
            {
                e->draw();
            }
        }

        void refresh()
        {
            entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity)
            {
                return !mEntity->isActive();
            }),
            std::end(entities));
        }

        Entity& addEntity()
        {
            Entity* e = new Entity();
            std::unique_ptr<Entity> uPtr { e };
            entities.emplace_back(std::move(uPtr));

            return *e;
        }

    private:
        std::vector<std::unique_ptr<Entity>> entities;
};