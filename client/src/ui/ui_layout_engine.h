#pragma once

/**
 * UI Layout Engine for managing UI elements in a scene.
 */

namespace ME {

// Forward declarations
class UIElement;

class UILayoutEngine {
   public:
    UILayoutEngine();
    ~UILayoutEngine();

    // Initialize the UI Layout Engine.
    void Init();

    // Update the layout of UI elements.
    void Update();

    // Add a UI element to the layout engine.
    void AddElement(class UIElement* element);

    // Remove a UI element from the layout engine.
    void RemoveElement(class UIElement* element);

   private:
    // Internal method to recalculate layouts.
    void RecalculateLayouts();

    // List of UI elements managed by the layout engine.
    UIElement** uiElements;
    size_t elementCount;
};

}  // namespace ME
